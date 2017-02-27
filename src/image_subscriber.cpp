#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace std;

//#include "b39vt_assignment/image_processing.hpp"

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
  ImageSubscriber() : it_(nh_), data_valid(false)//, OPENCV_WINDOW("Image window")
  
  {
  	//cout << "created " << endl;
    // Subscribe to input video feed
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, 
      &ImageSubscriber::imageCb, this);

    //cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageSubscriber()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
  	//cout << "called " << endl;
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
    //cv::waitKey(3);
  }
  
  cv::Mat getImage()
  {
 	  return  cv_ptr->image;
  }
};
 double average = 1;
int numOfSpins = 1; 
//double updateAverage()
//{average == (getMinVal + average)/numOfSpins;}

int updateNumOfSpins()
{numOfSpins == numOfSpins++;}

int main(int argc, char** argv)
{
void templateMatching(const cv::Mat& im, const cv::Mat& templ,const std::vector<std::string> templates,int y,int x,int numOfSpins,double average);

  //generaion of values for terminal promot to help understand the output
  //int y = 4;
  


  ros::init(argc, argv, "image_subscriber");
  ImageSubscriber ic;
  
  // Dummy variables - Replace with your own
  cv::Mat templ;
  
  //array listing diffrent templates
	std::vector<std::string> templates = {"danger.png" , "fire.png" ,"radioactive.png" , "smoking.png" , "biohazard.png","toxic.png" , "green_helmet.png", "red_helmet.png"} ;
	
	
  
  while (ros::ok())
  {
		 if (ic.data_valid)
			{
			
			for (int y=0;y<=7;y++){
			//address name for current template x
	    std::string address = "/home/turtlebot/Pictures/Webcam/"+templates[y];
	    templ = cv::imread(address, 1 );
	   
				//resize template so matchs can be made a diffrent ranges
				for (int x=350;x>=140;x=x-60){
							cv::resize(templ,templ, cv::Size(x,x));
				      templateMatching(ic.getImage(), templ,  templates,y,x,numOfSpins, average);
				      //cout <<"the range is: " << x << endl;
				   
				}                        
		  
		    
						
					}
		 }   
		 ros::spinOnce();
		}  
  
  return 0;
}
 
