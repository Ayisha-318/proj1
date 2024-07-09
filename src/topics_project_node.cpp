#include "geometry_msgs/Twist.h"
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include <iostream>
#include <array>
#include <vector>

using namespace std;

float front_laser;
float left_laser;
float right_laser;

void laser_Callback(const sensor_msgs::LaserScan::ConstPtr& msg) {
    front_laser = msg->ranges[0];
    left_laser = msg->ranges[90];
    right_laser = msg->ranges[270];

    std::cout << front_laser << " " << left_laser << " " << right_laser << std::endl;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "topics_quiz_node");
    ros::NodeHandle n;

    geometry_msgs::Twist vel;

    ros::Subscriber sub = n.subscribe("/scan", 50, laser_Callback);
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 50);

    ros::Rate rate(10);

    while (ros::ok()) {
        if (front_laser > 1.0 && right_laser > 1.0 && left_laser > 1.0) {
            vel.linear.x = 0.2;
            vel.angular.z = 0.0;
            ROS_INFO("move forward: [%f]", front_laser);
        } 
        
        if (front_laser < 1.0 || right_laser <= 1.0) {
            vel.linear.x = 0.0;
            vel.angular.z = 0.2;
            ROS_INFO("turn left: [%f]", right_laser);
        }

        if (left_laser <= 1.0) {
            vel.linear.x = 0.0;
            vel.angular.z = -0.2;
            ROS_INFO("turn right: [%f]", left_laser);
        }

        vel_pub.publish(vel);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
