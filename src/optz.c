#include "optz.h"
#include <string.h>
#include <stdio.h>

/**
 * Mark the given position in the argv array as already consumed in the parsing
 * process.
 *
 * Notice
 *   - it requires all the strings pointed by the argv being consecutive
 *     in memory and separated by null characters (\0) according to the figure
 *     [(reference)](http://heim.ifi.uio.no/~inf103/Grl/docs/argv.txt):
 *
 * ```
 *    ______________________________________________
 *   /____________/____________/___________________/|
 *   |argv[0]| \0 |argv[1]| \0 | ... |argv[N] | \0 |/
 *   ----------------------------------------------
 * ```
 *   - it assumes that argv[0] will be not parsed
 *   - it modifies the pointed memory region
 *
 * Arguments
 *   : index : position in the argv array to be marked
 *   : argv  : argument array passed by the process
 *
 * Return
 *   Pointer to the original argument first character
 */
char* optz_take_arg(int index, char** argv) {
  char* original = argv[index];

  /*!
    Once all the argv[i] strings are preceded by '\0' for i > 0, if the pointer
    is rewinded by 1 position the resuling string will start with a '\0' and
    there for will be considered empty.
  */
  argv[index]--;

  return original;
};

/**
 * Find the next non-empty argv
 *
 * Arguments:
 *   : index : pointer to the first position to be analyzed in the argv array.
 *             Notice the value pointed will change for the position where the
 *             non-empty arg actually is.
 *   : argv  : array with args to be analyzed
 *
 * Return:
 *    Pointer to the non empty argument
 */
char* optz_next_arg(int* index, char** argv) {
  for (; argv[*index][0] == 0; (*index)++);

  return argv[index];
};

/**
 * Indicates if the given argument corresponds to the given option
 *
 * Arguments
 *   : option : option to be analyzed
 *   : arg    : string suposed to match the given option
 *
 * Return
 *   : -1 : if the argument does not match the option;
 *   :  1 : if the argument match the option;
 *   :  0 : if the option has type OPTZ_BOOLEAN or OPTZ_NBOOLEAN and the argument
 *          matches the "oposite version" of the option (e.g., if a "--download"
 *          option exists and the argument "--skip-download" or "--no-download"
 *          is given ).
 */
static int optz_match(optz_t* option, char* arg) {
  /*! Options should start with a '-' character */
  if (arg[0] != '-') return -1;
  /*! If there is just one '-' character, it must be a short option */
  if (arg[1] != '-') {
    if (arg[1] == option->short_form[1]) return 1;
    /*! Short options does not have "oposite form" */
    return -1; /* pessimist */
  }

  /*!
    To reach this point it must start with 2 '-', so the long form for the
    option should be analyzed
  */

  /*!
    If the argument matches the option name there is no need to check
    for boolean types
  */
  if (strncmp(&(arg[2]), &(option->_name), option->_name_len) == 0) return 1;

  if (option->type == OPTZ_BOOLEAN || option->type == OPTZ_NBOOLEAN) {
    /*! "Oposite option" analysis  */
    if (strncmp(&(arg[2]),  "no", 2) == 0
      && strncmp(&(arg[4]), &(option->_name), option->_name_len) == 0)
      return 0;
    if (strncmp(&(arg[2]),  "skip", 4) == 0
      && strncmp(&(arg[6]), &(option->_name), option->_name_len) == 0)
      return 0;
  }

  return -1; /* pessimist */
}

/**
 * Parse the long_form string, returning some useful information
 *
 * Arguments:
 *   option   - pointer to option structure
 *   length   - pointer to integer that will be filled with the name length
 *   name     - pointer to variable that will hold the beginin of string
 *   arg_type - pointer to integer that will be filled with arg type:
 *              -1 optional, 0 no argument, 1 required
 *
 * Return:
 *   OPTZ_MALFORMED
 *   OPTZ_OK
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
    if (status != OPTZ_OK) return status
    for (j = 0; j < argc;) {
      arg = optz_next_arg(&j, argv);
      status = optz_match(opt, arg);
      if (status) >= 0 {
        
      }
    }
  }
}