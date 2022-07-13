/*
    EngineDataLogger
    Copyright (C) 2018-2020  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdarg.h>
#include <csignal>
#include <stdlib.h>

enum e_lvl {NONE, ERROR, WARN, INFO, DEBUG};
extern volatile sig_atomic_t time_to_quit;
extern e_lvl LEVEL_DEBUG;

void error_message(e_lvl err_lvl, char const *fmt, ...);
void signalHandler( int signum );

#endif
