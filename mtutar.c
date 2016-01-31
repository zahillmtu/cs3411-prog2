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

#define FILENAME "example.mtu"

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
    return strcmp(magicBytes, magicCheck);

}

int main (void) {

    int byteCheck;

    FILE *fp = fopen(FILENAME, "r");

    // check for valid file open
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    byteCheck = checkMagicBytes (fp);

    if (byteCheck != 0) {
        printf("Not a valid .mtu file - Exiting\n");
        exit(1);
    }

    printf("Vaild .mtu file\n");

    return 0;

}

/*
 * References
 *  http://stackoverflow.com/questions/579734/assigning-strings-to-arrays-of-characters
 *  http://www.programmingsimplified.com/c/program/print-string
 */
