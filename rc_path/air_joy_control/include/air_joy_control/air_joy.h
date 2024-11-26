/**
 * @file air_joy.h
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :
 * @versioninfo :
 */
#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>
#include <geometry_msgs/Twist.h>
#include "rc_msgs/IbusData.h"
#include "rc_msgs/ActionData.h"

#define PI 3.1415926


namespace air_joy
{

class air_joy_device :public nodelet::Nodelet
{
public:
    air_joy_device();
    virtual ~air_joy_device();

private:
    virtual void onInit();

    bool readParameters();

    void topicCallback(const rc_msgs::IbusData::ConstPtr& msg);

    void dataProcess();

    void topicChange(std::string &temp_1 , std::string &temp_2);

    void yawCallback(const rc_msgs::ActionData::ConstPtr& msg);

private:
    ros::NodeHandle nh_;
    
    ros::Publisher publisher_;
    std::string publishTopic_;
    std::int32_t pub_queuelength_;

    ros::Subscriber subscriber_;
    std::string subscriberTopic_;
    std::int32_t sub_queuelength_;

    ros::Subscriber getYaw_;
    std::string yawSource_;
    float yaw;

    geometry_msgs::Twist robot;// 机器人坐标系下速度指令
    geometry_msgs::Twist world;// 世界坐标系下速度指令
    ros::Time current_time, last_time;

    bool more_;
    std::string ns_;
};


}