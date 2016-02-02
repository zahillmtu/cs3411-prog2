/*
 * Zachary Hill
 * cs3411
 * 1/30/2016
 */


/*
 * This program is designed to implement an
 * achive file tool to create, modify, and
 * extract an archive file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <inttypes.h>

#define FILENAME "example.mtu"

int READMAGIC = 0;

int checkMagicBytes (FILE *fp) {

    int readCheck = 0;
    char magicCheck[6] = "CS3411";
    char magicBytes[6];

    printf("GOT TO THE MAGIC STUFF\n");
    readCheck = fread(&magicBytes, sizeof(char), 6, fp);
    printf("%s\n", magicBytes);

    // check to make sure the file was read properly
    if (readCheck < 1) {
        printf("An error occured while Reading - Exiting\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // check to make sure the file is a valid using the magicBytes
    READMAGIC = 1;
    return strcmp(magicBytes, magicCheck);

}

/*
 * This method reads in the next 256 characters
 * which is the max size of the name of the file
 */
void readName(char name_var[static 265], FILE *fp) {

    int readCheck = 0;

    readCheck = fread(name_var, sizeof(char), 256, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        fclose(fp);
        exit(1);
    }

    return;
}

/*
 * This method reads in the next 8 bytes assumed
 * to be the size of the file
 */
void readSize(uint64_t *size_var, FILE *fp) { // pass in a pointer or return an array??

    int readCheck = 0;

    readCheck = fread(size_var, sizeof(char), 8, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        fclose(fp);
        exit(1);
    }

    return;
}

/*
 * This method reads in the next byte and determines
 * if the byte says the file is to be deleted or stay
 * Returns 1 if the file exists, 0 otherwise
 */
void readExists(uint8_t *exists_var, FILE *fp) { // pass in a pointer or return an array??

    int readCheck = 0;

    readCheck = fread(exists_var, sizeof(char), 1, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        fclose(fp);
        exit(1);
    }

    return;
}

/*
 * This method reads in the next 4 bytes that
 * contain the mode_t data for the file
 */
void readMode(uint32_t *mode_var, FILE *fp) { // pass in a pointer or return an array??

    int readCheck = 0;

    readCheck = fread(mode_var, sizeof(char), 4, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        fclose(fp);
        exit(1);
    }

    return;
}


int main (int argc, char* argv[]) {

    int opt;
    // int readcheck = 0;
    int extractIndex = 2;
    char fileName[256];
    uint64_t fileSize;

    if (argc == 0) {
        //print help stuff
        printf("HELP STUFF\n");
    }

    // use to parse options on command line
    opt = getopt(argc, argv, "a:d:x:");

    switch (opt) {
        case('a'):

            printf("This is the case 'a'\n");
            break;
        case('d'):

            printf("This is the case 'd'\n");
            break;
        case('x'):
            // check if the archive file exists
            if (argv[2] == NULL) {
                printf("Archive file does not exist - Exiting\n");
                exit(1);
            }
            if (access(argv[2], R_OK) == 0) {
                // File exists and can read
                printf("File Exists\n");
                FILE *fp = fopen(argv[2], "r");
                if (fp == NULL) {
                    perror("fopen");
                    exit(1);
                }
                printf("GOT HERE\n");
                if (READMAGIC == 0) {
                    if (checkMagicBytes(fp) == 0) {
                        printf("Valid .mtu file\n");
                    }
                    else {
                        printf("Not a valid .mtu file - Exiting\n");
                        exit(1);
                    }
                }
                while (extractIndex < argc) { // change to know when at end of file

                    readName(fileName, fp);
                    printf("file name is: %s\n", fileName);
                    readSize(&fileSize, fp);
                    printf("File size is: %"PRIu64"\n", fileSize);

                    // seek forward to skip reading the data
                    fseek(fp, fileSize, SEEK_CUR);

                    extractIndex = extractIndex + 1;

                }

            }
            else {
                // File does not exist
                perror("access");
                exit(1);
            }
            break;

        case('?'): // Some option not expected from opt
            // print help
            printf("HELP STUFF\n");
            break;

        default:
            // print help
            printf("HELP STUFF\n");
            break;
    }

    return 0;

}

/*
 * References
 *  http://stackoverflow.com/questions/579734/assigning-strings-to-arrays-of-characters
 *  http://www.programmingsimplified.com/c/program/print-string
 *  http://www.thegeekstuff.com/2013/01/c-argc-argv/
 *  http://stackoverflow.com/questions/5573310/difference-between-passing-array-and-array-pointer-into-function-in-c
 */
