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

#include "error_handling.h"

e_lvl LEVEL_DEBUG = DEBUG;
void error_message(e_lvl err_lvl, char const *fmt, ...)
{

  FILE *dest = stderr;

  // Handle error messages
  va_list args;
  va_start(args, fmt);
  if (err_lvl <= LEVEL_DEBUG ) {
    while (*fmt != '\0') {
      if (*fmt == '%') {
        ++fmt;
        if (*fmt == 'd') {
          int i = va_arg(args, int);
          fprintf(dest, "%d", i ) ;
        } else if (*fmt == 'c') {
          int c = va_arg(args, int);
          fprintf(dest, "%c", (char)c ) ;
        } else if (*fmt == 'X') {
          int X = va_arg(args, int);
          fprintf(dest, "%X", X ) ;
        } else if (*fmt == 'f') {
          double d = va_arg(args, double);
          fprintf(dest, "%f", d ) ;
        } else if (*fmt == 's') {
          const char *str = va_arg(args, const char *);
          fprintf(dest, "%s", str );
        }
      } else {
        fprintf(dest, "%c", (char)*fmt);
      }
      ++fmt;
    }
    fprintf(dest, "\n");
  }
  va_end(args);
}

volatile sig_atomic_t time_to_quit = false;
void signalHandler( int signum )
{
  error_message(INFO, "Signal %d received", signum);
  time_to_quit = true;

  // cleanup and close up stuff here
  // terminate program

  //exit(signum);
}

volatile sig_atomic_t restart_log = false;
void hupHandler( int signum )
{
  error_message(INFO, "Signal %d received", signum);
  restart_log = true;
}
