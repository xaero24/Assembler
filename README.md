# Assembler

For your consideration, a simple assembler - without linking - that parses a list of files.
The output code is 3 files in case of success:
.en for entry tag references
.ex for for external tag references
.ob with the octal code for each parsed code line.

## How to run:
From the project root (where the makefile is located) run:
```
$ make
$ ./assembler <file1> <file2> <file3>
```

Note that the filenames are of the .as files you want to parse, WITHOUT the .as suffix.

Some test files, as given by other members of team, are in the "tests" dir.

## TODO:
1. [X] create output files
2. [X] Compile on ubuntu
3. [X] Zip 
