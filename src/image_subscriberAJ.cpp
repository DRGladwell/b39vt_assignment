#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "b39vt_assignment/image_processingAJ.hpp"

class ImageSubscriber
{
	// By default, variables and methods are private
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  	cv_bridge::CvImagePtr cv_ptr;
  

  
  const std::string OPENCV_WINDOW;
  
public:
	// Public variables - it is a good practice to separate the declaration of
	// variables from the declaration/definition of methods
	bool data_valid;


  
public:
	// Public methods
  ImageSubscriber() : it_(nh_), data_valid(false), OPENCV_WINDOW("Image window")
  {
    // Subscribe to input video feed
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, 
      &ImageSubscriber::imageCb, this);


  }

  ~ImageSubscriber()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

	cv::Mat getMat(){
		return cv_ptr->image;
	}

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      data_valid = true;
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    // Update GUI Window - uncomment if you want to visualize the subscribed
    // image...
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);
  }
};

struct templ {
	cv::Mat templmat;
	std::string name;
	double thresh;
} greenman, danger, nosmoke, toxic, fire, nuke, biohaz, redman;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_subscriber");
  ImageSubscriber ic;
  
  greenman.templmat = cv::imread("/home/turtlebot/Pictures/green_helmet.png");
  cv::resize(greenman.templmat, greenman.templmat, cv::Size(200,200));
  greenman.name = "Alive Worker";
  greenman.thresh = 0.7;
  danger.templmat = cv::imread("/home/turtlebot/Pictures/danger.png");
  cv::resize(danger.templmat, danger.templmat, cv::Size(200,200));
  danger.name = "Danger";
  danger.thresh = 0.5;
  nosmoke.templmat = cv::imread("/home/turtlebot/Pictures/nosmoke.jpg");
  cv::resize(nosmoke.templmat, nosmoke.templmat, cv::Size(200,200));
  nosmoke.name = "No Smoking";
  nosmoke.thresh = 0.43;
  toxic.templmat = cv::imread("/home/turtlebot/Pictures/toxic.jpg");
  cv::resize(toxic.templmat, toxic.templmat, cv::Size(200,200));
  toxic.name = "Toxic";
  toxic.thresh = 0.43;
  fire.templmat = cv::imread("/home/turtlebot/Pictures/fire.png");
  cv::resize(fire.templmat, fire.templmat, cv::Size(200,200));
  fire.name = "Fire";
  fire.thresh = 0.55;
  nuke.templmat = cv::imread("/home/turtlebot/Pictures/radioactive.png");
  cv::resize(nuke.templmat, nuke.templmat, cv::Size(200,200));
  nuke.name = "Radioactive";
  nuke.thresh = 0.4;
  biohaz.templmat = cv::imread("/home/turtlebot/Pictures/biohazard.png");
  cv::resize(biohaz.templmat, biohaz.templmat, cv::Size(200,200));
  biohaz.name = "Biohazard";
  biohaz.thresh = 0.45;
  redman.templmat = cv::imread("/home/turtlebot/Pictures/red_helmet.png");
  cv::resize(redman.templmat, redman.templmat, cv::Size(200,200));
  redman.name = "Dead Worker";
  redman.thresh = 0.7;
  
  char* result_window = "Result";
  // Dummy variables - Replace with your own
  cv::Mat img;
  templ templates [8] = {greenman, danger, nosmoke, toxic, fire, nuke, biohaz, redman}; 
  //cv::resize(templ, templ, cv::Size(150,150));
  
  while (ros::ok())
  {
		if (ic.data_valid)
		{		
			
			for (int a = 0; a < 8; a++){
				img = ic.getMat();
				bool result = false;
				templ temptempl = templates[a];
				//ROS_INFO_STREAM("Testing " << temptempl.name.c_str);
			ROS_INFO_STREAM("Checking " << temptempl.name.c_str() << " with threshold of " << temptempl.thresh);
				result = templateMatching(img, temptempl.templmat, temptempl.thresh);
				ROS_INFO_STREAM("result is " << result);			
				if (result == true){
					cv::namedWindow( result_window, CV_WINDOW_AUTOSIZE);
					ROS_INFO("%s", temptempl.name.c_str());
					cv::Mat resultMat;
					temptempl.templmat.copyTo(resultMat);
					cv::imshow( result_window, resultMat);
					cv::waitKey(20);
				}
			} 
		}
  	ros::spinOnce();
  }
  return 0;
}
