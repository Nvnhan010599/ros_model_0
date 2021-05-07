#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char** argv){
    ros::init(argc, argv,"Write_to_serial_node");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<std_msgs::String>("write_uart",1);
    ros::Rate loop_rate(1);

    while(ros::ok()){
        std_msgs::String msg;
        msg.data = "%DM!dm0!@";
        ROS_INFO("Messeage OK");
        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}