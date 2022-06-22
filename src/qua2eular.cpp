#include<ros/ros.h>
#include<geometry_msgs/PoseStamped.h>
#include<bits/stdc++.h>

float quat_z = 0;
float eular_z = 0;

void PathCallback(const geometry_msgs::PoseStamped::ConstPtr&msg );
int main(int argc, char* argv[]){
        ros::init(argc, argv,"qua2eular");

        ros::NodeHandle n;

        ros::Subscriber sub = n.subscribe("/slam_out_pose", 1000, PathCallback);

        ros::spin();
        return 0;
}

void PathCallback(const geometry_msgs::PoseStamped::ConstPtr&msg){
        quat_z = msg -> pose.orientation.z;
        eular_z = (2*acos(quat_z))*180/3.1415;
        std::cout << "z = "<<eular_z<<std::endl;
        
}