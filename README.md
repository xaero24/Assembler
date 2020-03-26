### Assembler

For your consideration, a simple assembler - without linking - that parses a list of files.
The output code is 3 files in case of success:
.en for entry tag references
.ex for for external tag references
.ob with the octal code for each parsed code line.

# How to run:
From the project root (where the makefile is located) run:
```
$ make
$ cd tests
$ ./assembler <file1> <file2> <file3>
```

Note that the filenames are of the .as files you want to parse, WITHOUT the .as suffix.

# TODO:
1. [] Remove all TODO tags
2. [] Update the dynamic allocations to correct allocations
3. [] Parsers - update the utilities functions
4. [] Validation - update validators
5. [] File creation
6. [] Memory deletion
7. [] Update the makefile from tests/assembler to assembler
8. [] Update fully the secondRun.c file based on the firstRun.c file