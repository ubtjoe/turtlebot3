/*******************************************************************************
* Copyright 2019 UBTECH Robotics NARDC
*
*  DO NOT DISTRIBUTE EXTERNALLY
*******************************************************************************/

/* Authors: Joe Dinius Ph.D */

#include <ros/ros.h>
#include <turtlebot3_msgs/SensorState.h>
#include <geometry_msgs/QuaternionStamped.h>

ros::Publisher encoder_pub_;
double enc_rdg_[2];

void msgPub()
{
  geometry_msgs::QuaternionStamped encoder;

  encoder.header.stamp = ros::Time::now();

  /**
   * NOTE(jwd) - this is a hack to conform to current DRE-SLAM encoder message
   */
  encoder.quaternion.x = enc_rdg_[0];
  encoder.quaternion.y = enc_rdg_[0];
  encoder.quaternion.z = enc_rdg_[1];
  encoder.quaternion.w = enc_rdg_[1];

  encoder_pub_.publish(encoder);
}


void sensorStateMsgCallback(const turtlebot3_msgs::SensorState::ConstPtr &msg)
{
  /**
   * just grab the encoder readings 
   */
  enc_rdg_[0] = static_cast<double>(msg->left_encoder);
  enc_rdg_[1] = static_cast<double>(msg->right_encoder);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "encoder_for_dreslam");
  ros::NodeHandle nh;

  encoder_pub_  = nh.advertise<geometry_msgs::QuaternionStamped>("encoder", 15);

  ros::Subscriber tb3_sensor  = nh.subscribe("sensor_state", 15, sensorStateMsgCallback);

  ros::Rate loop_rate(15);

  while (ros::ok())
  {
    msgPub();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
