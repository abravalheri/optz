optz
====

Easy command parser written in C with ZERO memory waste.

## Introduction

This project aims to provide a simple option parser for development of cli
applications.

The main requirement is to keep the memory usage as low as possible, with zero
waste, in order to make it suitable for embedded projects.

## Example

[-n] [-v] [--force | -f] [--interactive | -i] [--patch | -p]
    [--edit | -e] [--[no-]all | --[no-]ignore-removal | [--update | -u]]
    [--intent-to-add | -N] [--refresh] [--ignore-errors] [--ignore-missing]
    [--] [<pathspec>...]

```c
int asc_order = 1;
int ls = MAX_LENGTH, ln = MAX_LENGTH;
int scores[MAX_LENGTH];
int round = 1;
int verbose;
int hightlight = -1;

char * judge;
char * names[MAX_LENGTH];
char version[]  = "1.2.1";

optz options[] = {
  { "-v", "--version" , "Output program version"                      , OPTZ_CALLBACK, &optz_version_cb, &version },
  { "-h", "--help"    , "Display the help message."                   , OPTZ_CALLBACK, &optz_help_cb },
  { "-a", "--asc"     , "Display scores in the ascending order."      , OPTZ_BOOLEAN , &verbose },
  { "-d", "--desc"    , "Display scores in the descending order."     , OPTZ_NBOOLEAN, &asc_order },
  { "-j", "--judge  <NAME>"      , "Name of the judge"                , OPTZ_REQUIRED & OPTZ_STRING, &judge },
  { "-s", "--scores <S1,S2,...>" , "Comma separeted list of integers" , OPTZ_REQUIRED },
  { "-n", "--names  <N1,N2,...>" , "Comma separeted list of names"    , OPTZ_REQUIRED }, 
  { "-H", "--highlight [N]"      , "Highlight the N first winners"    , OPTZ_INTEGER , &highlight }, 
}

optz_integer_list(&options[4], &scores, &ls);
optz_string_list(&options[5], &names, &ln);

```