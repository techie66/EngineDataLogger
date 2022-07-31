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
#define _GNU_SOURCE
#include "logfile.h"
#include "definitions.h"


char *log_insert_time(char *buf1, const char *path,time_t tod)
{
  const char *ext = NULL;
  const char *base = path;
  ptrdiff_t pathlen=0;

  prep_path_insert(path,&pathlen,&base,&ext);

  // Get string for current time
  char stampbuf[TIME_BUF_LEN] = {0};
  if (tod == 0)
        tod = time(NULL);
  if ( tod == -1 ) {
    error_message(ERROR, "ERROR: time() function failed");
    return NULL;
  }
  struct tm *ptm = localtime(&tod);
  if ( ptm == NULL ) {
    error_message(ERROR, "ERROR: localtime() failed");
    return NULL;
  }
  int stamplen = strftime(stampbuf, TIME_BUF_LEN, "-%Y%m%d%H%M%S", ptm);
  // get enough buffer space
  buf1 = copyout(buf1,path,pathlen,base,ext,stamplen,stampbuf);

  return buf1;
}

char *log_insert_str(char *buf1, const char *path,const char* str)
{
  const char *ext = NULL;
  const char *base = path;
  ptrdiff_t pathlen=0;

  prep_path_insert(path,&pathlen,&base,&ext);
  buf1 = copyout(buf1,path,pathlen,base,ext,strlen(str),str);

}

void prep_path_insert(const char *path, int *pathlen, const char **base, const char **ext)
{
  // find "base.ext" and ".ext" in "/dir/base.ext"
  const char *src = path;
  for (int chr = *src++;  chr != 0;  chr = *src++) {
      switch (chr) {
      case '/':
          *base = src;
          *ext = NULL;
          break;
      case '.':
          // extra checks ignore hidden files
          if (*ext == NULL && *(src - 2) != '/' && (src-1) != path)
              *ext = src - 1;
          break;
      }
  }

  // full length of path
  *pathlen = (src - 1) - path;
}

char *filter_chars(const char *filter, char *str)
{
  char *out = str, *put = str;
  if ( str == NULL )
    return NULL;
  if ( filter == NULL )
    return str;

  for(; *str != '\0'; ++str)
  {
    if(!strchr(filter,*str))
      *put++ = *str;
  }
  *put = '\0';
  str = out;
  return out;
}

// copyout -- fill in destination buffer
char *
copyout(char *buf1,const char *path,int pathlen, const char *base,
        const char *ext, int inslen, const char *ins_string)
{
    char *dst;
    ptrdiff_t cpylen;

    // get enough buffer space
    if (buf1 == NULL)
        buf1 = malloc(pathlen + inslen + 1);

    dst = buf1;

    // ext must be to the right of base
    do {
        if (ext == NULL)
            break;
        if (base == NULL)
            break;
        if (ext >= base)
            break;
        ext = NULL;
    } while (0);

    // copy over all but extension
    if (ext != NULL)
        cpylen = ext - path;
    else
        cpylen = pathlen;
    dst = mempcpy(dst,path,cpylen);

    // copy over the date stamp
    dst = mempcpy(dst,ins_string,inslen);

    // copy over the extension
    if (ext != NULL) {
        cpylen = &path[pathlen] - ext;
        dst = mempcpy(dst,ext,cpylen);
    }

    *dst = 0;

    return buf1;
}


void log_open(char const **log_file, FILE **fd_log, char const *filename, _Bool file_date, char const *format)
{
  if ( file_date ) {
    *log_file = log_insert_time(NULL,filename,0);
  }
  else if (strcmp(filename, "-") == 0 )
  {
    char *buf1 = strdup("/dev/stdout");
    *log_file = buf1;
  }
  else {
    char *buf0 = strdup(filename);
    *log_file = buf0;
  }

  // Open log file
  if ((*fd_log = fopen(*log_file, "a")) < 0) {
    error_message (ERROR, "ERROR:LOG: Failed to open the log file err:%d - %s", errno, strerror(errno));
    //return -1;
  }
  // Put proper CSV header here
  fprintf(*fd_log, "%s\n", format);

}

void log_restart(char const **log_file, FILE **fd_log, char const *filename, _Bool file_date, char const *format)
{
  static int log_restarts = 0;
  error_message(INFO,"Restarting log file");
  fclose(*fd_log);
  // TODO can't free static string
  free((void*)*log_file);
  *log_file = NULL;

  if ( file_date ) {
    log_open(log_file,fd_log,filename,file_date,format);
  } else {
    // Add counter to log
    char insert_str[80];
    sprintf(insert_str,".%d",log_restarts);
    char *new_filename = log_insert_str(NULL,filename,insert_str);
    log_open(log_file,fd_log,new_filename,file_date,format);
  }

  log_restarts++;

}
