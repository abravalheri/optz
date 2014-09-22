#include <stdio.h>
#include <string.h>

#define VERSION "0.1.0"

typedef struct {
  char name[200];
  char number[12][12];
} Phonebook;

int find_name(Phonebook* phonebook, char* name) {
  int i;

  for (i = 0; phonebook[i].name != NULL; i++)
    if (strcmp(phonebook.name, name) == 0) return i;

  return -1;
}

int add_entry(Phonebook* phonebook, char* name, char* number) {
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
  Phonebook phonebook[50] = {
    /* The null pointer will be used to mark the end of arrays */
    { "John" , { "9999-9999", "3333-3333", NULL } },
    { "Maria", { "5555-5555", NULL } },
    { "James", { "4444-4444", NULL } },
    { NULL }
  };

  char *numbers[12];
  optz_strlist_p numbers_p = { /*Max length*/ 12, /* char * */ numbers};
  char version[] = VERSION;
  char *contact  = NULL;
  int show = 1;

  optz_t options[] = {
    { "-v" , "--version"      , "Output program version"                                             ,
      OPTZ_CALLBACK , &optz_version_cb, &version },
    { "-h" , "--help"         , "Display the help message."                                          ,
      OPTZ_CALLBACK , &optz_help_cb },
    { "-c" , "--contact"      , "Name of the person to be added or searched in phonebook."           ,
      OPTZ_STRING   , &contact },
    { "-a" , "--add <P1,...>" , "Comma separeted list of phone numbers to be added to the phonebook.",
      OPTZ_STRLIST  , &numbers_p },
    { "-s" , "--show [N]"     , "Display the N first phone numbers for the contact.\n"
                                "The N parameter is optional and the default valur is 1."            ,
      OPTZ_INTEGER  , &show },
  };

  optz_parse(optz, argc, argv);

  /* When used as callback parameter in the `optz_t` struct, the
    `optz_strlist_p.len` field should provides the maximum number of elements
    accepted in the array (`optz_strlist_p.list`). But afther the parsing the
    same field will provide the actual number of elements passed to the program. */
  if (numbers_p.len > 0)
}

// int asc_order = 1;
// int ls = MAX_LENGTH, ln = MAX_LENGTH;
// int scores[MAX_LENGTH];
// int round = 1;
// int verbose;
// int hightlight = -1;

// char * judge;
// char * names[MAX_LENGTH];
// char version[]  = "1.2.1";

// optz options[] = {
//   { "-v", "--version" , "Output program version",
//     OPTZ_CALLBACK, &optz_version_cb, &version },
//   { "-h", "--help", "Display the help message.",
//     OPTZ_CALLBACK, &optz_help_cb },
//   { "-a", "--asc", "Display scores in the ascending order.",
//     OPTZ_BOOLEAN , &verbose },
//   { "-d", "--desc", "Display scores in the descending order.",
//     OPTZ_NBOOLEAN, &asc_order },
//   { "-j", "--judge  <NAME>", "Name of the judge",
//     OPTZ_REQUIRED & OPTZ_STRING, &judge },
//   { "-s", "--scores <S1,S2,...>" , "Comma separeted list of integers",
//     OPTZ_REQUIRED },
//   { "-n", "--names  <N1,N2,...>" , "Comma separeted list of names",
//     OPTZ_REQUIRED },
//   { "-H", "--highlight [N]", "Highlight the N first winners",
//     OPTZ_INTEGER , &highlight }
// }