#include "../src/logfile.h"
#include <stdio.h>

int main(int argc,char *argv[])
{
  LEVEL_DEBUG=DEBUG; // error_handling.c
  // 
  int _retval = 0;
  char inputs[][30] = {
    "test",
    " test\n",
    "test \n",
    "test\n\r",
    "\tte st\r\n",
    "test",
    "\ttest",
    "test\t",
    "te st",
    "\ntest",
    " test ",
    "test "
  };
  char outputs[][30] = {
    "test",
    "test",
    "test",
    "test",
    "test",
    "test",
    "test",
    "test",
    "test",
    "test",
    "test",
    "test"
  };
  printf("Test Size: %d\n", sizeof(inputs)/sizeof(inputs[0]));
  for ( int i=0 ; i<sizeof(inputs)/sizeof(inputs[0]) ; i++ )
  {
    printf("Outputs: %s : Expected: %s",filter_chars("\t\n\r ",inputs[i]),outputs[i]);
    if ( strcmp(inputs[i],outputs[i]) == 0 )
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
