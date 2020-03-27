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
[] Remove all TODO tags
[X] Update the dynamic allocations to correct allocations
[X] Parsers - update the utilities functions
[X] Validation - update validators
[] File creation
[] Memory deletion
[] Update the makefile from tests/assembler to assembler
[] Update fully the secondRun.c file based on the firstRun.c file
[] Write test files
[] Update addresses from labels after second run
[] Convert code to octal (after second run it will be easier)
[] Save or derive numbers of instruction and data frames for .ob file header
[] Remove unneeded variables in first and second run
[] Update function definitions