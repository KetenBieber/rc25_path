#include "odom_sensor/odom_broadcast.h"

PLUGINLIB_EXPORT_CLASS(odom_sensor::odom_broadcast, nodelet::Nodelet)


namespace odom_sensor
{

odom_broadcast::odom_broadcast(): tf_buffer_(), tf_listener_(tf_buffer_), first_time(false) // 在初始化列表中初始化 tf_listener_
{

}

void odom_broadcast::onInit()
{
    nh_ = getPrivateNodeHandle();
    nh_.param<std::string>("robot",ns_,"/robot1");
    ROS_INFO("%s is ready for Odometry",ns_.c_str());
    nh_.param<std::string>("frame_id",odom_.header.frame_id,"odom");
    ROS_INFO("[%s] are set to be frame_id",odom_.header.frame_id.c_str());
    nh_.param<std::string>("child_frame_id",odom_.child_frame_id,"robot1/location_action");
    ROS_INFO("[%s] are set to be child_frame_id",odom_.child_frame_id.c_str());
    

    /* 初始化发布者 */
    pub_odom_ = nh_.advertise<nav_msgs::Odometry>(ns_ + "/odom", 50);

    timer_ = nh_.createTimer(ros::Duration(0.1),&odom_broadcast::run,this);
}


void odom_broadcast::run(const ros::TimerEvent& event)
{
    getVel();
}

void odom_broadcast::getVel()
{
    // ROS_INFO("Enter!");
    ros::Time time = ros::Time(0);

    try
    {
        current_time = ros::Time::now();
        if(first_time == false)
        {
            /* 第一次先存last */
            last_transformStamped = tf_buffer_.lookupTransform(odom_.header.frame_id, odom_.child_frame_id, time,ros::Duration(1.0));
            last_time = current_time;
            first_time = true;
            return;
        }
        /* 第二次及其以后就进这里 */

        /* 获取变换 */
        // 目标：机器人 源：地图零点 time：当前时间 超时时间：1s
        transformStamped = tf_buffer_.lookupTransform(odom_.header.frame_id, odom_.child_frame_id, time,ros::Duration(1.0));

        /* 计算速度 */
        double dt = (current_time - last_time).toSec();
        odom_.twist.twist.linear.x = (transformStamped.transform.translation.x - last_transformStamped.transform.translation.x) / dt;
        odom_.twist.twist.linear.y = (transformStamped.transform.translation.y - last_transformStamped.transform.translation.y) / dt;

        tf2::Quaternion q1, q2, delta_q;
        tf2::fromMsg(last_transformStamped.transform.rotation, q1);
        tf2::fromMsg(transformStamped.transform.rotation, q2);

        // 计算相对旋转
        delta_q = q2 * q1.inverse();
        delta_q.normalize();

        // 将相对旋转转换为欧拉角
        tf2::Matrix3x3 m(delta_q);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);

        // 计算角速度（仅绕 z 轴）
        // 为了避免角度跳变，可以对 yaw 进行归一化处理
        // 例如，从-π到π
        if(yaw - last_transformStamped.transform.rotation.z > M_PI)
            yaw -= 2 * M_PI;
        else if(yaw - last_transformStamped.transform.rotation.z < -M_PI)
            yaw += 2 * M_PI;

        odom_.twist.twist.angular.z = yaw / dt;

         /* 填充 Odometry 消息 */
        odom_.header.stamp = transformStamped.header.stamp;

         // 设置位置信息
        odom_.pose.pose.position.x = transformStamped.transform.translation.x;
        odom_.pose.pose.position.y = transformStamped.transform.translation.y;
        odom_.pose.pose.position.z = transformStamped.transform.translation.z;
        odom_.pose.pose.orientation = transformStamped.transform.rotation;

         pub_odom_.publish(odom_);
        //  ROS_INFO("pub the odom data!");

        /* 更新 last_transformStamped 和 last_time_ */
        last_transformStamped = transformStamped;
        last_time = current_time;
    }
    catch(tf2::TransformException &ex)
    {
        ROS_WARN("Could not get transform: %s",ex.what());
        return;
    }

}


}