#include "ros/ros.h"
#include "std_msgs/String.h"
#include "string"

std::string key_press = "Stop";
std::string key_press_1 = "Stop";
int check = 0;
void Key_process(const std_msgs::String::ConstPtr& msg){
    key_press_1 = key_press;
    key_press = msg->data.c_str();
    check = 50;
}
int main(int argc, char** argv){
    ros::init(argc, argv,"Process");
    ros::NodeHandle n;
    ros::Subscriber key_cmd = n.subscribe("Teleop_Key",1000,Key_process);
    ros::Publisher cmd_to_serial = n.advertise<std_msgs::String>("write_uart",1);
    ros::Rate loop_rate(100);
    
    while (ros::ok())
    {   
        std_msgs::String msg;
        if(check == 0){
            key_press_1 = key_press;
            key_press = "Stop";
        }
        check = (check > 0) ? check - 1 : check = -1;

        if(key_press_1 != key_press){
            ROS_INFO_STREAM("cmd: " << key_press);
            if(key_press == "Up") msg.data = "%DM!dm1!@";
            if(key_press == "Down") msg.data = "%DM!dm2!@";
            if(key_press == "Left") msg.data = "%DM!dm3!@";
            if(key_press == "Right") msg.data = "%DM!dm4!@";
            if(key_press == "Stop") msg.data = "%DM!dm0!@";
            cmd_to_serial.publish(msg);
            key_press_1 = key_press;
        }
        ros::spinOnce();
        loop_rate.sleep();
        
    }
    
}