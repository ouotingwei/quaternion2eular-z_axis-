#include<ros/ros.h>
#include<bits/stdc++.h>
#include<stdlib.h>

#include<geometry_msgs/PoseStamped.h>

class move_control
{
        public:
                move_control(ros::NodeHandle* nodehandle); 
        
        private:
                ros::NodeHandle n;
                ros::Subscriber sub = n.subscribe("/slam_out_pose", 10, SLAM_POSE_Callback);

};