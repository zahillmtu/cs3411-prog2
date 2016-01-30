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
#include <stdint.h>


int checkMagicBytes (FILE *fp) {

    int readCheck = 0;
    char magicBytes[6];
    char magicCheck[6];

    magicCheck = "CS3411"

    readCheck = fread(&magicBytes, sizeof(char), 6, fp);

    // check to make sure the file was read properly
    if (readCheck < 1) {
        printf("An error occured while Reading - Exiting\n");
        fclose(fp);
        exit(1);
    }

    // check to make sure the file is a valid using the magicBytes


}

int main (void) {


}
