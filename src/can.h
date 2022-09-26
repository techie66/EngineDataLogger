/**
 * @file can.h
 * @brief Header file for CAN parsing
 */

/*
    EngineDataLogger
    Copyright (C) 2018-2022  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef CAN_H
#define CAN_H

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "error_handling.h"
#include "definitions.h"
#include <errno.h>
#include "imu_can.h"
#include "ignitech_can.h"
#include "obd2.h"
#include "gps.h"
#include "fc.h"

// 3rd-Party Libraries

/**
 * \brief   Function to simplify connecting to CAN
 *
 * \details Handles IOCTLs and various other tasks to connect to CAN device by name
 *          Lots of otherwise boilerplate code
 *
 * \note    Contains specific usage details, may not be suitably reusable.
 *
 * \param[in]     can_s     Socket identifier
 * \param[in]     can_arg   CAN interface name. Gets blindly copied to ifreq.ifr_name.
 *
 * \return   True if successful, or FALSE if error.
 */
bool can_sock_connect( int can_s, char const *can_arg);

/**
 * \brief   Function to extract data from CAN frames
 *
 * \details This function takes CAN frames, parses them and puts the data
 *          into the right place.
 *
 * \note    What is a note?
 *
 * \param[in]     frame     A can_frame struct (linux/can.h) that already contains a valid recieved frame.
 * \param[in,out] log_data  A structure to store parsed data into.
 * \param[in]     can_s     Socket file-descriptor for CANSock interface
 *
 * \return        Nothing
 *
 * \retval        NotImplemented
 * \retval        ERR_SUCCESS    The function is successfully executed
 * \retval        ERR_FAILURE    An error occurred
 */
void can_parse(const can_frame &frame, bike_data &log_data, const int can_s);

/**
 * \brief   Handle all OBD2 exchanges
 *
 * \param[in]     frame     A can_frame struct (linux/can.h) that already contains a valid recieved frame.
 * \param[in]     log_data  A structure to store parsed data into.
 * \param[in]     can_s     Socket file-descriptor for CANSock interface
 */
int obd2_process(const can_frame &frame, bike_data &log_data, const int can_s);

#endif
