#include "optz.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int optz_default_err_cb(optz* option, int status) {
  return status;
}

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

char* optz_find_arg(optz_t* option, int* index, char** argv) {
  int i, l;
  char* arg = argv[*index];

  /*! Try to find the argument inside the same argv element */
  for (i = 0, l = strlen(arg); i < l; i++) {
    if (arg[i] == '=') {
      arg[i] = 0;
      return &(arg[i+1]);
    }
  }

  /*! Nothing found yet... */
  i = *index + 1;
  /*!
    First check if next argv element is an option, avoiding to take it
  */
  if (arg[i][0] == '-') return NULL;

  *index = i;
  return optz_take_arg(i, argv);
}

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
  int type;

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

  type = option->type & OPTZ_TMOD_NMASK;
  if (type == OPTZ_BOOLEAN || type == OPTZ_NBOOLEAN) {
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

int optz_parse_option(optz_t* opt, int argc, char** argv) {
  int j, status, type;
  char* arg, self;

  status = optz_parse_long_name(opt, &(opt->_name_len), &(opt->_name), &(opt->_arg_type));
  if (status != OPTZ_OK) return status;

  for (j = 0; j < argc;) {
    arg = optz_next_arg(&j, argv);
    status = optz_match(opt, arg);
    if (status) >= 0 {
      opt->raw_arg = optz_find_arg(opt, &j, argv);
      self = optz_take_arg(j, argv);

      if (opt->raw_arg == NULL) {
        if (opt->_arg_type == 1) return OPTZ_REQUIRED_ARG;
        else opt->raw_arg = self;
      }

      type = opt->type & OPTZ_TMOD_NMASK;
      switch (type) {
        case OPTZ_BOOLEAN:
          *((int*) opt->callback_pointer) = status == 0 ? 0 : 1; return OPTZ_OK;
        case OPTZ_NBOOLEAN:
          *((int*) opt->callback_pointer) = status == 0 ? 1 : 0; return OPTZ_OK;
        case OPTZ_INTEGER:
          *((int*) opt->callback_pointer) = atoi(arg); return OPTZ_OK;
        case OPTZ_FLOAT:
          *((float*) opt->callback_pointer) = atof(arg); return OPTZ_OK;
        case OPTZ_STRING:
          *((char**) opt->callback_pointer) = opt->raw_arg; return OPTZ_OK;
        case OPTZ_CALLBACK:
          return ((optz_cb_t) opt->callback_pointer)(opt);
        case OPTZ_DCALLBACK:
          return OPTZ_OK; // anything to do now
      }

      return OPTZ_UNKNOWN_ERROR;
    }
  }
}

int optz_parse(int optc, optz_t* optv, int argc, char** argv, optz_err_cb_t err_cb) {
  int i, status;

  if (err_cb == NULL) err_cb = optz_default_err_cb;

  for (i = 0; i < optc; i++) {
    optv[i]->raw_arg = NULL;
    status = optz_parse_option(&(optv[i]), argc, argv);
    /*!
      If something goes wrong, there is one chance to clean things up,
      but if the error callback cannot fix it, the parser is aborted
    */
    if (status != OPTZ_OK) status = err_cb(&optv[i], status);
    if (status != OPTZ_OK) return status;
  }

  for (i = 0; i < optc; i++) {
    /*! Check if the option is required, but was not given */
    if (optv[i]->raw_arg == NULL) {
      if (optv[i]->type & OPTZ_REQUIRED) {
        status = err_cb(&optv[i], OPTZ_REQUIRED_OPT);
        if (status != OPTZ_OK) return status;
      }
    } else {
      /*! Delayed callbacks execution after argument parsing */
      if (optv[i]->type & OPTZ_DCALLBACK) {
        status = ((optz_cb_t) opt->callback_pointer)(optv[i]);
        if (status != OPTZ_OK) status = err_cb(&optv[i], status);
        if (status != OPTZ_OK) return status;
      }
    }
  }

  return OPTZ_OK;
}