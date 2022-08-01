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
#include "powercalc.h"
int trailing_average_power(bike_data &log_data, int periods)
{
  // TODO Calculate power by comparing deltas
  static float last_speed = 0;
  struct timeval currtime;
  gettimeofday(&currtime, NULL);
  static struct timeval last_time = currtime;
  float speed_delta = last_speed - log_data.speed;
  struct timeval time_delta;

  // TODO Calc power by accelerometer
  // power = force * speed
  // force = accel * mass
  int _power = ( ( log_data.acc_forward * log_data.weight ) * log_data.speed ) / WATTS_PER_HPI;


  return _power;
}

/** @Alexey Frunze
	https://stackoverflow.com/questions/15846762/timeval-subtract-explanation
	Shamelessly copied
**/
int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
  struct timeval xx = *x;
  struct timeval yy = *y;
  x = &xx; y = &yy;

  if (x->tv_usec > 999999) {
    x->tv_sec += x->tv_usec / 1000000;
    x->tv_usec %= 1000000;
  }

  if (y->tv_usec > 999999) {
    y->tv_sec += y->tv_usec / 1000000;
    y->tv_usec %= 1000000;
  }

  result->tv_sec = x->tv_sec - y->tv_sec;

  if ((result->tv_usec = x->tv_usec - y->tv_usec) < 0) {
    result->tv_usec += 1000000;
    result->tv_sec--; // borrow
  }

  return result->tv_sec < 0;
}
