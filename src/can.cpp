/*
    EngineDataLogger
    Copyright (C) 2018-2021  Jacob Geigle

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
#include "can.h"

bool can_sock_connect( int can_s, char const *can_arg ) {
	struct sockaddr_can addr;
	struct ifreq ifr;
	strcpy(ifr.ifr_name, can_arg);
	if ( ioctl(can_s, SIOCGIFINDEX, &ifr) < 0 ) {
		error_message(WARN,"Warning:CAN: ioctl failed : %s, Retrying...",strerror(errno));
		return false;
	}

	else {
		addr.can_family = AF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;
		if ( bind(can_s, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
			error_message(WARN,"Warning:CAN: bind failed : %s, Retrying...",strerror(errno));
			return false;
		}

		else {
			return true;
		}
	}
}

