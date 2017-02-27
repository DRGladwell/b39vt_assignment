#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "talker");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    geometry_msgs::Twist msg;

    msg.linear.x=0.5;
    msg.linear.y=0;
    msg.linear.z=0;
    
    msg.angular.x=0;
    msg.angular.y=0;
    msg.angular.z=0;
   
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();

  }


  return 0;
}
