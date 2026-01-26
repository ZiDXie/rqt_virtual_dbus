//
// Created by wiselook on 2025/12/25.
//

#pragma once

#include <QObject>
#include <QPoint>
#include <QTimer>

class GlobalMouseInput : public QObject {
  Q_OBJECT
public:
  explicit GlobalMouseInput(QObject *parent = nullptr);

  // 启停
  void start(int intervalMs = 16); // 默认 ~60Hz
  void stop();

  // 参数设置
  void setSensitivity(double s); // 0.05 ~ 0.3
  void setDeadzone(double dz);   // 0.03 ~ 0.08
  void setMaxDelta(double d);    // 像素限幅（30~60）

  // FPS 鼠标锁定
  void setMouseLockEnabled(bool enable);
  bool mouseLockEnabled() const;

  // 锁定中心（全局坐标）
  void setLockCenter(const QPoint &globalPos);

  // 当前输出
  double x() const; // [-1,1]
  double y() const;

signals:
  void updated(double x, double y);

private:
  void update();

  QTimer timer_;
  QPoint lastPos_;

  // 参数
  double sensitivity_ = 0.15f;
  double deadzone_ = 0.05f;
  double maxDelta_ = 50.0f;

  // FPS 锁定相关
  bool lockEnabled_ = false;
  bool justLocked_ = false;
  QPoint lockCenter_;

  // 输出
  double outX_ = 0.0f;
  double outY_ = 0.0f;
};
