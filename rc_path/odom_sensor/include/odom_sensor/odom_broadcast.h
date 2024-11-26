/**
 * @file odom_broadcast.h
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-27
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :通过监听tf，对坐标数据做微分处理，得到机器人当前世界坐标系下速度
 *        @todo:或许会需要得到机器人下坐标系的速度？可以加一个接口把速度发布出去，但是当前只需要世界坐标系下
 *              速度做闭环
 * @versioninfo :
 */
#pragma once

#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>
#include <nav_msgs/Odometry.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>



namespace odom_sensor
{

class odom_broadcast : public nodelet::Nodelet
{
public:
    odom_broadcast();
    ~odom_broadcast() = default;
private:
    virtual void onInit();

    void run(const ros::TimerEvent& event);
    void getVel();

private:
    ros::NodeHandle nh_;

    std::string ns_;

    ros::Publisher pub_odom_;
    
    nav_msgs::Odometry odom_;


    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;  // 定义 TF 监听器


    ros::Timer timer_;// 定时器
private:
    geometry_msgs::TransformStamped transformStamped,last_transformStamped;
    ros::Time current_time,last_time;// 时间帧，用于微分求速度
    bool first_time = false;
};


}
