assembler: firstPass.o secondPass.o validation.o main.o utilities.o createFiles.o
	gcc -g -ansi -Wall validation.o firstPass.o secondPass.o main.o utilities.o createFiles.o -lm -pedantic -o tests/assembler


validation.o: validation.c assembler.h
	gcc -c -g -ansi validation.c -lm -Wall -pedantic -o validation.o 

main.o: main.c assembler.h
	gcc -c -g -ansi main.c -lm -Wall -pedantic -o main.o 

firstPass.o: firstPass.c assembler.h
	gcc -c -g -ansi firstPass.c -lm -Wall -pedantic -o firstPass.o 

secondPass.o: secondPass.c assembler.h
	gcc -c -g -ansi secondPass.c -lm -Wall -pedantic -o secondPass.o 

utilities.o: utilities.c assembler.h
	gcc -c -g -ansi utilities.c -lm -Wall -pedantic -o utilities.o 

createFiles.o: createFiles.c assembler.h
	gcc -c -g -ansi createFiles.c -lm -Wall -pedantic -o createFiles.o 