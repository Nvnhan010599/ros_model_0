#include <ros/ros.h>
#include <std_msgs/String.h>
#include <dynamic_reconfigure/server.h>
#include <dynamic_tut/PID2Config.h>

#include <sstream>
void callback(dynamic_tut::PID2Config& config, uint32_t level, ros::Publisher *parameter_pub){
    ROS_INFO("Vref: %d \n Kp = %f, Ki = %f, Kd = %f", 
                config.v_right_ref,
                config.Kp2,
                config.Ki2,
                config.Kd2);
    std::stringstream ss;
    ss << "#P2!Vd" << config.v_right_ref <<"!"<<"Kp"<<config.Kp2<<"!"
    <<"Ki"<<config.Ki2<<"!"<<"Kd"<<config.Kd2<<"!"<<"@";
    std_msgs::String msg;
    msg.data = ss.str();
    parameter_pub->publish(msg);
}

int main(int argc, char** argv ){
    ros::init(argc, argv, "configure_PID2");
    ros::NodeHandle n;
    ros::Publisher parameter_pub = n.advertise<std_msgs::String>("write_uart",1);
    dynamic_reconfigure::Server<dynamic_tut::PID2Config> server;
    dynamic_reconfigure::Server<dynamic_tut::PID2Config>::CallbackType f;
    
    f = boost::bind(&callback,_1,_2,&parameter_pub);
    server.setCallback(f);
    ROS_INFO("Spining node");
    ros::spin();
    return 0;
    
}
