#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.angular_z = ang_z;
    srv.request.linear_x = lin_x;
    client.call(srv);


}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel = 255;
    for(int column_number = 1; column_number < img.height;column_number++){
        for(int raw_number = 1; raw_number < img.width;raw_number++){
            if(img.data[column_number * raw_number] == white_pixel)
                if(column_number < img.width/3)
                    drive_robot(0, 0.5);
                else if(img.width/3 <= column_number <= 2*img.width/3)
                    drive_robot(0.5, 0);
                else if(2*img.width/3 < column_number)
                    drive_robot(0, -0.5);
                else 
                    drive_robot(0, 0);
        }
    }
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