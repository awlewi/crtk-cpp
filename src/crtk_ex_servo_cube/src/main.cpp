/* Raven 2 Control - Control software for the Raven II robot
 * Copyright (C) 2005-2018  Andrew Lewis, Yun-Hsuan Su, Blake Hannaford, 
 * and the University of Washington BioRobotics Laboratory
 *
 * This file is part of Raven 2 Control.
 *
 * Raven 2 Control is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Raven 2 Control is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Raven 2 Control.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * crtk_state.cpp
 *
 * \brief Class file for CRTK API state and status flags
 *
 *
 * \date Oct 18, 2018
 * \author Andrew Lewis
 * \author Melody Yun-Hsuan Su
 *
 */

#ifndef MAIN_
#define MAIN_


#include <crtk_lib_cpp/defines.h>
#include <crtk_lib_cpp/crtk_robot_state.h>
#include <crtk_lib_cpp/crtk_robot.h>
#include <crtk_lib_cpp/crtk_motion.h>
#include <crtk_msgs/operating_state.h>
#include <crtk_msgs/StringStamped.h>
#include <sstream>
#include <ctime>
#include <iostream>
#include <string>
#include <ros/ros.h>


#include "main.h"



using namespace std;

tf::Vector3 vec_x(1,0,0);
tf::Vector3 vec_y(0,1,0);
tf::Vector3 vec_z(0,0,1);


/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */

int main(int argc, char **argv)
{

  time_t current_time;
  ros::init(argc, argv, "crtk_test_servo_all");
  static ros::NodeHandle n; 
  ros::Rate loop_rate(LOOP_RATE); 

  CRTK_robot robot(n);

  int count = 0;

  ROS_INFO("Please launch stand alone roscore.");
  while (ros::ok()){
    current_time = time(NULL);
    run_cube(&robot, current_time);
    robot.run();
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }
  return 0;
}



// 3
// 2-2 Relative (command: servo_cr) Cube tracing Test
// (functionality) Trace a cube
//    Pass: Ask user!

int run_cube(CRTK_robot *robot, time_t current_time){
  static int current_step = 1;
  static time_t pause_start;
  int out = 0, out1 = 0, out2 = 0;
  std::string start, s1,s2;

  float dist = 0.01; // 10 mm total
  int duration = 1;
  float completion_percentage_thres = 0.85;  // 0.95;
  static tf::Transform start_pos1, start_pos2;

  static char curr_vertex0 = 0b110; //start arm 0 in front left upper
  static char curr_vertex1 = 0b110; //start arm 1 in front left upper
  static tf::Vector3 move_vec0, move_vec1;
  static CRTK_axis prev_axis0 = CRTK_Z;
  static CRTK_axis prev_axis1 = CRTK_Z; 

  static char edge_count = 0;

  switch(current_step)
  {
    case 1:
    {
      ROS_INFO("======================= Starting servo_cr cube ======================= ");
      ROS_INFO("Start and home robot if not already.");
      ROS_INFO("(Press 'Enter' when done.)"); 
      ROS_INFO("In this example, the arms should randomly trace a cube. Forever \n");
      ROS_INFO("And ever...\n \n");
      ROS_INFO("and ever.");
      current_step ++;
      break;
    }
    case 2:
    {
      // (2) wait for 'Enter' key press
      getline(std::cin,start);
      if(start == ""){
        current_step ++;
      }
      break;
    }
    case 3:
    {
      // (3) send resume command to enable robot
      ROS_INFO("CRTK_RESUME command sent.");
      ROS_INFO("Waiting for robot to enter CRTK_ENABLED state..."); 
      CRTK_robot_command command = CRTK_RESUME;
      robot->state.crtk_command_pb(command); 
      robot->arm[0].start_motion(current_time);
      current_step++;
      break;
    }
    case 4:
    {
      // (4) send motion command to move left robot arm down (for 2 secs)
      if (robot->state.get_enabled()){
        out = robot->arm[0].send_servo_cr_time(-vec_z,dist,duration,current_time);
        if(out) current_step++;
        if(out) robot->arm[1].start_motion(current_time);
      }
      break;
    }
    case 5:
    {
      // (5) send motion command to move right robot arm down (for 2 secs)
      if (robot->state.get_enabled()){
        out = robot->arm[1].send_servo_cr_time(-vec_z,dist,duration,current_time);
        if(out) current_step++;
      }
      break;
    }
    case 6:
    {
      // (6) record start pos
      ROS_INFO("Start randomly tracing a cube.");
      current_step ++;
      break;
    }
    case 7:
    {
      rand_cube_dir(&curr_vertex0, &move_vec0, &prev_axis0);
      rand_cube_dir(&curr_vertex1, &move_vec1, &prev_axis1);
      robot->arm[0].start_motion(current_time);
      robot->arm[1].start_motion(current_time);

      edge_count++;
      current_step++;
      break;
    }
    case 8:
    {
      out1 = 0;
      out2 = 0;
      if(!out1) out1 = robot->arm[0].send_servo_cr_time(move_vec0,dist,duration,current_time);
      else             robot->arm[0].send_servo_cr(tf::Transform());
      if(!out2) out2 = robot->arm[1].send_servo_cr_time(move_vec1,dist,duration,current_time);
      else             robot->arm[1].send_servo_cr(tf::Transform());

      if(out1 && out2) current_step = 7;
      
      break;
    }
  }
  if(out < 0) return out;
  return 0;
}


//todo, check prev dir
char rand_cube_dir(char *curr_vertex, tf::Vector3 *move_vec, CRTK_axis *prev_axis){
  char choice = *prev_axis;
  while ((CRTK_axis)choice == *prev_axis){
    choice = std::rand() % 3; //random int 0-2
  }

  ROS_INFO("\t \t Randomly Picked %i", choice);

  switch((cube_dir)choice){
    case (cube_x):
    {
      ROS_INFO("Picked X!, %i", *curr_vertex);
      *prev_axis = CRTK_X ;

      if(*curr_vertex & front_face){
        *move_vec = tf::Vector3(1,0,0);
        *curr_vertex &= ~front_face;

      } else {
        *move_vec = tf::Vector3(-1,0,0);
        *curr_vertex |= front_face;
      }
      break;
    }    
    case (cube_y):
    {
      ROS_INFO("Picked Y!, %i", *curr_vertex);
      *prev_axis = CRTK_Y ;

      if(*curr_vertex & left_face){
        *move_vec = tf::Vector3(0,1,0);
        *curr_vertex &= ~left_face;

      } else {
        *move_vec = tf::Vector3(0,-1,0);
        *curr_vertex |= left_face;
      }
      break;
    }    
    case (cube_z):
    {
      ROS_INFO("Picked Z!, %i", *curr_vertex);
      *prev_axis = CRTK_Z;

      if(*curr_vertex & lower_face){
        *move_vec = tf::Vector3(0,0,1);
        *curr_vertex &= ~lower_face;

      } else {
        *move_vec = tf::Vector3(0,0,-1);
        *curr_vertex |= lower_face;
      }
      break;
    }
    default:
    {
      ROS_ERROR("unknown cube dir");
      break;
    }
  }
  return 1;
}


#endif
