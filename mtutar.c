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
char *fileContents;

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
void readExists(uint8_t *deleted_var, FILE *fp) { // pass in a pointer or return an array??

    int readCheck = 0;

    readCheck = fread(deleted_var, sizeof(char), 1, fp);
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

/*
 * Method to extract the next file, if the file already exists, then the program
 * should prompt to overwrite. Expects that the file is to be extracted
 */
void extractFile(char name_var[static 256], uint64_t size_var, FILE *fp) {

    int readCheck;
    char contents[size_var];
    FILE * extracted_fp;

    // Read in the contents of the file
    readCheck = fread(contents, sizeof(char), size_var, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        fclose(fp);
        exit(1);
    }


    printf("OPENING FILE\n");
    // Create the destination to extract the file
    extracted_fp = fopen(name_var, "w");

    printf("PUTTING CONTENTS IN FILE\n");
    // Write the contents into the extraction
    fputs(contents, extracted_fp);

    printf("CLOSING THE FILE\n");
    // Finished extracting - Close the file
    fclose(extracted_fp);

}

/*
 * Method used to print the help information if the user provides
 * incorrect or unexpected input
 */
void printHelp(void) {


    printf("mtutar - a program used to create, update, and extract archive files.\n");
    printf("Author: Zachary Hill\n");
    printf("Date last updated: Feburary 7th, 2016\n\n");

    printf("USAGE:\n");
    printf("    mtutar                              Print this help information\n\n");

    printf("    mtutar -a archive.mtu file1 file2   Appends one or more files into a file named archive.mtu.\n"
           "                                        If the archive file doesn't exist, create it. The same file\n"
           "                                        (with the same filename) could be added to the same archive\n"
           "                                        multiple times.\n\n");

    printf("    mtutar -d archive.mtu file1         Deletes a single file (file1) from the archive. Program exits with\n"
           "                                        a non-zero return code and prints a message if the archive doesn't\n"
           "                                        exist or if the file that the user is trying to delete does not\n"
           "                                        exist in the archive. If there are multiple files in the archive\n"
           "                                        with the same name, all of them with a name that matches the file\n"
           "                                        that the user requested to be deleted should be deleted.\n\n");

    printf("    mtutar -x archive.mtu               Extracts the files from archive.mtu into the current directory.\n"
           "                                        If a file that is going to be extracted already exists, the user\n"
           "                                        is asked if they would like to overwrite the existing file. If the\n"
           "                                        archive can't be read or if the extracted files can't be written,\n"
           "                                        the program should exit with a non-zero return code and print an error message.\n");

}



int main (int argc, char* argv[]) {

    int opt;
    // int readcheck = 0;
    char fileName[256];
    uint64_t fileSize;
    uint8_t fileDeleted;
    int seekEnd = 0;


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

                // Seek to the end of file and store value to find EOF
                fseek(fp, 0, SEEK_END);
                seekEnd = ftell(fp);
                // Return the cursor to the beginning of file
                fseek(fp, 0, SEEK_SET);

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

                while (ftell(fp) < seekEnd) { // seekEnd contains EOF position

                    readName(fileName, fp);
                    printf("file name is: %s\n", fileName);
                    readSize(&fileSize, fp);
                    printf("File size is: %"PRIu64"\n", fileSize);
                    readExists(&fileDeleted, fp);
                    printf("File exists? %"PRIu8"\n", fileDeleted);

                    // skip the mode for now
                    fseek(fp, 4, SEEK_CUR);


                    // extract the file
                    if (fileDeleted == 0) {
                        extractFile(fileName, fileSize, fp);
                    }
                    else {
                        // if the file is to be deleted, skip ahead
                        printf("FILE: %s is to be deleted, skipping extraction\n", fileName);
                        fseek(fp, fileSize, SEEK_CUR);
                    }

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
            printHelp();
            break;

        default:
            // print help
            printHelp();
            break;
    }

    return 0;

}

/*
 * References
 *  http://stackoverflow.com/questions/579734/assigning-strings-to-arrays-of-characters
 *  http://www.programmingsimplified.com/c/program/print-string
 *  http://www.thegeekstuff.com/2013/01/c-argc-argv/
 *  http://stackoverflow.com/questions/16523239/c-finding-end-of-file-before-reading
 *  http://stackoverflow.com/questions/5573310/difference-between-passing-array-and-array-pointer-into-function-in-c
 */
