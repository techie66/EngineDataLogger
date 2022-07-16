/**
 * @file powercalc.h
 * @brief Header file for power calculation functions
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

#ifndef POWERCALC_H
#define POWERCALC_H

#include <sys/time.h>
#include "error_handling.h"
#include "definitions.h"

int trailing_average_power( bike_data &log_data, float weight_kg = 300, int periods = 20 );
int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);

#endif
