/* ***********************************************
Author        :guanjun  guanjunace@foxmail.com
Created Time  :2017/7/8 10:40:13
File Name     :camera_publisher.cpp
************************************************ */
#include <bits/stdc++.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class WebCam {
 public:
    WebCam (ros::NodeHandle& nh, int video_source = 0)
	: it(nh), 
	cap(video_source) {
		if (!cap.isOpened()) {
			ROS_ERROR("Cannot open the camera!\n");
		}
		/*设置主题名和缓冲区*/   
        imgPub = it.advertise("webcam/img", 1);
        /*初始化CvImage智能指针，CvImage为Mat与ROS图像之间转换的载体*/
        frame = boost::make_shared<cv_bridge::CvImage>();
        /*设置ROS图片为BGR且每个像素点用1个字节来表示类似于CV_8U*/
        frame->encoding = sensor_msgs::image_encodings::BGR8;
    }
    /*图像发布函数*/
    int publishImage(){
        /*将摄像头获取到的图像存放在frame中的image*/
        cap >> frame->image;
        /*判断是否获取到图像，若获取到图像，将其转化为ROS图片*/
        if (!(frame->image.empty())){
            frame->header.stamp = ros::Time::now();
            imgPub.publish(frame->toImageMsg());
        }
        return 0;       
    }
 private:    
    /*设置图片节点*/
    image_transport::ImageTransport it;
    /*设置图片的发布者*/
    image_transport::Publisher imgPub;
    /*设置存放摄像头图像的变量*/    
    VideoCapture cap;
    /*设置cvImage的智能指针*/
    cv_bridge::CvImagePtr frame;
};

int main(int argc, char *argv[]) {
    /*初始化节点，并设定节点名*/
    ros::init(argc, argv, "img_publiser");
    /*设置节点句柄*/
    ros::NodeHandle nh;

    /*判断输入参数是否完成*/
    if (argv[1] == NULL){
        ROS_WARN("Please choose the camera you want to use !");
        return 1;
    }
    /*获取打开摄像机的设备号*/
    int video_source = 0;
    int default_p = 0;
    istringstream default_param(argv[1]);
    default_param >> default_p;
    nh.param<int>("video_source", video_source, default_p);

    /*定义摄像机对象*/
    WebCam webcam(nh, video_source);

    /*设置主题的发布频率为10Hz*/
    ros::Rate loop_rate(10);
    /*图片节点进行主题的发布*/
    while (ros::ok()) {
        webcam.publishImage();
        ros::spinOnce();
        /*按照设定的频率来将程序挂起*/
        loop_rate.sleep();
    }
    return 0;
}
