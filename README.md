# optz

Easy command parser written in C with ZERO memory waste.

## Description

This project aims to provide a simple option parser for development of command line applications.

The main requirement is to keep the memory usage as low as possible, with zero waste, in order to make it suitable for embedded projects.

## Getting Started

>The key words "**MUST**", "**MUST NOT**", "**SHALL**", "**SHALL NOT**", "**SHOULD**", "**SHOULD NOT**", and "**MAY**" in this document are to be interpreted as described in (RFC 2119)[http://tools.ietf.org/html/rfc2119].

The library interface was designed to be _stupid simple™_ and is only composed by one custom type and the three functions as described bellow:

### `optz_t`

The `optz_t` type define a structure that will be used by the parser. The public fields are:

```c
typedef struct {
  char* short_form;
  char* long_form;
  char* description;
  int   type;
  void* callback_pointer;
  void* aditional_param;
  char* raw_arg; /* Read-only */
} optz_t;
```


The value pointed by `short_form` **MUST** be a single character preceded by a single hyphen, e.g. `"-h"`. The total length of the string used **MUST BE** 2.

The value pointed by `long_form` **MUST** be formed by multiple characters preceded by two hyphens, e.g. `"--help"`. If it is possible to pass an argument to to the option, its placeholder **MUST** be included in the `long_form`, e.g. `--ntimes <N>`, and **MUST NOT** be included in the `short_form`. The angle brackets **MUST** be used for required arguments and the square brackets **MUST** be used for optional arguments.

The string pointed by `description` is intended to be printed out in the help and **SHOULD** provide a brief about the option usage.

The `type` field is a integer that can assume the following values:

* `OPTZ_STRING`
  - The developer **SHOULD** set the `callback_pointer` field using a `char*` variable. After the parsing, this variable will point to the string argument.
* `OPTZ_INTEGER`
  - The developer **SHOULD** set the `callback_pointer` field with a reference to a variable of type `int`. This variable will have the numeric value given by the argument.
* `OPTZ_FLOAT`
  - similar to `OPTZ_INTEGER`, but `callback_pointer` **MUST** reference a `float`.
* `OPTZ_BOOLEAN`
  - when the option is passed, it represents a `TRUE` value. Does not accept arguments, but modify the variable pointed by the fifth field of the `optz_t` struct. The developer **SHOULD** set the `callback_pointer` field with a reference to a variable of type `int` (or `bool` if available).
* `OPTZ_NBOOLEAN`
  - similar to `OPTZ_NBOOLEAN`, but the variable referenced by `callback_pointer` will be changed to `FALSE` when this option is passed.
* `OPTZ_STRLIST`
  - argument is interpreted as a list of strings separed by commas. The developer **SHOULD** set the `callback_pointer` field with a reference to a array of `char*` and the `aditional_param` with a reference to a `int` variable. The array will be filled with pointers to each string passed. The `int` variable needs to be initialized with the maximum permitted length of the array and after the parsing will hold the number of positions actually filled.
* `OPTZ_INTLIST`
  - similar to `OPTZ_STRLIST`, but the `callback_pointer` needs to referer a `int` array, that will be filled with the numeric values of the list.
* `OPTZ_FLTLIST`
  - similar to `OPTZ_INTLIST`, but  `callback_pointer` needs to referer a `float` array.
* `OPTZ_CALLBACK`
  - when the option is present, a callback function runs as soon as the option is recongnized;
  - a pointer to the function (`int (*callback)(optz*)`) that will be used as callback needs to be specified in the field `callback_pointer`;
  - the developer can pass information to this function using the field `aditional_param`.
* `OPTZ_DELAYED_CALLBACK`
  - similar to `OPTZ_CALLBACK`, but waits all the options of other types be parsed to run the callback function.

The default behavior of the parser is not consider error the absence of an option. The developer can change this by combining the desired type with `OPTZ_REQUIRED` using the bitwise `&` operator, e.g. `OPTZ_REQUIRED & OPTZ_INTEGER`.

There is no relation between the mandatory of the option and its argument.
A required option can have a optional argument, or even no arguments.
Similarly when a non-required option is passed it can demand an mandatory argument. The mandatory of arguments are expressed by the type of brackets in the `long_form` field.

The callbacks **SHOULD** return 0 when are performed without error.

If no type is specified, the parser assumes the `OPTZ_STRING` type.

#### Example: the unreal phonebook options

The following code ilustrate the basic `optz_t` usage:

```c
char  version[] = "0.1.0";
char* contact  = NULL;

int len = 12;
char* numbers[12];

int show = 1;

optz_t options[] = {
  { "-v" , "--version",
    "Output program version",
     OPTZ_CALLBACK , &optz_version_cb, &version },
  { "-h" , "--help",
    "Display the help message.",
     OPTZ_CALLBACK , &optz_help_cb, options },
  { "-c" , "--contact <name>",
    "Name of the contact.\n"
    "When absent all the contacts will be considered.",
     OPTZ_STRING  , &contact },
  { "-a" , "--add <P1,...>",
    "Comma separeted list of phone numbers to be added.",
     OPTZ_STRLIST  , &numbers, &len },
  { "-s" , "--show [N]",
    "Display the N first phone numbers for the contact.\n"
    "The N parameter is optional and the default value is 1.",
     OPTZ_INTEGER  , &show }
};

```

#### Example: the ridiculous calendar app options

Supose the following structures and functions exists:

```c
typedef struct {
  int daystamp;
  int minutes;
} moment_t;

typedef struct {
  char* name;
  moment_t start;
  moment_t end;
} event_t;

int delete_event_cb(optz* option) {
  char* event_name = (char*) option->aditional_param;
  destroy_event(event_name);

  return OPTZ_ABORT;
  /* Returning a value different from zero will suspend the program */
}

int parse_moment_cb(optz* option) {
  char* moment_str = option->raw_arg;
  moment_t* moment = (moment_t) option->aditional_param;

  /* Memoize the start of each block in the representation */
  char* year   = &moment_str[0];
  char* month  = &moment_str[4];
  char* day    = &moment_str[6];
  char* hour   = &moment_str[8];
  char* minute = &moment_str[10];

  /* Replace the characters '/', '-', ':' by string terminations
     in order to inform to the `atoi` where to stop the conversion */
  moment_str[3] = 0;
  moment_str[5] = 0;
  moment_str[7] = 0;
  moment_str[9] = 0;

  moment->daystamp = atoi(day) + 100*atoi(month) + 10000*atoi(year);
  moment->minutes  = atoi(minute) + 60*atoi(hour);

  return 0; /* No errors */
}
```

The following code ilustrate how callbacks can be used to implement custom type parsing:

```c
char* event;
moment_t start;

optz_t options[] = {
  { "-e" , "--event <name>"             , "Name of the event."                , OPTZ_REQUIRED                 , event },
  { "-s" , "--start <YYYY/mm/dd-HH:MM>" , "Moment when the event will start." , OPTZ_REQUIRED & OPTZ_CALLBACK , parse_moment_cb , &start },
  { "-x" , "--delete"                   , "Delete this event instead of Add." , OPTZ_DELAYED_CALLBACK         , delete_event_cb , event }
  /* This callback needs to wait the \"--event\" option be parsed, in order to have acces to the right event name */
};
```