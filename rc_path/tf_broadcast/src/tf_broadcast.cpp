/**
 * @file tf_broadcast.cpp
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
#include "tf_broadcast/tf_broadcast.h"

PLUGINLIB_EXPORT_CLASS(tf_broadcast::tf_broadcaster, nodelet::Nodelet)

namespace tf_broadcast
{

void tf_broadcaster::onInit()
{
    /* 获取公共和私有 NodeHandle */
    nh_ = getNodeHandle();
    private_nh_ = getPrivateNodeHandle();

    if(!readParameters())
    {
        ROS_ERROR("Could not load Parameters!");
        ros::requestShutdown();
        return;
    }
    loop_rate = ros::Rate(pub_frequency_);

    subscriber_ = nh_.subscribe(subscriberTopic_, sub_queuelength_, &tf_broadcaster::topicCallback, this);
    current_time = ros::Time::now();
}

tf_broadcaster::tf_broadcaster()                       
{

}

tf_broadcaster::~tf_broadcaster()
{

}

void tf_broadcaster::spin()
{
    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
}

bool tf_broadcaster::readParameters()
{
    private_nh_.param<std::string>("sub_ns", sub_ns_, "Robot1Action");
    private_nh_.param<std::string>("pub_ns", pub_ns_, "Robot1Odometry");
    private_nh_.param<std::string>("tf_ns", tf_ns_, "tfRobot1");
    /* 接收sub.yaml */
    if(!private_nh_.getParam(sub_ns_ + "/topic_name",subscriberTopic_))
    {
        ROS_ERROR("Couldn't get the param:[%s/topic_name]",sub_ns_.c_str());
        return false;
    }
    if(!private_nh_.getParam(sub_ns_ + "/queue_size",sub_queuelength_))
    {
        ROS_ERROR("Couldn't get the param:[%s/queue_size]",sub_ns_.c_str());
        return false;        
    }
    /* 接收pub.yaml */
    if(!private_nh_.getParam(pub_ns_ + "/topic_name",publisherTopic_))
    {
        ROS_ERROR("Couldn't get the param:[%s/topic_name]",pub_ns_.c_str());
        return false;        
    }
    if(!private_nh_.getParam(pub_ns_ + "/queue_size", pub_queuelength_))
    {
        ROS_ERROR("Couldn't get the param:[%s/queue_size]",pub_ns_.c_str());
        return false;        
    }
    if(!private_nh_.getParam(pub_ns_ + "/frequency", pub_frequency_))
    {
        ROS_ERROR("Couldn't get the param:[%s/frequency]",pub_ns_.c_str());
        return false;
    }
    if(!private_nh_.getParam(tf_ns_ + "/frame_id",robot_pos.header.frame_id))
    {
        ROS_ERROR("Couldn't get the param:[%s/frame_id]",tf_ns_.c_str());
        return false;
    }
    if(!private_nh_.getParam(tf_ns_ + "/child_frame_id", robot_pos.child_frame_id))
    {
        ROS_ERROR("Couldn't get the param:[%s/child_frame_id]",tf_ns_.c_str());
        return false;
    }
    
    return true;
}

void tf_broadcaster::topicCallback(const rc_msgs::ActionData::ConstPtr& msg)
{
    // robot_pos.header = msg->header;
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(msg->yaw_angle);

    current_time = ros::Time::now();
    robot_pos.header.stamp = current_time;

    robot_pos.transform.translation.x = msg->pose_x;
    robot_pos.transform.translation.y = msg->pose_y;
    robot_pos.transform.translation.z = 0;
    robot_pos.transform.rotation = odom_quat;
    odom_broadcaster.sendTransform(robot_pos);
    /* 可以选择性地在这里加入时间判断，看看线程是否被阻塞严重 */
    // if((current_time - last_time)) 
    last_time = current_time;
    
}

void tf_broadcaster::tfProcess()
{
}


}
