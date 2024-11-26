/**
 * @file sim_bringup.h
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note : 一键启动底盘控制器（加速度设定话题发布），必须在设定加速度之后才可以驱动底盘控制器
 * @versioninfo :
 */
#pragma once

#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>
#include "rc_msgs/ChassisCmd.h"


namespace sim_bringup
{

class sim_bringup : public nodelet::Nodelet
{
public:
    sim_bringup();
    ~sim_bringup() = default;

private:
    virtual void onInit();

private:

    ros::NodeHandle nh_;
    ros::Publisher pub_1;
    ros::Publisher pub_2;
    ros::Publisher pub_3;
    ros::Publisher pub_4;
    
};

}