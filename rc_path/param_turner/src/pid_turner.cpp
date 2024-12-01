#include "param_turner/pid_turner.h"

PLUGINLIB_EXPORT_CLASS(pid_turner::pid_turner, nodelet::Nodelet)

namespace pid_turner
{

pid_turner:: pid_turner()
{
}

void pid_turner::onInit()
{
    nh_ = getPrivateNodeHandle();
    if(!readParameters())
    {
        ROS_ERROR("Could not load the parameters!");
        /* 在这里就不直接shutdown了，pid_turner不是必需启动项 */
        return;
    }
    
    callbacktype_ = boost::bind(&pid_turner::ServiceCallback,this,_1,_2);
    server_.reset(new dynamic_reconfigure::Server<param_turner::pid_turnerConfig>(nh_));
    server_->setCallback(callbacktype_);

    ROS_INFO("[%s]'s pid_turner is ready!",ns_.c_str());
}

bool pid_turner::readParameters()
{
    if(!nh_.getParam("robot",ns_))
    {
        ROS_ERROR("Couldn't get the param:[robot]");
        return false;
    }
    return true;
}

void pid_turner::ServiceCallback(param_turner::pid_turnerConfig &config,uint32_t level)
{
    nh_.setParam(ns_ + "/Kp",config.K_p);
    nh_.setParam(ns_ + "/Ki",config.K_i);
    nh_.setParam(ns_ + "/Kd",config.K_d);
    nh_.setParam(ns_ + "/ref",config.ref);
    ROS_INFO("Now [%s]'s pid config is Kp:[%f] Ki:[%f] Kd:[%f],ref:[%f]",ns_.c_str(),config.K_p,config.K_i,config.K_d,config.ref);
}

void pid_turner::spin()
{


}
    
}