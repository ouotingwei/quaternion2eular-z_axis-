#include<ros/ros.h>
#include<geometry_msgs/PoseStamped.h>
#include<bits/stdc++.h>
#include<stdlib.h>

// cm
#define robot_vel_max = 10;
#define robot_vel_max_distence = 50;
#define robot_vel_med =5;
#define robot_vel_max_distence = 20;
#define robot_vel_min = 1;
#define robot_vel_max_distence = 10;

#define mission_point_a_x = 1;
#define mission_point_a_y = 1;
#define mission_pose_a = 270;

int move_error = 2;
float quat_z = 0;
float robot_now_point_x = 0;
float robot_now_point_y = 0;
int robot_now_vel = 0;
float eular_z = 0;
int robot_pose = 0;
bool done_flag = false;

float qua2eular(float);         //only for z-axis rotation
void SLAM_POSE_Callback(const geometry_msgs::PoseStamped::ConstPtr&msg );
void move_vel(int , int);


int main(int argc, char* argv[]){
        ros::init(argc, argv,"qua2eular");

        ros::NodeHandle n;

        ros::Subscriber sub = n.subscribe("/slam_out_pose", 10, SLAM_POSE_Callback);

        while(ros::ok){
                 robot_pose = qua2eular(quat_z);
                 std::cout << "robot_pose =  "<<robot_pose<<"           x = "<<robot_now_point_x<<"               y = " <<robot_now_point_y<<std::endl;

                 ros::spinOnce();
        }

        return 0;
}

void SLAM_POSE_Callback(const geometry_msgs::PoseStamped::ConstPtr&msg){
        quat_z = msg -> pose.orientation.z;
        robot_now_point_x = msg -> pose.position.x;
        robot_now_point_x = robot_now_point_x*100;
        robot_now_point_y = msg -> pose.position.y;
        robot_now_point_y = robot_now_point_y*100;

}

float qua2eular(float quat_z){
        int pose = (2*acos(quat_z))*180/3.1415;

        return pose;
}

int move_vel(int robot_now_point , int mission_point, bool done_flag ){
        int dis_to_setpoint = mission_point - robot_now_point;
        int error_low = mission_point - move_error;
        int error_high = mission_point + move_error; 

        if 

        return  robot_now_vel;
}



