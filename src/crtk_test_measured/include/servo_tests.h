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
 * 
 *
 *
 *  \date Oct 29, 2018
 *  \author Andrew Lewis, Yun-Hsuan Su

 */

#include <crtk_lib_cpp/defines.h>
#include <crtk_lib_cpp/crtk_robot_state.h>
#include <crtk_lib_cpp/crtk_robot.h>


#ifndef _SERVO_TESTS_
#define _SERVO_TESTS_


// This function loops through all the crtk tests
int servo_testing(CRTK_robot*, time_t);

// 1 Motion measured query testing
//    1-1 (measured_js functionality) Move all joints in series manually.
//      Pass: Check that each joint has moved.
//      Pass: Check that each joint velocity has a non-zero value.
//    1-2 (measured_cp functionality) Move tool to the right manually.
//      Pass: Check that the correct axis has been mostly moved in.
int test_1(CRTK_robot *, time_t);


#endif