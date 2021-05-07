#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"


void Sub_CmdVel(const geometry_msgs::Twist::ConstPtr & msg,ros::Publisher *pub ){
    ROS_INFO("Hehe Da= %f, Dw= %f",msg->linear.x, msg->angular.z);
    std_msgs::String msg1;
    msg1.data = "#DM!dm0!@";
    if(msg->linear.x > 0){
        msg1.data = "#DM!dm2!@";
    }else if(msg->linear.x < 0){
        msg1.data = "#DM!dm3!@";
    }else if(msg->angular.z > 0){
        msg1.data = "#DM!dm4!@";
    }else if(msg->angular.z < 0){
        msg1.data = "#DM!dm5!@";
    }
    pub->publish(msg1);
}

int main(int argc, char** argv){
    ros::init(argc, argv, "Get_cmd_vel");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<std_msgs::String>("write_uart",1);
    ros::Subscriber cmd_vel_sub = n.subscribe<geometry_msgs::Twist>
    ("cmd_vel",1,boost::bind(&Sub_CmdVel,_1,&pub));
    
    ros::spin();
    return 0;

}