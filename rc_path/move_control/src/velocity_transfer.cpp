#include "move_control/velocity_transfer.h"

PLUGINLIB_EXPORT_CLASS(v_transfer::v_transfer, nodelet::Nodelet)

namespace v_transfer
{

v_transfer::v_transfer(): tf_buffer_(), tf_listener_(tf_buffer_)
{
}

void v_transfer::onInit()
{
    ROS_INFO("Enter!");
    nh_ = getPrivateNodeHandle();
    if(!readParameters())
    {
        ROS_ERROR("Could not load Parameters!");
        return;
    }
    ROS_INFO("successfully load the parameters!");
    subscriber_ = nh_.subscribe(subscribeTopic_, 1000, &v_transfer::TopicsCallback, this);
    publisher_ = nh_.advertise<geometry_msgs::Twist>(publishTopic_,1000);
    ROS_INFO("Subscribed to topic: %s", subscriber_.getTopic().c_str());
    ROS_INFO("Advertised topic: %s", publisher_.getTopic().c_str());
    ROS_INFO("Sub and pub are both ready!");
    // spin();
}

bool v_transfer::readParameters()
{
    nh_.param<std::string>("source_frame",source_frame_,"robot2/odom");
    nh_.param<std::string>("target_frame",target_frame_,"robot2/base_link");
    nh_.param<std::string>("sub_topics",subscribeTopic_,"/cmd_vel");
    nh_.param<std::string>("pub_topics",publishTopic_,"/robot2/cmd_vel");
    return true;
}


void v_transfer::TopicsCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    // ROS_INFO("TopicsCallback called");
    ros::Time time = ros::Time(0);
    geometry_msgs::TransformStamped transformStamped;
    try
    {
        transformStamped = tf_buffer_.lookupTransform(target_frame_, source_frame_, time,ros::Duration(1.0));
    }
    catch (tf2::TransformException &ex)
    {
        ROS_WARN("%s", ex.what());
        return;
    }
    // ROS_INFO("Got the speed!");
    /* 将速度指令从源坐标系转换到目标坐标系 */
    geometry_msgs::Twist robot_cmd_vel;
    tf2::doTransform(*msg, robot_cmd_vel, transformStamped);
    ROS_INFO("The speed is x:[%f] y:[%f] omega:[%f]", robot_cmd_vel.linear.x, robot_cmd_vel.linear.y, robot_cmd_vel.angular.z);
    /* 发布结果 */
    publisher_.publish(robot_cmd_vel);   
}

void v_transfer::spin()
{
    while(ros::ok())
    {
        // ROS_INFO("Yeah");
        ros::spinOnce();
    }
}

}