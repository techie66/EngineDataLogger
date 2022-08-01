#include "../src/logfile.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
  LEVEL_DEBUG=DEBUG; // error_handling.c
  // Set Stable Timezone for testing purpose
  putenv("TZ=UTC");
  // 
  time_t _time = 1658471847; // -20220722153727
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
    "log-20220722063727.csv",
    ".log-20220722063727.csv",
    "/dir/log-20220722063727.ext",
    "/dir.path/.log-20220722063727.ext.gz",
    "/.-20220722063727",
    "/.log-20220722063727",
    "./path/dir/log-20220722063727.csv",
    "./path/.dir/log-20220722063727.csv",
    "./.path.dir/xxx/log-20220722063727.csv",
    "./.path.dir./xxx/log-20220722063727.csv",
    "./.path.dir./.xxx/log-20220722063727.csv",
    "./.path.dir./.xxx/.log-20220722063727.csv"
  };
  printf("Test Size: %d\n", sizeof(inputs)/sizeof(inputs[0]));
  for ( int i=0 ; i<sizeof(inputs)/sizeof(inputs[0]) ; i++ )
  {
    char buf1[1000] = {0};
    log_insert_time(buf1,inputs[i],_time);
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
