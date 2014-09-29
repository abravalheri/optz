#ifndef __OPTZ_H__
#define __OPTZ_H__

#include <error.h>

#ifndef EINVAL
#define EINVAL 22
#endif

#define OPTZ_MALFORMED_OPTION EINVAL

typedef struct {
  char* short_form;
  char* long_form;
  char* description;
  int   type;
  void* callback_pointer;
  void* aditional_param;
  char* raw_arg; /* Read-only */
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