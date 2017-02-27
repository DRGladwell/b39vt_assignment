#include "ros/ros.h"
#include "std_msgs/String.h"

#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#include <sensor_msgs/LaserScan.h>
using namespace std;

class LaserInterpreter 
{
ros::NodeHandle n;
ros::Subscriber sub;
private:

std::vector<float> distances;

public:
// booleans for if a wall exsists 
bool left=false;
bool front=false;
bool right=false;

//may wish to have a varying x in futur so might edit *
double x = 0.50; 

public:

std::vector<float> getVectorOfDistances()
{
  return distances; 
}

//this will give the angle of the front therefore enabling us to see what points are infront of us
 double getAngleFront(){
 
	 double length;
	 double resultFront;
	 
	 length = 0.25/x;
	 resultFront = atan (length)*180/M_PI;
	 return resultFront;
 }
 
 //this will give you the angle for the sides relative the size of the front
 double getAngleSide(){
 
	 double resultFront;
	 double resultSide;
	 
	 resultSide =(240-resultFront)/2;
	 return resultSide;
 }
 
 
//the distance of picked up at every 0.36 degrees set up in array form 
	void getDistances(const sensor_msgs::LaserScan msg)


    {
      
      distances = msg.ranges;
      ROS_WARN_STREAM(distances.size());
      
   
	} 
//this will set the boolean on or off if there is a all
//wallExsistance(){

//} 

//direction chosen{

//case 1
//break;

//case 2


//case 3


//case 4


//case 5


//case 6



//}

 
		  //constructor
		 LaserInterpreter(){
		 
		 sub = n.subscribe("/scan", 1, &LaserInterpreter::getDistances,this);

	
		 }
		
		  //detructor 
		 ~LaserInterpreter(){
		 
		  cout << "laser Interpreter has been destroyed" <<endl;}
		  
		  
		
		
  
  
};  

int main(int argc, char **argv)
{
  ros::init(argc, argv, "LaserInterpreter");


 LaserInterpreter li;
 
 


  ros::spin();

  return 0;
}
