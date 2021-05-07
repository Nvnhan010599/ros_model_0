#include <ros/ros.h>
#include <std_msgs/String.h>
#include <dynamic_reconfigure/server.h>
#include <dynamic_tut/PID1Config.h>

#include <sstream>
void callback(dynamic_tut::PID1Config& config, uint32_t level, ros::Publisher *parameter_pub){
    ROS_INFO("Vref: %d \n Kp = %f, Ki = %f, Kd = %f", 
                config.v_left_ref,
                config.Kp1,
                config.Ki1,
                config.Kd1);
    std::stringstream ss;
    ss << "#P1!Vd" << config.v_left_ref <<"!"<<"Kp"<<config.Kp1<<"!"
    <<"Ki"<<config.Ki1<<"!"<<"Kd"<<config.Kd1<<"!"<<"@";
    std_msgs::String msg;
    msg.data = ss.str();
    parameter_pub->publish(msg);
}

int main(int argc, char** argv ){
    ros::init(argc, argv, "configure_PID1");
    ros::NodeHandle n;
    ros::Publisher parameter_pub = n.advertise<std_msgs::String>("write_uart",1);
    dynamic_reconfigure::Server<dynamic_tut::PID1Config> server;
    dynamic_reconfigure::Server<dynamic_tut::PID1Config>::CallbackType f;
    
    f = boost::bind(&callback,_1,_2,&parameter_pub);
    server.setCallback(f);
    ROS_INFO("Spining node");
    ros::spin();
    return 0;
    
}
