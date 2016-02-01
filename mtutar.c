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

#define FILENAME "example.mtu"
#define READMAGIC 0

int checkMagicBytes (FILE *fp) {

    int readCheck = 0;
    char magicCheck[6] = "CS3411";
    char magicBytes[6];

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
void readName(FILE *fp) { // pass in a pointer or return an array??

    readCheck = fread(some_var, sizeof(char), 256, fp);
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
void readSize(FILE *fp) { // pass in a pointer or return an array??

    readCheck = fread(some_var, sizeof(char), 256, fp);
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
void readExists(FILE *fp) { // pass in a pointer or return an array??

    readCheck = fread(some_var, sizeof(char), 256, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        fclose(fp);
        exit(1);
    }

    return;
}

int main (int argc, char* argv[]) {

    int opt;

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
                if (checkMagicBytes(fp) == 0) {
                    printf("Valid .mtu file\n");
                }
                else {
                    printf("Not a valid .mtu file - Exiting\n");
                    exit(1);
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
 */
