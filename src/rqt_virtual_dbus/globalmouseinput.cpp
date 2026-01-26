//
// Created by wiselook on 2025/12/25.
//

#include "rqt_virtual_dbus/globalmouseinput.h"

#include <QCursor>
#include <algorithm>
#include <cmath>

GlobalMouseInput::GlobalMouseInput(QObject *parent) : QObject(parent) {
  connect(&timer_, &QTimer::timeout, this, &GlobalMouseInput::update);
}

void GlobalMouseInput::start(int intervalMs) {
  lastPos_ = QCursor::pos();
  timer_.start(intervalMs);
}

void GlobalMouseInput::stop() { timer_.stop(); }

void GlobalMouseInput::setSensitivity(double s) {
  sensitivity_ = std::clamp(s, 0.001, 10.0);
}

void GlobalMouseInput::setDeadzone(double dz) {
  deadzone_ = std::clamp(dz, 0.0, 0.5);
}

void GlobalMouseInput::setMaxDelta(double d) { maxDelta_ = std::max(d, 1.0); }

void GlobalMouseInput::setMouseLockEnabled(bool enable) {
  if (lockEnabled_ == enable)
    return;

  lockEnabled_ = enable;
  justLocked_ = true;

  if (lockEnabled_) {
    QCursor::setPos(lockCenter_);
    lastPos_ = lockCenter_;
  } else {
    lastPos_ = QCursor::pos();
  }
}

bool GlobalMouseInput::mouseLockEnabled() const { return lockEnabled_; }

void GlobalMouseInput::setLockCenter(const QPoint &globalPos) {
  lockCenter_ = globalPos;
}

double GlobalMouseInput::x() const { return outX_; }
double GlobalMouseInput::y() const { return outY_; }

static inline double applyDeadzone(double v, double dz) {
  return (std::abs(v) < dz) ? 0.0f : v;
}

void GlobalMouseInput::update() {
  QPoint cur = QCursor::pos();

  // 刚进入锁定模式，跳过一帧防止突变
  if (lockEnabled_ && justLocked_) {
    justLocked_ = false;
    lastPos_ = cur;
    outX_ = outY_ = 0.0f;
    return;
  }

  QPoint delta = cur - lastPos_;
  lastPos_ = cur;

  // FPS 模式：立刻拉回中心
  if (lockEnabled_) {
    QCursor::setPos(lockCenter_);
    lastPos_ = lockCenter_;
  }

  // 像素限幅
  double dx = std::clamp((double)delta.x(), -maxDelta_, maxDelta_);
  double dy = std::clamp((double)delta.y(), -maxDelta_, maxDelta_);

  // 归一化 [-1,1]
  double nx = dx / maxDelta_;
  double ny = dy / maxDelta_;

  // 防抖死区
  nx = applyDeadzone(nx, deadzone_);
  ny = applyDeadzone(ny, deadzone_);

  // 灵敏度
  outX_ = std::clamp(nx * sensitivity_, -1.0, 1.0);
  outY_ = std::clamp(ny * sensitivity_, -1.0, 1.0);

  emit updated(outX_, outY_);
}
