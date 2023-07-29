/**
 * @file logfile.h
 * @brief Header file for log file functions
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
#ifdef __cplusplus
extern "C" {
#endif

#ifndef LOGFILE_H
#define LOGFILE_H
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "error_handling.h"

char *log_insert_str(char *buf1, const char *path, const char *str);
char *log_insert_time(char *buf1, const char *path, time_t tod);
char *filter_chars(const char *filter, char *str);
char *copyout(char *buf1, const char *path, int pathlen, const char *base,
              const char *ext, int inslen, const char *ins_string);
void prep_path_insert(const char *path, ptrdiff_t *pathlen, const char **base, const char **ext);
void log_open(char const **log_file, FILE **fd_log, char const *filename, _Bool file_date, char const *format);
void log_restart(char const **log_file, FILE **fd_log, char const *filename, _Bool file_date, char const *format);


#endif

#ifdef __cplusplus
}
#endif
