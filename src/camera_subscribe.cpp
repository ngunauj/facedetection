/* ***********************************************
Author        :guanjunace@foxmail.com
Created Time  :2017年07月10日 星期一 10时43分26秒
File Name     :camera_subscribe.cpp
************************************************ */
#include <bits/stdc++.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void img_Callback(const sensor_msgs::ImageConstPtr& msg) {
	try {
		//cout<<"YES"<<endl;
		imshow("img", cv_bridge::toCvShare(msg, "bgr8")->image);
		waitKey(1);
	} catch (cv_bridge::Exception &e) {
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}
}
int main(int argc, char *argv[]) {
	ros::init(argc, argv, "img_subscribe");
	ros::NodeHandle nh;
	namedWindow("webcamimg");
	startWindowThread();
	
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber img_sub = it.subscribe("/webcam/img", 1, &(img_Callback));

	destroyWindow("webcamimg");
	ros::spin();
	return 0;
}
