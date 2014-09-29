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

static int optz_match(optz_t* option, char* arg) {
  if (arg[0] != '-') return 0;

}

/**
 * Parse the long_form string, returning some useful information
 *
 * Arguments:
 *   optz_t* option   pointer to option structure
 *   int*    length   pointer to integer that will be filled with the name length
 *   char**  name     pointer to variable that will hold the beginin of string
 *   int*    arg_type pointer to integer that will be filled with arg type:
 *           -1 optional, 0 no argument, 1 required
 *
 * Return:
 *   Pointer to argument name without starting '--'
 */
static int optz_parse_long_name(optz_t* option, int* length, char** name, int* arg_type) {
  int i;
  char* opt = option->long_form;

  if (opt[0] != '-' && opt[1] != '-')
    return OPTZ_MALFORMED;

  *arg_type = 0;
  *name = &(opt[2]);

  for (i = 2; opt[i] != 0; i++) {
    if(opt[i] == ' ' || opt[i] == '=') {
      if (opt[i+1] == '[') *arg_type = -1;
      else if (opt[i+1] == '<') *arg_type = 1;
      break;
    }
  }

  *length = i-2;

  return OPTZ_OK;
}

int optz_parse(int optc, optz_t* optv, int argc, char** argv) {
  int i, j, status;
  char* arg;
  optz_t* opt;

  for (i = 0; i < optc; i++) {
    opt = &(optv[i]);
    status = optz_parse_long_name(opt, &(opt->_name_len), &(opt->_name), &(opt->_arg_type));
    for (j = 0; j < argc;) {
       arg = optz_next_arg(&j, argv);
    }
  }
}