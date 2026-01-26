//
// Created by wiselook on 2025/12/25.
//

#pragma

#include <QDebug>
#include <QGuiApplication>
#include <QKeySequence>
#include <QMouseEvent>
#include <QPainter>
#include <QRadioButton>
#include <QShortcut>
#include <QTimer>
#include <QWidget>
#include <ros/ros.h>

#include "rm_msgs/DbusData.h"
#include "rqt_virtual_dbus/globalmouseinput.h"

class MouseButton : public QRadioButton {
  Q_OBJECT

public:
  explicit MouseButton(QWidget *parent = nullptr,
                       GlobalMouseInput *mouse = nullptr);
  ~MouseButton();

  void setDbusData(rm_msgs::DbusData *data) { dbus_data_ = data; }
  void updateSlip(bool state) { slip_state_ = state; }
  void updateState();
  void focusButton();
  
protected:
  void keyPressEvent(QKeyEvent *ev) override;
  void keyReleaseEvent(QKeyEvent *ev) override;
  void resizeEvent(QResizeEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;

private:
  bool slip_state_, read_mouse_ = false;
  rm_msgs::DbusData *dbus_data_;
  QShortcut *esc_shortcut_, *space_shortcut_;
  GlobalMouseInput *mouse_;
};
