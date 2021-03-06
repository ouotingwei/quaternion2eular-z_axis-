/*
        linear unit = centimeter
        velicity unit = m/s
        rotation unit = degrees
        
        to controller:
        [1][2][3]

                [1]direction
                        0 = rotation mode & 1 = X-direction & 2 = Y-direction
                [2]velocity
                        velocity( + or - )     
                [3]rotation
                        0 = do nothing & 1 = cw  & 2 = ccw

        to main_control:
        subscribe geometry_msgs x &y  & quaternion_pose

*/

#include<ros/ros.h>
#include<bits/stdc++.h>
#include<stdlib.h>

#include<geometry_msgs/PoseStamped.h>
#include<std_msgs/Int16MultiArray.h>

//demo
int target[3] = {5, 5, 90};

//error
int point_error = 2;
int rotate_error = 2;

int distence_to_target_max = 50;
int distence_to_target_min = 30;

int set_vel_max = 10;
int set_vel_med = 5;
int set_vel_min = 3; 

//robot data
float robot_now_point_x = 0;
float robot_now_point_y = 0;
int robot_now_vel = 0;
int robot_now_pose = 0;
int temp_pose = 0;
bool mission_flag = false;
bool continue_flag = false;

//else
float quat_z = 0;
float eular_z = 0;
int controller_msg[3];

float qua2eular(float);         //only for z-axis rotation
void SLAM_POSE_Callback(const geometry_msgs::PoseStamped::ConstPtr&msg );
void check_attitude(int);
void move_plan(int ,int, int);
void set_vel(int);

int main(int argc, char* argv[]){
        ros::init(argc, argv,"qua2eular");

        ros::NodeHandle n;

        ros::Subscriber sub = n.subscribe("/slam_out_pose", 10, SLAM_POSE_Callback);
        ros::Publisher pub = n.advertise<std_msgs::Int16MultiArray>("/controller_command", 10);

        while(ros::ok){

                if(continue_flag == false){
                        std_msgs::Int16MultiArray msg;
                        msg.data.clear();

                        move_plan(target[0], target[1], target[2]);
                }
                

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

void check_attitude(int temp_pose){
        int pose_error = robot_now_pose - temp_pose;

        if(pose_error > 0){
                //turn left
                std::cout << " turn_ccw "<<std::endl;
                controller_msg[2] = 0;

        }else if(pose_error < 0){
                //turn right
                std::cout << " turn_cw"<<std::endl;
                controller_msg[2] = 1;

        }else{
                //do nothing
               std::cout<<" go straight"<<std::endl;
               controller_msg[2] = 2;

        }
}

void rotation (int set_pose){
        int deg_to_set_pose = robot_now_pose - set_pose;
        int error_low = set_pose - rotate_error;
        int error_high = set_pose + rotate_error;

        if(deg_to_set_pose > error_low && deg_to_set_pose < error_high){
                //done
                std::cout << " done  "<<std::endl;
                controller_msg[2] = 0;

        }else if(deg_to_set_pose >0 ){
                //turn right
                std::cout << " turn_cw"<<std::endl;
                controller_msg[2] = 1;

        }else{
                //turn left
                std::cout << " turn_ccw  "<<std::endl;
                controller_msg[2] = 2;

        }
}

void move_plan(int set_point_x, int set_point_y, int set_pose){
        int error_low_x =set_point_x - point_error;
        int error_high_x = set_point_x + point_error; 
        int error_low_y =set_point_y - point_error;
        int error_high_y = set_point_y + point_error; 
        int error_low_r = set_pose - rotate_error;
        int error_high_r = set_pose + rotate_error;

        while(mission_flag == false){
                int dis_to_setpoint = set_point_x - robot_now_point_x;
                controller_msg[0] = 1;

                robot_now_pose = qua2eular(quat_z);

                if(dis_to_setpoint > 0){
                        temp_pose = 180;

                }else{
                        temp_pose = 0;

                }

                if (robot_now_point_x > error_low_x && robot_now_point_x < error_high_x){        //reached
                        //done
                        mission_flag = true;

                        set_vel(dis_to_setpoint);

                }else{                                                                                                                                        //not reached
                        if(dis_to_setpoint > 0 ){
                                std::cout << " move forward               ";
                                check_attitude(temp_pose);
                                set_vel(dis_to_setpoint);

                        }else{
                                std::cout << " move backward           ";
                                check_attitude(temp_pose);
                                set_vel(dis_to_setpoint);

                        }
                }

                ros::spinOnce();
        }
        mission_flag = false;

        while(mission_flag == false){
                int dis_to_setpoint = set_point_x - robot_now_point_x;
                controller_msg [0] = 1;

                robot_now_pose = qua2eular(quat_z);
                
                if(dis_to_setpoint > 0){
                        temp_pose = 270;

                }else{
                        temp_pose = 90;

                }

                if (robot_now_point_y > error_low_y && robot_now_point_y < error_high_y){        //reached
                        //done
                        mission_flag = true;
                        set_vel(dis_to_setpoint);

                }else{                                                                                                                                        //not reached
                        if(dis_to_setpoint > 0 ){
                                std::cout << " move right                ";
                                check_attitude(temp_pose);
                                set_vel(dis_to_setpoint);

                        }else{
                                std::cout << " move left                    ";
                                check_attitude(temp_pose);
                                set_vel(dis_to_setpoint);

                        }
                }
        
                ros::spinOnce();
        }

        mission_flag = false;
        
        while(mission_flag == false){
                int dis_to_setpoint = 0;
                int deg_to_set_pose = robot_now_pose - set_pose;
                controller_msg [0] = 0;
                std::cout << "dis_to_setpoint"<<deg_to_set_pose<<std::endl;
                if(deg_to_set_pose > error_low_r && deg_to_set_pose < error_high_r){
                        mission_flag = true;

                }else if(deg_to_set_pose > 0 ){
                        std::cout << " turn_cw"<<std::endl;
                        set_vel(dis_to_setpoint);

                }else{
                        std::cout << " turn_ccw  "<<std::endl;
                        set_vel(dis_to_setpoint);

                }

                ros::spinOnce();
        }

        std::cout << "done"<<std::endl;
        continue_flag = true;
}

void set_vel(int distance_to_target){

        if(distance_to_target > 0){
                if(distance_to_target >= distence_to_target_max){
                        controller_msg[1] = set_vel_max;

                }else if(distance_to_target > distence_to_target_min && distance_to_target < distence_to_target_max){
                        controller_msg[1] = set_vel_med;

                }else{
                        controller_msg[1] = set_vel_min;

                }

        }else if(distance_to_target < 0){
                if(distance_to_target <= -1*distence_to_target_max){
                        controller_msg[1] = -1*set_vel_max;

                }else if(distance_to_target < -1*distence_to_target_min && distance_to_target > -1*distence_to_target_max){
                        controller_msg[1] = -1*set_vel_med;

                }else{
                        controller_msg[1] = -1*set_vel_min;

                }
        }else{
                controller_msg[1] = 0;
                
        }
}

