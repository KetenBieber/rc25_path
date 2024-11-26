
/**
 * @file tf_broadcast.h
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :使用两个命名空间，本节点和launch文件有较大的关系，需要通过launch来传入命名空间和参数
 *        公共命名空间：用于订阅公共话题，保持与全局命名空间的交互，如发布tf变换
 *        私有命名空间：用于读取每个Nodelet实例的私有参数，确保每个实例有独立的配置
 *                    参数通过<param>标签在launch文件中传递，属于该Nodelet的私有命名空间
 *        
 * @versioninfo :
 */
#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf/transform_datatypes.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

#include "rc_msgs/ActionData.h"

/* 使用nodelet，需要注意：插件类必须留出默认构造函数，然后实现onInit函数 */
/* 注意这俩函数实现，都不能有形参 */

namespace tf_broadcast
{

class tf_broadcaster :public nodelet::Nodelet
{
public:
    tf_broadcaster();
    virtual ~tf_broadcaster();
    void spin();

private:
    virtual void onInit();

    bool readParameters();

    void topicCallback(const rc_msgs::ActionData::ConstPtr& msg);

    void tfProcess();

private:
    /* NodeHandle 成员变量 */
    ros::NodeHandle nh_;
    ros::NodeHandle private_nh_;


    ros::Subscriber subscriber_;
    ros::Publisher publisher_;
    
    std::string pub_ns_;
    std::string sub_ns_;
    std::string tf_ns_;

    std::string subscriberTopic_; 
    std::int32_t sub_queuelength_;

    std::string publisherTopic_;
    std::int32_t pub_queuelength_;
    std::int32_t pub_frequency_;
    ros::Rate loop_rate{10};//default:10hz

    tf2_ros::TransformBroadcaster odom_broadcaster;
    geometry_msgs::TransformStamped robot_pos;

    rc_msgs::ActionData robot_sensor;

    ros::Time current_time, last_time;// 时钟挂载

};

}