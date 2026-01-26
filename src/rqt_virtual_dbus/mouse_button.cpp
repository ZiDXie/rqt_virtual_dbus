//
// Created by wiselook on 2025/12/25.
//

#include "rqt_virtual_dbus/mouse_button.h"

MouseButton::MouseButton(QWidget *parent, GlobalMouseInput *mouse)
    : QRadioButton(parent), mouse_(mouse) {
  resize(parent->width(), parent->height());
  this->setText("Use mouse");

  esc_shortcut_ = new QShortcut(QKeySequence(Qt::Key::Key_Escape), this);
  space_shortcut_ = new QShortcut(QKeySequence(Qt::Key::Key_Space), this);
  esc_shortcut_->setContext(Qt::ApplicationShortcut);
  space_shortcut_->setContext(Qt::ApplicationShortcut);

  connect(this, &QRadioButton::clicked, this, &MouseButton::updateState);
  connect(esc_shortcut_, &QShortcut::activated, this, [=]() {
    mouse_->setMouseLockEnabled(false);
    mouse_->stop();
    QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
    this->setChecked(false);
  });
  connect(space_shortcut_, &QShortcut::activated, this, [=]() {
    QPoint locked_mouse_center = parent->mapToGlobal(parent->rect().center());
    mouse_->setLockCenter(locked_mouse_center);
    mouse_->setMouseLockEnabled(true);
    mouse_->start();
    QGuiApplication::setOverrideCursor(Qt::BlankCursor);
    this->setChecked(true);
  });
}

MouseButton::~MouseButton() {}

void MouseButton::updateState() {
  read_mouse_ = this->isChecked();
  if (read_mouse_) {
    mouse_->start();
  } else {
    mouse_->stop();
  }
}

void MouseButton::resizeEvent(QResizeEvent *event) {
  resize(this->parentWidget()->width(), this->parentWidget()->height());
}

void MouseButton::keyPressEvent(QKeyEvent *ev) {
  if (!slip_state_)
    return;

  switch (ev->key()) {
  case Qt::Key_Escape:

    break;
  }
}

void MouseButton::keyReleaseEvent(QKeyEvent *ev) {
  if (!slip_state_)
    return;

  switch (ev->key()) {
  case Qt::Key_Escape:

    break;
  }
}

void MouseButton::mouseMoveEvent(QMouseEvent *ev) {}
