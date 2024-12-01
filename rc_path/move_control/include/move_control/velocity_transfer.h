/**
 * @file velocity_transfer.h
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-30
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :速度发布转换插件，可以通过在launch中直接指定参数来启动不同速度量的转换(输入)
 *        设置参数：source_frame  已有的坐标系
 *                 target_frame  目标坐标系
 * 
 * @versioninfo :
 */
#pragma once

#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>
#include <geometry_msgs/Twist.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>


namespace v_transfer
{

class v_transfer : public nodelet::Nodelet
{
public:
    v_transfer();
    ~v_transfer() = default;

private:

    virtual void onInit();

    bool readParameters();

    void TopicsCallback(const geometry_msgs::Twist::ConstPtr& msg);

    void spin();

private:

    ros::NodeHandle nh_;

    ros::Subscriber subscriber_;
    std::string subscribeTopic_;

    ros::Publisher publisher_;
    std::string publishTopic_;

    std::string source_frame_;
    std::string target_frame_;

    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;  // 定义 TF 监听器


};

}

namespace tf2
{
    // 使用特定模版，需要用户自己实现
template <>
void doTransform<geometry_msgs::Twist>(
    const geometry_msgs::Twist& input,
    geometry_msgs::Twist& output,
    const geometry_msgs::TransformStamped& transform)
{
    tf2::Transform tf;
    tf2::fromMsg(transform.transform, tf);

    // 线速度转换
    tf2::Vector3 linear_vec(input.linear.x, input.linear.y, input.linear.z);
    tf2::Vector3 rotated_linear = tf.getBasis() * linear_vec;
    output.linear.x = rotated_linear.x();
    output.linear.y = rotated_linear.y();
    output.linear.z = rotated_linear.z();

    // 角速度转换（仅绕z轴）
    tf2::Vector3 angular_vec(input.angular.x, input.angular.y, input.angular.z);
    tf2::Vector3 rotated_angular = tf.getBasis() * angular_vec;
    output.angular.x = rotated_angular.x();
    output.angular.y = rotated_angular.y();
    output.angular.z = rotated_angular.z();
}

}
