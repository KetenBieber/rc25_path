#include "move_control/sim_bringup.h"

PLUGINLIB_EXPORT_CLASS(sim_bringup::sim_bringup, nodelet::Nodelet)


namespace sim_bringup
{

sim_bringup::sim_bringup()
{

}

void sim_bringup::onInit()
{
    nh_ = getNodeHandle();
    pub_1 = nh_.advertise<rc_msgs::ChassisCmd>("robot1/chassis_controller/cmd_chassis",100,true);
    pub_2 = nh_.advertise<rc_msgs::ChassisCmd>("robot2/chassis_controller/cmd_chassis",100,true);
    pub_3 = nh_.advertise<rc_msgs::ChassisCmd>("robot3/chassis_controller/cmd_chassis",100,true);
    pub_4 = nh_.advertise<rc_msgs::ChassisCmd>("robot4/chassis_controller/cmd_chassis",100,true);

    /* 设置加速度 */
    rc_msgs::ChassisCmd cmd_1,cmd_2,cmd_3,cmd_4;
    cmd_1.accel.linear.x = 5.0;
    cmd_1.accel.linear.y = 5.0;
    cmd_1.accel.angular.z = 9.42;

    cmd_2.accel.linear.x = 5.0;
    cmd_2.accel.linear.y = 5.0;
    cmd_2.accel.angular.z = 9.42;

    cmd_3.accel.linear.x = 5.0;
    cmd_3.accel.linear.y = 5.0;
    cmd_3.accel.angular.z = 9.42;

    cmd_4.accel.linear.x = 5.0;
    cmd_4.accel.linear.y = 5.0;
    cmd_4.accel.angular.z = 9.42;

    pub_1.publish(cmd_1);
    ROS_INFO("robot1 accel published!");
    pub_2.publish(cmd_2);
    ROS_INFO("robot2 accel published!");
    pub_3.publish(cmd_3);
    ROS_INFO("robot3 accel published!");
    pub_4.publish(cmd_4);
    ROS_INFO("robot4 accel published!");


}

}