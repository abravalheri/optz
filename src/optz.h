#ifndef __OPTZ_H__
#define __OPTZ_H__

#define OPTZ_ERRNO_BASE 200

#define OPTZ_OK 0
enum {
  OPTZ_UNKNOWN_ERROR = OPTZ_ERRNO_BASE,
  OPTZ_MALFORMED,
  OPTZ_REQUIRED_OPT,
  OPTZ_REQUIRED_ARG
};

typedef int (*optz_cb_t)(optz* option);
typedef int (*optz_err_cb_t)(optz* option, int status);

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
#define OPTZ_TMOD_MASK  0x00000003
#define OPTZ_TMOD_NMASK 0xFFFFFFFC
enum {
  OPTZ_OPTIONAL = 0,
  OPTZ_REQUIRED
};

/* Raw option types */
enum {
  /* The 2 first bits are reserved to modifiers */
  OPTZ_STRING    =  1 << OPTZ_TMOD_NBITS,
  OPTZ_INTEGER   =  2 << OPTZ_TMOD_NBITS,
  OPTZ_FLOAT     =  3 << OPTZ_TMOD_NBITS,
  OPTZ_BOOLEAN   =  4 << OPTZ_TMOD_NBITS,
  OPTZ_NBOOLEAN  =  5 << OPTZ_TMOD_NBITS,
  OPTZ_CALLBACK  =  6 << OPTZ_TMOD_NBITS,
  OPTZ_DCALLBACK =  7 << OPTZ_TMOD_NBITS,
  OPTZ_STRLIST   =  8 << OPTZ_TMOD_NBITS,
  OPTZ_INTLIST   =  9 << OPTZ_TMOD_NBITS,
  OPTZ_FLTLIST   = 10 << OPTZ_TMOD_NBITS
};

int optz_default_err_cb(optz* option, int status);
char* optz_take_arg(int i, char** argv);
char* optz_next_arg(int* i, char** argv);
int optz_parse(int optc, optz_t* optv, int argc, char** argv, optz_err_cb_t err_cb);
#endif