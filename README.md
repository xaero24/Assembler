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
$ ./assembler <file1> <file2> <file3>
```

Note that the filenames are of the .as files you want to parse, WITHOUT the .as suffix.

Some test files, as given by other members of team, are in the "tests" dir.

# TODO:
[X] Remove all TODO tags
[X] Update the dynamic allocations to correct allocations
[X] Parsers - update the utilities functions
[X] Validation - update validators
[X] File creation
[X] Memory deletion
[X] Update the makefile from tests/assembler to assembler
[X] Update fully the secondRun.c file based on the firstRun.c file
[X] Write test files (added by team)
[X] Update addresses from labels after second run
[X] Convert code to octal (after second run it will be easier)
[X] Save or derive numbers of instruction and data frames for .ob file header
[X] Remove unneeded variables in first and second run
[X] Update function definitions