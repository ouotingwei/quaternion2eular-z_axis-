#include<ros/ros.h>
#include<geometry_msgs/PoseStamped.h>
#include<bits/stdc++.h>
#include<stdlib.h>

// cm

int  set_point_a_x = 30;

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
void move_x(int , int);


int main(int argc, char* argv[]){
        ros::init(argc, argv,"qua2eular");

        ros::NodeHandle n;

        ros::Subscriber sub = n.subscribe("/slam_out_pose", 10, SLAM_POSE_Callback);

        while(ros::ok){
                 robot_pose = qua2eular(quat_z);
                 //std::cout << "robot_pose =  "<<robot_pose<<"           x = "<<robot_now_point_x<<"               y = " <<robot_now_point_y<<std::endl;
                 move_x(robot_now_point_x, set_point_a_x);

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

void move_x(int robot_now_point_x , int set_point_x ){
        int dis_to_setpoint = set_point_x - robot_now_point_x;
        int error_low =set_point_x - move_error;
        int error_high = set_point_x + move_error; 

        if (robot_now_point_x > error_low && robot_now_point_x < error_high){        //reached
                        //done
                        std::cout << " done  "<<std::endl;
        }else{                                                                                                                                        //not reached
                if(dis_to_setpoint > 0 ){
                        // move forward
                        std::cout << " move forward  "<<std::endl;

                }else{
                        //move backward
                        std::cout << " move backward "<<std::endl;
                }
        }

}



