#!/usr/bin/env bats

EXAMPLE="examples/phonebook.c"
EXAMPLE_OUT="test/phonebook.out"

setup() {
  gcc "$EXAMPLE" --std=c89 -Wpedantic -Wall -Wextra -o "$EXAMPLE_OUT"
}

teardow() {
  rm -Rf "$EXAMPLE_OUT"
}

@test "should find an existing contact with flag -c" {
  run "./test/phonebook.out" -c "John"
  [ "$status" -eq 0 ]
  [[ ${lines[0]} =~ "John" ]]
}

@test "should not find a non existing contact with flag -c" {
  run "./test/phonebook.out" -c "Juan"
  [ ! "$status" -eq 0 ]
  [[ ${lines[0]} =~ "not found" ]]
}

@test "should add a non-existing contact with flag -a" {
  run "./test/phonebook.out" -c "Juan" -a "XXXX-XXXX"
  [ "$status" -eq 0 ]
  [ ${#lines[@]} -eq 3 ]
  [[ ${lines[0]} =~ "contact added" ]]
  [[ ${lines[1]} =~ "Juan" ]]
  [[ ${lines[2]} =~ "XXXX-XXXX" ]]
}

@test "should add numbers with flag -a" {
  run "./test/phonebook.out" -c "Maria" -a "XXXX-XXXX"
  [ "$status" -eq 0 ]
  [ ${#lines[@]} -eq 4 ]
  [[ ${lines[0]} =~ "new numbers added" ]]
  [[ ${lines[1]} =~ "Maria" ]]
  [[ ${lines[2]} =~ "5555-5555" ]]
  [[ ${lines[3]} =~ "XXXX-XXXX" ]]
}

@test "should not restrict the amount of phone numbers without flag -s" {
  run "./test/phonebook.out" -c "John"
  [ ${#lines[@]} -eq 4 ]
  [[ ${lines[0]} =~ "John" ]]
  [[ ${lines[1]} =~ "9999-9999" ]]
  [[ ${lines[2]} =~ "6666-6666" ]]
  [[ ${lines[3]} =~ "3333-3333" ]]
}

@test "should restrict the amount of phone numbers with flag -s" {
  run "./test/phonebook.out" -c "John" -s 2
  [ ${#lines[@]} -eq 3 ]
  [[ ${lines[0]} =~ "John" ]]
  [[ ${lines[1]} =~ "9999-9999" ]]
  [[ ${lines[2]} =~ "6666-6666" ]]
}

@test "flag -s should have the default value 1" {
  run "./test/phonebook.out" -c "John" -s
  [ ${#lines[@]} -eq 2 ]
  [[ ${lines[0]} =~ "John" ]]
  [[ ${lines[1]} =~ "9999-9999" ]]
}