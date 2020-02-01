
#include "parser.c"
#include "reader.c"

int main(int argc, char* argv[]){
    char fileName[256], fileExt[6];
    int fileIndex, fileNameLength;
    FILE* inFile = NULL;

    if(argc<2){
        printf("ERROR: No files were given.\n");
        exit(0);
    }
    for(fileIndex = 1; fileIndex < argc; fileIndex++){
        //Run assembler on each file after error checks
        strcpy(fileName, argv[fileIndex]);
        strcat(fileName, ".as");
        inFile = fopen(argv[fileIndex], "r");
        if (inFile == NULL){
            printf("ERROR: File name does not exist.\n");
        }
        else{
            //parse file
            parseFile(inFile);
        }
    }
}