#include "../src/logfile.h"
#include <stdio.h>

int main(int argc,char *argv[])
{
  LEVEL_DEBUG=DEBUG; // error_handling.c
  // 
  int _retval = 0;
  char *inputs[] = {
    "log.csv",
    ".log.csv",
    "/dir/log.ext",
    "/dir.path/.log.ext.gz",
    "/.",
    "/.log",
    "./path/dir/log.csv",
    "./path/.dir/log.csv",
    "./.path.dir/xxx/log.csv",
    "./.path.dir./xxx/log.csv",
    "./.path.dir./.xxx/log.csv",
    "./.path.dir./.xxx/.log.csv"
  };
  char *outputs[] = {
    "log-0.csv",
    ".log-0.csv",
    "/dir/log-0.ext",
    "/dir.path/.log-0.ext.gz",
    "/.-0",
    "/.log-0",
    "./path/dir/log-0.csv",
    "./path/.dir/log-0.csv",
    "./.path.dir/xxx/log-0.csv",
    "./.path.dir./xxx/log-0.csv",
    "./.path.dir./.xxx/log-0.csv",
    "./.path.dir./.xxx/.log-0.csv"
  };
  printf("Test Size: %ld\n", sizeof(inputs)/sizeof(inputs[0]));
  for ( int i=0 ; i<sizeof(inputs)/sizeof(inputs[0]) ; i++ )
  {
    char buf1[1000] = {0};
    log_insert_str(buf1,inputs[i],"-0");
    printf("Outputs: %s : Expected: %s",buf1,outputs[i]);
    if ( strcmp(buf1,outputs[i]) == 0 )
    {
      printf("\t:: PASS\n");
    }
    else
    {
      printf("\t:: FAIL\n");
      _retval += 1;
    }
  }
  return _retval;
}
