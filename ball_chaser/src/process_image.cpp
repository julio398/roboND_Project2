//Student name: Julio Aguilar feb 25th 2019, Robotics Software Engineering - Nano Degree UDACITY

#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <iostream>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    
    ROS_INFO_STREAM("Passing velocities to drive the robot");

    // Request direction [lin_x, ang_z]

        ball_chaser::DriveToTarget srv;
        srv.request.linear_x = lin_x;
        srv.request.angular_z = ang_z;

    if (!client.call(srv)){
        ROS_ERROR("Failed to call service command_robot");
	}
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
      
    // Loop through each pixel in the image and check if its equal to 255 (white_pixel)
 	 
    for (int i = 0; i < img.height * img.step; i++) {
	//White ball found
        if (img.data[i] == white_pixel ) {
	  
            int current_pixel = i;	
	    int row_number = current_pixel / 2400;
            int pixels_bellow = row_number * 2400;
            int current_position = current_pixel - pixels_bellow; //current position in a row
	   
           //if pixel in the left, turn slightly left and reduce speed
	   if (current_position <= 800){
	       drive_robot(0.3,0.3);
	       sleep(1);
	       break;
            }
	   //if pixel in the middle, drive forward
	   if ((current_position > 800) && (current_position <= 1600)){
	       drive_robot(0.5, 0.0);
	       sleep(1);
	       break;
	   }
           //if pixel in the right, turn slightly right and reduce speed
	   if (current_position > 1600){
	       drive_robot(0.3,-0.3);
	       sleep(1);
	       break;
	   }	
	}
    }
    //stop if there is no white ball
    drive_robot(0.0,0.0);
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;
    
    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
