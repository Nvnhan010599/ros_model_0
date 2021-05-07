#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dynamic_reconfigure/server.h"
#include "dynamic_tut/ModeConfig.h"

void callback(dynamic_tut::ModeConfig& config, uint32_t level, ros::Publisher *parameter_pub){
    ROS_INFO("RUN : %s, Mode : %d",
        config.RUN ? "true":"false", config.Mode);
    int can_run = 0;
    if(config.RUN) can_run = 1;

    std::stringstream ss;
    ss << "#DM!MA"<< config.Mode << "!" << "dm" << config.RUN <<"!@"; 
    std_msgs::String msg;
    msg.data = ss.str();
    parameter_pub->publish(msg);
}

int main(int argc, char** argv){

    ros::init(argc, argv,"configure_Mode");
    ros::NodeHandle n;
    ros::Publisher parameter_pub = n.advertise<std_msgs::String>("write_uart",1);
    dynamic_reconfigure::Server<dynamic_tut::ModeConfig> server;
    dynamic_reconfigure::Server<dynamic_tut::ModeConfig>::CallbackType f;
    
    f = boost::bind(&callback,_1,_2,&parameter_pub);
    server.setCallback(f);
    ROS_INFO("Spining node");
    ros::spin();


    return 0;
}
