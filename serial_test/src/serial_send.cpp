#include "ros/ros.h"
#include "serial/serial.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"

serial::Serial ser;


void write_callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO_STREAM("Writing to serial port"<< msg->data);
    ser.write(msg->data);
}

int main(int argc, char ** argv){
    ros::init(argc, argv, "serial_node_general");
    ros::NodeHandle n;


    ros::Subscriber write_pub = n.subscribe("write_uart",1,write_callback);
    ros::Publisher read_pub = n.advertise<std_msgs::String>("read_uart",1);

    try{
        ser.setPort("/dev/ttyACM0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch(serial::IOException& e){
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else{
        return -1;
    }
    ros::Rate loop_rate(5);
    while(ros::ok()){
        ros::spinOnce();
        if(ser.available()){
            ROS_INFO_STREAM("Reading from serial port");
            std_msgs::String result;
            result.data = ser.read(ser.available());
            ROS_INFO_STREAM("Read: "<< result.data);
            read_pub.publish(result);
        }
        loop_rate.sleep();
    }
}