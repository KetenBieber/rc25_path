/**
 * @file air_joy.cpp
 * @author Keten (2863861004@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * @attention :
 * @note :写的是一个手柄的控制，但实际应该是直接单个控单个，等待完善
 *        改写思路：通过launch文件传入参数，动态加载多个遥控 已完成！
 *        @todo ：写一个启动文件，将底盘控制器的加速度一次性赋值出去
 *        @todo : 看看能不能绑定固定的usb端口号，省去重复sudo chmod的麻烦 sudo chmod 666 /dev/ttyUSB0
 *        使用手册：
 *             接收机
 * 
 * @versioninfo :
 */
#include "air_joy_control/air_joy.h"

PLUGINLIB_EXPORT_CLASS(air_joy::air_joy_device, nodelet::Nodelet)

namespace air_joy
{

air_joy_device::air_joy_device()
{

}

air_joy_device::~air_joy_device()
{


}

void air_joy_device::onInit()
{
    /* 添加对于多手柄控制的参数载入 */
    nh_ = getPrivateNodeHandle();// 记得使用私有句柄

    if(!readParameters())
    {
        ROS_ERROR("Could not load Parameters!");
        ros::requestShutdown();
        return;
    }
    if(more_ == true)
    {
        /* 设置指定手柄控制的参数 */
        publisher_ = nh_.advertise<geometry_msgs::Twist>(ns_ + publishTopic_,1000);
        subscriber_ = nh_.subscribe(ns_ + subscriberTopic_, sub_queuelength_,&air_joy_device::topicCallback,this);
        getYaw_ = nh_.subscribe(ns_ + "/location_action_odom",1000,&air_joy_device::yawCallback,this);
        ROS_INFO("set the device to [%s]",ns_.c_str());

    }
    else
    {
        publisher_ = nh_.advertise<geometry_msgs::Twist>("robot1/cmd_vel",1000);

        subscriber_ = nh_.subscribe(subscriberTopic_,sub_queuelength_,&air_joy_device::topicCallback,this);
        getYaw_ = nh_.subscribe("robot1/location_action_odom",1000,&air_joy_device::yawCallback,this);// 初始化相当于在这里写了，就是默认配置
        ROS_INFO("only one device will be launched!");
    }

}

bool air_joy_device::readParameters()
{
    if(!nh_.getParam("if_more_air_device",more_))
    {
        ROS_ERROR("Couldn't get the param:[if_more_air_device]");
        return false;        
    }
    if(more_ == true)
    {
        nh_.param<std::string>("ctrl_object",ns_,"/robot1");
        if(!nh_.getParam("/CmdVelData/topic_name",publishTopic_))
        {
            ROS_ERROR("Couldn't get the param:[/CmdVelData/topic_name]");
            return false;
        }
        if(!nh_.getParam("/CmdVelData/queue_size",pub_queuelength_))
        {
            ROS_ERROR("Couldn't get the param:[/CmdVelData/queue_size]");
            return false;
        }
    }
    else
    {
    }
    /* 多个手柄或单个手柄都得导入下面参数 */
    if(!nh_.getParam("/AirJoyData/topic_name",subscriberTopic_))
    {
        ROS_ERROR("Couldn't get the param:[/AirJoyData/topic_name]");
        return false;
    }
    if(!nh_.getParam("/AirJoyData/queue_size",sub_queuelength_))
    {
        ROS_ERROR("Couldn't get the param:[/AirJoyData/queue_size]");
        return false;
    }
    return true;
}


void air_joy_device::topicCallback(const rc_msgs::IbusData::ConstPtr& msg)
{
    if(more_ == true)
    {

    }
    else
    {
        /* 进行手柄数据处理 */
        std::string temp1_topic,temp2_topic;
        /* if sw_a ->up && sw_d ->up */
        if(msg->sw_a==2 && msg->sw_d==2)
        {
            temp2_topic = "robot1/cmd_vel";
            temp1_topic = "/robot1/location_action_odom";
        }
        else if(msg->sw_a==2 && msg->sw_d==1)
        {
            temp2_topic = "robot2/cmd_vel";
            temp1_topic = "/robot2/location_action_odom";
        }
        else if(msg->sw_a==1 && msg->sw_d==2)
        {
            temp2_topic = "robot3/cmd_vel";
            temp1_topic = "/robot3/location_action_odom";
        }
        else if(msg->sw_a==1 && msg->sw_d==1)
        {
            temp2_topic = "robot4/cmd_vel";
            temp1_topic = "/robot4/location_action_odom";
        }
        topicChange(temp1_topic,temp2_topic);
    }

    if(msg->sw_c == 3)
    {
        if(msg->sw_b == 2)
        {
            /* 自动模式 */
        }
        else
        {
            if(msg->sw_b == 1)
            {
                /* 机器人坐标系 */
                robot.linear.x = msg->ch_l_y*4;
                robot.linear.y = -msg->ch_l_x*4;
                robot.angular.z = -msg->ch_r_x*4;
                publisher_.publish(robot);
                return;
            }
            else if(msg->sw_b == 3)
            {
                /* 世界坐标系 */
                world.linear.x = msg->ch_l_y*4;
                world.linear.y = -msg->ch_l_x*4;
                world.angular.z = -msg->ch_r_x*4;
                double COS,SIN;
                COS = cos(yaw);
                SIN = sin(yaw);
                robot.linear.x = world.linear.x * COS + world.linear.y * SIN;
                robot.linear.y = -world.linear.x * SIN + world.linear.y * COS;
                robot.angular.z = world.angular.z;
                publisher_.publish(robot);
                return;
            }
        }
    }

}

void air_joy_device::topicChange(std::string &temp_1 , std::string &temp_2)
{
    if(temp_1 == yawSource_)
    {
        return;
    }
    else
    {
        getYaw_.shutdown();
        yawSource_ = temp_1;
        getYaw_ = nh_.subscribe(yawSource_, 1000,&air_joy_device::yawCallback,this);
        ROS_INFO("ctrl object change to be %s",yawSource_.c_str());
    }
    if(temp_2 == publishTopic_)
    {
        return;
    }
    else
    {
        publisher_.shutdown();
        publishTopic_ = temp_2;
        publisher_ = nh_.advertise<geometry_msgs::Twist>(publishTopic_,1000);
        ROS_INFO("ctrl object change to be %s",publishTopic_.c_str());
    }

}

void air_joy_device::yawCallback(const rc_msgs::ActionData::ConstPtr& msg)
{
    yaw = msg->yaw_angle;
    ROS_INFO("receive data:[%s]::[%f]",ns_.c_str(),msg->yaw_angle);
}


}

