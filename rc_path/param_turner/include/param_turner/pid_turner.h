/**
 * @file pid_turner.h
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-27
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :pid 动态调参插件
 * @versioninfo :
 */
#pragma once

#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>

#include "param_turner/pid_turnerConfig.h"


namespace pid_turner
{

class pid_turner : public nodelet::Nodelet
{
public:
    pid_turner();
    ~pid_turner() = default;

private:
    virtual void onInit();

    bool readParameters();

    void ServiceCallback(param_turner::pid_turnerConfig &config,uint32_t level);

    void spin();

private:
    ros::NodeHandle nh_;

    // 使用 boost::shared_ptr 管理 dynamic_reconfigure::Server
    boost::shared_ptr<dynamic_reconfigure::Server<param_turner::pid_turnerConfig>> server_;
    dynamic_reconfigure::Server<param_turner::pid_turnerConfig>::CallbackType callbacktype_;
    std::string ns_;


};


}

