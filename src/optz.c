#include "optz.h"
#include <string.h>
#include <stdio.h>

char* optz_take_arg(int i, char** argv) {
  char* original = argv[i];

  /*
    Considering all the strings pointed by the argv being consecutive in memory
    and separeted by null characters (\0) according to the following figure
    ((reference))[http://heim.ifi.uio.no/~inf103/Grl/docs/argv.txt]:
     ______________________________________________
    /____________/____________/___________________/|
    |argv[0]| \0 |argv[1]| \0 | ... |argv[N] | \0 |/
    ----------------------------------------------

    If the argv[i] pointer is rewinded by 1 position the resuling string is
    empty.
  */
  argv[i]--;

  return original;
};

char* optz_next_arg(int* i, char** argv) {
  /* Find the first non-empty argv */
  for (; argv[*i][0] == 0; (*i)++);

  return argv[i];
};

static char* optz_get_long_name(optz_t* option, int* length, int* optional_arg) {

}

int optz_parse(int optc, optz_t* optv, int argc, char** argv) {
  int i, j;

  for (int i = 2; i < count; ++i) {
    /* code */
  }
}