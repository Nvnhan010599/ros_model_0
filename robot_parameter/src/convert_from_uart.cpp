#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/String.h>
#include <string>
#include <cstring>
#include <vector>

std::vector<std::string> command_string(std::string str, char delim){
    int n = str.size();
    int a[n];
    int num_delim = 0;
    std::vector<std::string> res;
    for(int i = 0; i < n; i++){
        if(str[i] == delim){
            a[num_delim] = i;
            num_delim++;
        }
    }

    char * kq;

    for(int i = 1; i < num_delim; i++){
        int begin = a[i - 1] + 1;
        int n_point = a[i] - a[i-1] - 1;
        kq = (char*)malloc((n_point+1)*sizeof(char));
        memset(kq,'\0', n_point + 1);
        std:: string tmp = str.substr(begin,n_point);
        res.push_back(tmp);
        }
    free(kq);
    return res;
}

void Convert_callback(const std_msgs::String::ConstPtr & msg, ros::Publisher *pub){
    std::string input = msg->data;
    if((input.substr(0,4) == "#STM" ) && (input[input.size() - 1] = '@')){
        std::vector<std::string> res = command_string(msg->data,'!');
        geometry_msgs::Pose p;
        p.position.x = atof(&(res[2])[2]);
        p.position.y = atof(&(res[3])[2]);
        p.position.z = 0;
        p.orientation.z = atof(&(res[4])[2]);

        pub->publish(p);
    }
    
}

int main(int argc, char** argv){

    ros::init(argc, argv, "convert_from_uart");
    ros::NodeHandle n;
    ros::Publisher  convert_pub = n.advertise<geometry_msgs::Pose>("Robot_Pos",1);
    ros::Subscriber uart_sub = n.subscribe<std_msgs::String>
    ("read_uart",1,boost::bind(&Convert_callback,_1,&convert_pub));
    ros::spin();
    return 0;
}