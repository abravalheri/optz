#ifndef __OPTZ_H__
#define __OPTZ_H__

#define OPTZ_ERRNO_BASE 200

#define OPTZ_OK 0
enum {
  OPTZ_UNKNOWN_ERROR = OPTZ_ERRNO_BASE,
  OPTZ_MALFORMED,
  OPTZ_REQUIRED_OPT,
  OPTZ_REQUIRED_ARG,
};

typedef struct {
  char* short_form;
  char* long_form;
  char* description;
  int   type;
  void* callback_pointer;
  void* aditional_param;
  char* raw_arg; /* Read-only */

  /* Private */
  char* _name;
  int   _name_len;
  int   _arg_type;
} optz_t;

/* Option type modifiers */
#define OPTZ_TMOD_NBITS 2
#define OPTZ_TMOD_MASK 0xFFFFFFFC
enum {
  OPTZ_OPTIONAL = 0,
  OPTZ_REQUIRED
};

/* Raw option types */
enum {
  /* The 2 first bits are reserved to modifiers */
  OPTZ_STRING = (1 << OPTZ_TMOD_NBITS),
  OPTZ_INTEGER,
  OPTZ_FLOAT,
  OPTZ_BOOLEAN,
  OPTZ_NBOOLEAN,
  OPTZ_CALLBACK,
  OPTZ_DCALLBACK,
  OPTZ_STRLIST,
  OPTZ_INTLIST,
  OPTZ_FLTLIST
};

char* optz_take_arg(int i, char** argv);
char* optz_next_arg(int* i, char** argv);

#endif