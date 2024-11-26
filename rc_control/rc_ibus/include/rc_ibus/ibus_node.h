//
// Created by myx on 2022/11/29.
//

#pragma once

#include <ros/ros.h>
#include "rc_ibus/ibus.h"
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>

#include <rc_msgs/IbusData.h>

namespace rc_ibus
{

class IBusNode : public nodelet::Nodelet
{
private:
  virtual void onInit();

private:

  ros::NodeHandle nh_;
  ros::Publisher ibus_pub_;
  std::string serial_port_;
  std::string publishTopic_;
  rc_msgs::IbusData Ibus_cmd_;
  rc_ibus::IBus ibus_{};

  ros::Timer timer_;// 定时器

public:
  IBusNode();
  ~IBusNode() = default;
  void run(const ros::TimerEvent& event);
};

}