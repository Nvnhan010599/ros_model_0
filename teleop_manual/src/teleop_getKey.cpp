#include "ros/ros.h"
#include "std_msgs/String.h"
#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <iostream>

#define KEYCODE_RIGHT 0x43
#define KEYCODE_LEFT 0x44
#define KEYCODE_UP 0x41
#define KEYCODE_DOWN 0x42
#define KEYCODE_B 0x62
#define KEYCODE_C 0x63
#define KEYCODE_D 0x64
#define KEYCODE_E 0x65
#define KEYCODE_F 0x66
#define KEYCODE_G 0x67
#define KEYCODE_Q 0x71
#define KEYCODE_R 0x72
#define KEYCODE_T 0x74
#define KEYCODE_V 0x76
void keyLoop(int argc, char **argv);

class KeyboardReader
{
public:
    KeyboardReader() : kfd(0)
    {
        // get the console in raw mode
        tcgetattr(kfd, &cooked);
        struct termios raw;
        memcpy(&raw, &cooked, sizeof(struct termios));
        raw.c_lflag &= ~(ICANON | ECHO);
        // Setting a new line, then end of file
        raw.c_cc[VEOL] = 1;
        raw.c_cc[VEOF] = 2;
        tcsetattr(kfd, TCSANOW, &raw);
    }
    void readOne(char *c)
    {
        int rc = read(kfd, c, 1);
        if (rc < 0)
        {
            throw std::runtime_error("read failed");
        }
    }
    void shutdown()
    {
        tcsetattr(kfd, TCSANOW, &cooked);
    }

private:
    int kfd;
    struct termios cooked;
};

KeyboardReader input;

void quit(int sig)
{
    (void)sig;
    input.shutdown();
    ros::shutdown();
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, quit);
    keyLoop(argc, argv);
    quit(0);
    return (0);
}

void keyLoop(int argc, char **argv)
{

    ros::init(argc,argv,"KeyBoard_press");
    ros::NodeHandle n;
    ros::Publisher keyPress = n.advertise<std_msgs::String>("Teleop_Key",1);
    // ros::Rate loop_rate(10);


    char c;
    bool clear = false;

    puts("Reading from keyboard");
    puts("---------------------------");
    puts("Use arrow keys to move, SPACE to stop . 'q' to quit.");
    puts("                  ^                  ");
    puts("               <  +  >               ");
    puts("                  v                  ");
    puts("-------------------------------------");

    while(ros::ok()){
        // get the next event from the keyboard
        std_msgs::String msg;
        try{
            input.readOne(&c);
        }
        catch (const std::runtime_error &){
            perror("read():");
            return;
        }
        //msg.data = "Stop";

        switch (c){
        case KEYCODE_LEFT:
            msg.data = "Left";
            clear = true;
            break;
        case KEYCODE_RIGHT:
            msg.data = "Right";
            clear = true;
            break;
        case KEYCODE_UP:
            msg.data = "Up";
            clear = true;
            break;
        case KEYCODE_DOWN:
            msg.data = "Down";
            clear = true;
            break;
        case KEYCODE_Q:
            ROS_INFO("Quit");
            msg.data = "Quit";
            clear = true;
            return;
        case ' ':
            msg.data = "Stop";
            clear = true;
            break;
        default:
            msg.data = "Stop";
            //clear = true;
            break;
        }
        if(clear){
            keyPress.publish(msg);
            clear = false;
        }
        
        // ros::spinOnce();
        // loop_rate.sleep();
    }

    return;
}