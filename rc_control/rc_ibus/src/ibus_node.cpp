/**
 * @file ibus_node.cpp --- adapted from myx
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :改写为nodelet插件类，使其可以直接加载到控制进程中，节省资源传递
 * @versioninfo :
 */
#include "rc_ibus/ibus_node.h"

PLUGINLIB_EXPORT_CLASS(rc_ibus::IBusNode, nodelet::Nodelet)


namespace rc_ibus
{

void IBusNode::onInit()
{ 
  nh_ = getPrivateNodeHandle();
  nh_.param<std::string>("air_device",publishTopic_,"/robot1");
  nh_.param<std::string>("serial_port",serial_port_,"/dev/ttyUSB0");
  ROS_INFO("set air_device to [%s]",publishTopic_.c_str());
  ROS_INFO("set serial port to [%s]",serial_port_.c_str());
  ibus_pub_ = nh_.advertise<rc_msgs::IbusData>(publishTopic_ + "/ibus_data",100);
  ibus_.init(serial_port_.data());

  /* 启动定时器 */
  double rate = 1000.0;// 运行频率
  timer_ = nh_.createTimer(ros::Duration(1.0 / rate),&IBusNode::run,this);
}

IBusNode::IBusNode()
{

}

void IBusNode::run(const ros::TimerEvent& event)
{
  ibus_.read();
  ibus_.getData(&Ibus_cmd_);
  ibus_pub_.publish(Ibus_cmd_);
}

}
