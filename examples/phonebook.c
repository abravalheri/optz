/*! #phonebook.c

   The file `examples/phonebook.c` provides a practical example of how to use the `optz`
   library to write beatiful command line interfaces for your programs.
 */
#include <stdio.h>
#include <string.h>

/*!
   Supose that we have an in-memory representation of a phonebook
   that can hold at maximum 200 contacts, with 12 phone numbers per contact
   and all our phone numbers are strings with 9 characters.

   In a real program this representation should be persisted in the disk, but
   I will omit this code, since it's an example.
 */
#define MAX_CONTACTS 200
#define MAX_NUMBERS_PER_CONTACT 12
#define NUMBER_LENGTH 9

typedef struct {
  char name[MAX_CONTACTS];
  char number[MAX_NUMBERS_PER_CONTACT][NUMBER_LENGTH];
} Phonebook;

/*!
   In order to work properly, the `phonebook` program will be able to search the
   entries by name using the `find_name` function.
 */
int find_name(Phonebook* phonebook, char* name) {
  int i;

  for (i = 0; phonebook[i].name != NULL; i++)
    if (strcmp(phonebook.name, name) == 0) return i;

  return -1;
}

/*!
   The program will be also able to associate a given phone number to a name
   using the function `add_number`. If the name doesn't exist in the phonebook,
   the same function will automatically create an entry.
 */
int add_number(Phonebook* phonebook, char* name, char* number) {
  int i, j;

  for (i = 0; phonebook[i].name != NULL; i++)
    if (strcmp(phonebook.name, name) == 0) break;

  /* As one entry will be added to the array,
     the new end should be marked to ensure
     that infinity loops will not occur    */
  phonebook[i+1].name = NULL;

  if (phonebook[i].name == NULL)
    strcpy(phonebook[i].name, name);

  for (i = 0; phonebook[i].number[j] != NULL; i++)
    if (strcmp(phonebook[i].number[j], number) == 0) break;

  if (phonebook[i].number[j] == NULL)
    strcpy(phonebook[i].number[j], number);

  return i;
}

int main(int argc, char** argv) {
  /*! Before the program start it will populate the memory with some fixtures. */
  Phonebook phonebook[50] = {
    /* The NULL pointer will be used to mark the end of arrays */
    { "John" , { "9999-9999", "3333-3333", NULL } },
    { "Maria", { "5555-5555", NULL } },
    { "James", { "4444-4444", NULL } },
    { NULL }
  };

  /*!
     It's necessary to create some variables that will be used by the parser,
     passing or receiving information
   */

  char  version[] = "0.1.0";/*! String containing the program version.        */
  char* contact  = NULL;    /*! Will to point to a string containing
                                the contact name after optz parsing.          */
  int len = 12;             /*! Amount of phone numbers to be added to
                                the phonebook. The variable needs to be
                                initialized with the maximum value permitted
                                corresponding to the length of the array that
                                will receive the pointer to the strings passed
                                be the user. After opt parsing, eventually this
                                variable will hold the number of positions
                                actually occupied positions in the array.     */
  char* numbers[12];        /*! Array of pointers (to strings, char*).
                                After the parsing each element of the array
                                will be a pointer to a string representing
                                a phone number to be added to phonebook.      */
  int show = 1;             /*! Amount of phone numbers to be displayed by
                                the program. The variable needs to be
                                initialized with the default value, but after
                                optz parsing eventually will hold the number
                                passed by the user in the terminal.           */

  optz_t options[] = {
    { "-v", "--version", "Output program version",
      OPTZ_CALLBACK , &optz_version_cb, &version },
    /*! The first field in the struct represents the short-name option style
        and the second the long-name. The third field is the description (that
        will be printed out in the help), the fourth is the option type. The last
        two fields are related to the option callback. For options of type
        OPTZ_CALLBACK, the fifth field is a pointer to a function with signature
        `int func(optz* option)`, and the sixth is a aditional parameter passed
        thougth a `void*` pointer. */
    { "-h", "--help", "Display the help message.",
      OPTZ_CALLBACK , &optz_help_cb },
    { "-c", "--contact <name>", "Name of the person to be added or searched .\n"
                                "If this option is present, the argument is mandatory.",
      OPTZ_STRING, &contact },
    /*! The angle brackets mark the argument as madatory for the option.
        Once the user can retrieve a list for all the entries in the phonebook
        just by omitting this option, the option itself is not required.*/
    { "-a" , "--add <P1,...>"   , "Comma separeted list of phone numbers to be added.\n"
                                  "If this option is present, the argument is mandatory."   ,
      OPTZ_STRLIST, &numbers, &len },
      /*! For options of type OPTZ_***LIST, the fifth field is a pointer to the
          empty array where the list will be stored, and the sixth is reference
          to a variable that holds the maximum number of elements for this array.
          After ther parsing this value will be modified to the actual number
          of entries. */
    { "-s" , "--show [N]"       , "Display the N first phone numbers for the contact.\n"
                                  "The N parameter is optional and the default value is 1." ,
      OPTZ_INTEGER  , &show },
    /*! The squared brackets represent a optional argument for the option. It's
        important to start the sixth paramenter with a default value. */
  };

  optz_parse(optz, /* size of options array */ 5, argc, argv, error_cb, &error_param);
}