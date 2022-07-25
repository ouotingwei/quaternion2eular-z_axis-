/*
        linear unit = centimeter
        velicity unit = m/s
        rotation unit = degrees
        
        to controller:
        [1][2][3]

                [1]
                        1 = X-direction         2 = Y-direction
                [2]
                        velocity( + or - )     
                [3]
                        rotation ( + or - )

        to main_control:
        subscribe geometry_msgs x &y  & quaternion_pose

*/

#include<ros/ros.h>
#include<bits/stdc++.h>
#include<stdlib.h>

#include<geometry_msgs/PoseStamped.h>

//demo
int set_point_a_x = 30;

//error
int move_error = 2;

//robot data
float robot_now_point_x = 0;
float robot_now_point_y = 0;
int robot_now_vel = 0;
int robot_now_pose = 0;
int temp_pose = 0;

//else
float quat_z = 0;
float eular_z = 0;

float qua2eular(float);         //only for z-axis rotation
void SLAM_POSE_Callback(const geometry_msgs::PoseStamped::ConstPtr&msg );
void move_x(int);
void check_attitude(int);

int main(int argc, char* argv[]){
        ros::init(argc, argv,"qua2eular");

        ros::NodeHandle n;

        ros::Subscriber sub = n.subscribe("/slam_out_pose", 10, SLAM_POSE_Callback);

        while(ros::ok){
                robot_now_pose = qua2eular(quat_z);
                //std::cout << "robot_pose =  "<<robot_now_pose<<"           x = "<<robot_now_point_x<<"               y = " <<robot_now_point_y<<std::endl;
                move_x(set_point_a_x);

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

void move_x(int set_point_x ){
        int dis_to_setpoint = set_point_x - robot_now_point_x;
        int error_low =set_point_x - move_error;
        int error_high = set_point_x + move_error; 
        
        if(dis_to_setpoint > 0){
                temp_pose = 180;

        }else{
                temp_pose = 0;
                
        }

        if (robot_now_point_x > error_low && robot_now_point_x < error_high){        //reached
                        //done
                        std::cout << " done  "<<std::endl;
        }else{                                                                                                                                        //not reached
                if(dis_to_setpoint > 0 ){
                        // move forward
                        std::cout << " move forward               ";
                        check_attitude(temp_pose);

                }else{
                        //move backward
                        std::cout << " move backward           ";
                        check_attitude(temp_pose);
                }
        }
}

void move_y(int set_point_y ,int set_pose){
        int dis_to_setpoint = set_point_y - robot_now_point_y;
        int error_low =set_point_y - move_error;
        int error_high = set_point_y + move_error; 

        if(dis_to_setpoint > 0){
                temp_pose = 90;

        }else{
                temp_pose = 270;

        }

        if (robot_now_point_y > error_low && robot_now_point_y < error_high){        //reached
                        //done
                        std::cout << " done  "<<std::endl;

        }else{                                                                                                                                        //not reached
                if(dis_to_setpoint > 0 ){
                        // move right
                        std::cout << " move right                ";
                        check_attitude(temp_pose);

                }else{
                        //move left
                        std::cout << " move left                    ";
                        check_attitude(temp_pose);

                }
        }
}

void check_attitude(int temp_pose){
        int pose_error = robot_now_pose - temp_pose;

        if(pose_error > 0){
                //turn left
                std::cout << " turn left "<<std::endl;

        }else if(pose_error < 0){
                //turn right
                std::cout << " turn_right"<<std::endl;

        }else{
                //do nothing
                std::cout<<" "<<std::endl;

        }
}



