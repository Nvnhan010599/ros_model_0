#include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <dynamic_tut/My_DynamicConfig.h>

void callback(dynamic_tut::My_DynamicConfig &config, uint32_t level) {
  ROS_INFO("Reconfigure Request: %s %f %f %f %s", 
            config.Mode_Auto?"True":"False",
            config.Kp1, 
            config.Ki1,
            config.Kd1,
            config.To_STM.c_str());
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "Car");

  dynamic_reconfigure::Server<dynamic_tut::My_DynamicConfig> server;
  dynamic_reconfigure::Server<dynamic_tut::My_DynamicConfig>::CallbackType f;

  f = boost::bind(&callback, _1, _2);
  server.setCallback(f);

  ROS_INFO("Spinning node");
  ros::spin();
  return 0;
}