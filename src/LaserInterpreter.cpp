#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <string>
#include<vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include <sensor_msgs/LaserScan.h>
using namespace std;

class LaserInterpreter 
{
    ros::NodeHandle n;
    ros::Subscriber sub;
    ros::Publisher driver_pub;

private:

    std::vector<float> distances;

public:

    float maxLeft;
    float minLeft;
    float maxFront;
    float minRight;
    float maxRight;

    float edge            = 0.80;
    float scanRange       = 0.80;
    float front           = 0.45;
    float diagonalLenghth = (scanRange-front)*(scanRange-front)+(scanRange-edge)*(scanRange-edge);
    float diagonal        = sqrt(diagonalLenghth);

    float angleFront    = atan((front/2)/scanRange);
    float angleDiagonal = atan((diagonal/2)/scanRange);
    float angleEdge     = atan(edge/scanRange);

    float getSideSize(){
        //represents the array value for which the leftest most pont of the leftEdge is situated
        int maxLeft =512;
        ROS_INFO_STREAM("maxleft " << maxLeft);
        //represents the array value for which the leftest most pont of the leftDiagonal is situated
        float minLeft  = 512*0.80;      //512*((angleEdge/180)+(angleDiagonal/180)+(angleFront/180)+(angleDiagonal/180));
        //ROS_INFO_STREAM(minLeft);
        //represents the array value for which the leftest most pont of the front is situated
        int maxFront = 512*0.60;     //512*((angleEdge/180)+(angleDiagonal/180)+(angleFront/180));
        //ROS_INFO_STREAM(maxFront);
        //represents the array value for which the leftest most pont of the rightDiagonal is situated
        int maxRight = 512*0.40;     //512*((angleEdge/180)+(angleDiagonal/180));
        //ROS_INFO_STREAM(maxRight);
        //represents the array value for which the leftest most pont of the rightEdge is situated
        int minRight = 512*0.20;     //512*(angleEdge/180);
        //ROS_INFO_STREAM(minRight);
        return minRight , maxRight ,minLeft , maxLeft, maxFront ;
    }
    /*
    //represents the array value for which the leftest most pont of the front is situated
    int maxLeft  =512;

    //represents the array value for which the leftest most pont of the front is situated
    int maxFront = 512*0.66;

    //represents the array value for which the leftest most pont of the right is situated
    int maxRight = 512*0.33;
*/
    ros::Publisher getPub(){
        return driver_pub;
    }




    //the distance of picked up at every 0.36 degrees set up in array form
    void getDistances(const sensor_msgs::LaserScan msg)
    {
        distances = msg.ranges;
    }

    //determines the loation of objects around robot
    int scanner(){

        ROS_INFO_STREAM("In scanner");

        //Create boolean for each third of vision
        bool leftEdge      = false;
        bool leftDiagonal  = false;
        bool front         = false;
        bool rightDiagonal = false;
        bool rightEdge     = false;


        /*
  //Create boolean for each third of vision
  bool left = false;
  bool front = false;
  bool right = false;
*/  
        //Set to default state that will not cause robot to move
        int x = 10;

        ROS_INFO("Before IF");
        //If distances vector is not empty analyse laserScan
        if (!distances.empty()){

            ROS_INFO("After IF");
            //represents the array value for which the leftest most pont of the leftEdge is situated
            int maxLeft =512;
            ROS_INFO_STREAM("maxleft " << maxLeft);
            //represents the array value for which the leftest most pont of the leftDiagonal is situated
            float minLeft  = 512*0.80;      //512*((angleEdge/180)+(angleDiagonal/180)+(angleFront/180)+(angleDiagonal/180));
            ROS_INFO_STREAM(minLeft);
            //represents the array value for which the leftest most pont of the front is situated
            int maxFront = 512*0.60;     //512*((angleEdge/180)+(angleDiagonal/180)+(angleFront/180));
            ROS_INFO_STREAM(maxFront);
            //represents the array value for which the leftest most pont of the rightDiagonal is situated
            int maxRight = 512*0.40;     //512*((angleEdge/180)+(angleDiagonal/180));
            ROS_INFO_STREAM(maxRight);
            //represents the array value for which the leftest most pont of the rightEdge is situated
            int minRight = 512*0.20;     //512*(angleEdge/180);
            ROS_INFO_STREAM(minRight);

            for(int i=0; i<maxRight;i++)

                if(0.05 < distances[i] && distances[i] < scanRange){


                    rightEdge = true;}

            for(int i=maxRight; i<minRight;i++){

                if(0.05 < distances[i] && distances[i] < scanRange){


                    rightDiagonal = true;}}

            for(int i=maxRight; i<maxFront;i++){

                if(0.05 < distances[i] && distances[i] < scanRange){


                    front = true;}}

            for(int i=maxFront; i<minLeft;i++){

                if(0.05 < distances[i] && distances[i] < scanRange){

                    ROS_INFO_STREAM("the range in max front was checked");
                    leftDiagonal = true;}}

            for(int i=minLeft; i<maxLeft;i++){

                if(0.05 < distances[i] && distances[i] < scanRange){


                    leftEdge = true;}}


            //determine if an object is situated to the RIGHT
            /*
    float resultRight = 0;
    int totRight = 0;
        for(int i=0; i<maxRight;i++)
          {
            if(0.05 < distances[i] && distances[i] < 6){
                resultRight = resultRight + distances[i];
                totRight++;
                }
          }
          resultRight = resultRight/totRight;
          ROS_INFO_STREAM("result right " << resultRight);
            if(resultRight <= 0.8 && resultRight >= 0.05){

              right = true;}


        //determine if an object is situated to the FRONT

        float resultFront = 0;
         int totFront = 0;

        for(int i=maxRight; i<maxFront;i++)
          {
            if (0.05 < distances[i] && distances[i] < 6){
                resultFront = resultFront + distances[i];
                totFront++;
                }
          }
          resultFront = resultFront/totFront;
          ROS_INFO_STREAM("result front " << resultFront);
            if(resultFront <= 0.8 && resultFront >= 0.05){
              front = true;
          }

        //determine if an object is situated to the LEFT

        float resultLeft = 0;
        int totLeft = 0;

        for(int i=maxFront; i<maxLeft;i++)
          {
            if (0.05 < distances[i] && distances[i] < 6){
                resultLeft = resultLeft + distances[i];
                totLeft++;
                }
          }
          resultLeft = resultLeft/totLeft;
          ROS_INFO_STREAM("result left " << resultLeft);
            if(resultLeft <= 0.8 && resultLeft >= 0.05){
              left = true;
          }

         //determine which situation therobo is in
         if (left == true && front == false && right == true){x = 1;}
         else if (left == true && front == true && right == false){x = 2;}
         else if (left == false && front == true && right == true){x = 3;}
         else if (left == false && front == true && right == false){x = 4;}
         else if (left == true && front == true && right == true){x = 5;}
         else if (left == true && front == false && right == false) x = 6;
         else if (left == false && front == false && right == true) x = 7;
         else x = 8;
            ROS_INFO_STREAM("Sensor results are " << left << front << right);
            ROS_INFO_STREAM("Now entering case " << x);
      */
            //no walls
            if (leftEdge == false && leftDiagonal == false && front == false && rightDiagonal == false && rightEdge == false){x = 1;}

            //wall front left
            else if (leftEdge == true && leftDiagonal == true && front == true && rightDiagonal == false && rightEdge == false || leftEdge == false && leftDiagonal == true &&front == true && rightDiagonal == false && rightEdge == false || leftEdge == true && leftDiagonal == false && front == true && rightDiagonal == false && rightEdge == false ){x = 2;}

            //wall front left  with gap
            else if (leftEdge == true && leftDiagonal == false && front == true && rightDiagonal == true && rightEdge == true||leftEdge == true && leftDiagonal == false && front == true && rightDiagonal == true && rightEdge == false){x = 12;}

            //wall front right
            else if (leftEdge == false && leftDiagonal == false && front == true && rightDiagonal == true && rightEdge == true || leftEdge == false && leftDiagonal == false && front == true && rightDiagonal == true && rightEdge == false ){x = 3;}

            //wall front right  with gap
            else if (leftEdge == false && leftDiagonal == false && front == true && rightDiagonal == false && rightEdge == true || leftEdge == true && leftDiagonal == true && front == true && rightDiagonal == false && rightEdge == true || leftEdge == true && leftDiagonal == false && front == true && rightDiagonal == false && rightEdge == true ||leftEdge == true && leftDiagonal == false && front == true && rightDiagonal == false && rightEdge == true ){x = 13;}

            //wall front
            else if (leftEdge == false && leftDiagonal == false && front == true && rightDiagonal == false && rightEdge == false){x = 4;}

            //deadend
            else if (leftEdge == true && leftDiagonal == true && front == true && rightDiagonal == true && rightEdge == true){x = 5;}

        }
        return x;

    }

    //the robo will decide which action to take base on certain conditions calculated previously
    geometry_msgs::Twist drive(){

        ROS_INFO_STREAM("In drive");
        geometry_msgs::Twist result;

        switch( scanner() ){


        case 1:
            //LR
            result.linear.x=0.5;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=0;
            break;

        case 2:
            //LF
            result.linear.x=0.1;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=-0.5;
            break;

        case 12:
            //LF with gap
            result.linear.x=0.15;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=-0.25;
            break;

        case 3:
            //FR
            result.linear.x=0.1;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=0.5;
            break;

        case 13:
            //FR with gap
            result.linear.x=0.15;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=0.25;
            break;

        case 4:
            //F
            result.linear.x=0;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=0.5;
            break;

        case 5:
            // LFR
            result.linear.x=0;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=-0.4;
            break;
            /*
                    case 6:
                        // L
            result.linear.x=0.3;
                        result.linear.y=0;
                        result.linear.z=0;

                        result.angular.x=0;
                        result.angular.y=0;
                        result.angular.z=-0.2;
                        break;

                    case 7:
                        // R
            result.linear.x=0.3;
                        result.linear.y=0;
                        result.linear.z=0;

                        result.angular.x=0;
                        result.angular.y=0;
                        result.angular.z=0.2;
                        break;

                    case 8:
                        //All clear
            result.linear.x=0.3;
                        result.linear.y=0;
                        result.linear.z=0;

                        result.angular.x=0;
                        result.angular.y=0;
                        result.angular.z=0;
                        break;

                    case 10:
                        //If no case stand still and cry

                        result.linear.x=0;
                        result.linear.y=0;
                        result.linear.z=0;

                        result.angular.x=0;
                        result.angular.y=0;
                        result.angular.z=0;
                        break;
 */
        default:
            //If no case stand still and cry

            result.linear.x=0.1;
            result.linear.y=0;
            result.linear.z=0;

            result.angular.x=0;
            result.angular.y=0;
            result.angular.z=0;
            break;

        }

        return result;
    }


    //constructor
    LaserInterpreter(){

        sub = n.subscribe("/scan", 1, &LaserInterpreter::getDistances,this);
        driver_pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);
    }

    //destructor
    ~LaserInterpreter(){
        cout << "laser Interpreter has been destroyed" <<endl;
    }

};  


//Main method
int main(int argc, char **argv)
{
    ros::init(argc, argv, "LaserInterpreter");



    LaserInterpreter li;
    ros::Rate loop_rate(10);

    while (ros::ok())
    {


        //Publish geometry_msg based on drive method to motor control
        li.getPub().publish(li.drive());


        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
