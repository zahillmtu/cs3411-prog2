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
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

int READMAGIC = 0;

int checkMagicBytes (FILE *fp) {

    int readCheck = 0;
    char magicCheck[7] = "CS3411";
    char magicBytes[7] = "NOT_OK";

    readCheck = fread(&magicBytes, sizeof(char), 6, fp);

    // check to make sure the file was read properly
    if (readCheck < 1) {
        printf("An error occured while Reading - Exiting\n");
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
        exit(1);
    }

    return;
}

/*
 * Method to extract the next file, if the file already exists, then the program
 * should prompt to overwrite. Expects that the file is to be extracted
 *
 */
void extractFile(char name_var[static 256], uint64_t size_var, uint32_t file_mode, FILE *fp) {

    int readCheck;
    char contents[size_var];
    FILE * extracted_fp;
    char userAns[256];

    // Read in the contents of the file
    readCheck = fread(contents, sizeof(char), size_var, fp);
    if (readCheck < 1) {
        printf("An error occured while reading - Exiting\n");
        exit(1);
    }

    // Check if the file exists
    if (access(name_var, F_OK) == 0) {

        // File exists, ask to overwrite
        printf("File '%s' exists. Overwite it? [y/n]\n", name_var);

        // Read in user input

        if (fgets(userAns, 256, stdin) != NULL) {

            if (strcmp(userAns, "y\n") == 0 || strcmp(userAns, "Y\n") == 0) {
                // Overwrite the file
                printf("Overwriting File...\n");
                extracted_fp = fopen(name_var, "w");
                if (extracted_fp == NULL) {
                    perror("fopen");
                    exit(1);
                }
            }
            else {
                return;
            }
        }
        else {
            return;
        }
    }
    else { // file does not exist, just create it
        extracted_fp = fopen(name_var, "w");
        if (extracted_fp == NULL) {
            perror("fopen");
            exit(1);
        }
    }

    // Write the contents into the extraction
    fwrite(contents, sizeof(char), size_var, extracted_fp);

    // Finished extracting - Close the file
    fclose(extracted_fp);

    chmod(name_var, file_mode);

}

/*
 * Writes all the contents of the header to the file given
 * This method assumes file has writing permissions
 */
void writeHeader(char name[static 256], uint64_t size, uint8_t deleted, uint32_t mode, FILE *fp) {

    // Write name
    fwrite(name, sizeof(char), 256, fp);

    // write size
    fwrite(&size, sizeof(char), 8, fp);

    // write deleted byte
    fwrite(&deleted, sizeof(char), 1, fp);

    // write mode
    fwrite(&mode, sizeof(char), 4, fp);

    return;

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
    int fileExists;
    int readCheck = 0;
    int indexIndicator = 3; // used to tell which cmd arg we are using
    int seekEnd = 0;

    char fileName[256];
    uint64_t fileSize;
    uint8_t fileDeleted;
    uint32_t fileMode;

    struct stat fileStruct;
    char deletedByte = 0; // used to test if a file is supposed to be deleted
    char deletionByte = 0x01; // used to set a file deletion byte

    char magicCheck[6] = "CS3411";

    FILE *fp;

    if (argc == 0) {
        //print help stuff
        printf("HELP STUFF\n");
    }

    // use to parse options on command line
    opt = getopt(argc, argv, "a:d:x:");

    // Check to make sure the option is of valid format
    if (optarg != 0) {
        if (argc < 3) { // Not enough args to be vaild
            printf("Invalid Argument Option - Exiting\n\n");
            printHelp();
            exit(1);
        }
        if(strcmp(optarg, argv[2]) != 0) { // picking up attached args
            printf("Invalid Argument Option - Exiting\n\n");
            printHelp();
            exit(1);
        }
    }

    switch (opt) {
        case('a'): // append the archive file

            // check if the file exists
            if (access(argv[2], F_OK == 0)) {
                fileExists = 1;
                printf("Archive file '%s' already exists\n", argv[2]);
            }
            else {
                fileExists = 0;
                printf("Archive file '%s' does not exist - Creating file\n", argv[2]);
                fp = fopen(argv[2], "w");
                if (fp == NULL) {
                        perror("fopen");
                        exit(1);
                }
                fclose(fp);
            }

            // check if the file is writable if it exists
            if (fileExists) {
                if (access(argv[2], W_OK) != 0) {
                    printf("Archive file '%s' has invalid permissions\n", argv[2]);
                    exit(1);
                }
            }

            // open the file for ammending
            fp = fopen(argv[2], "a+");
            if (fp == NULL) {
                    perror("fopen");
                    exit(1);
            }

            // if the file did not exist, write magic bytes
            if (fileExists == 0) {
                fwrite(magicCheck, sizeof(char), 6, fp);
            }
            else { // check for magic bytes
                if (READMAGIC == 0) {
                    fseek(fp, 0, SEEK_SET); // get to the beginning of file to check bits
                    if (checkMagicBytes(fp) == 0) {
                        printf("Valid .mtu file\n");
                        fseek(fp, 0L, SEEK_CUR); // call this whenever switching between reading and writing
                        fseek(fp, 0, SEEK_END); // go to the end of file to add new files
                    }
                    else {
                        printf("Not a valid .mtu file - Exiting\n");
                        exit(1);
                    }
                }
            }

            while (indexIndicator < argc) {

                deletedByte = 0;

                // Check if the file exists and can read from it
                if (access(argv[indexIndicator], R_OK) != 0) {
                    printf("File '%s' is not reachable - Exiting", argv[indexIndicator]);
                    exit(1);
                }

                // Find file name, size, and mode_t
                // create struct for file information
                // use fileStruct.st_size for size data
                // use fileStruct.st_mode for mode data
                stat(argv[indexIndicator], &fileStruct);

                // Find file contents
                FILE *addedFile = fopen(argv[indexIndicator], "r");
                if (addedFile == NULL) {
                    perror("fopen");
                    exit(1);
                }
                char contents[fileStruct.st_size];

                readCheck = fread(contents, sizeof(char), fileStruct.st_size, addedFile);
                if (readCheck < 0) {
                    printf("An error occured while reading - Exiting\n");
                    exit(1);
                }


                // write header but use basename before writing
                strcpy(fileName, basename(argv[indexIndicator]));
                writeHeader(fileName, fileStruct.st_size, deletedByte, fileStruct.st_mode, fp);

                // write file contents
                fwrite(contents, sizeof(char), fileStruct.st_size, fp);

                indexIndicator = indexIndicator + 1;

                fclose(addedFile);
            }

            fclose(fp);

            break;
        case('d'): // Delete a file in the archive file

            // check if the file exists
            if (access(argv[2], W_OK) != 0) {
                printf("Archive file '%s' is not reachable\n", argv[2]);
                exit(1);
            }

            // open the file for ammending
            fp = fopen(argv[2], "r+");
            if (fp == NULL) {
                    perror("fopen");
                    exit(1);
            }

            // Seek to the end of file and store value to find EOF
            fseek(fp, 0, SEEK_END);
            seekEnd = ftell(fp);
            // Return the cursor to the beginning of file
            fseek(fp, 0, SEEK_SET);


            // Check that it is a valid .mtu file
            if (checkMagicBytes(fp) == 0) {
                printf("Valid .mtu file\n");
            }
            else {
                printf("Not a valid .mtu file - Exiting\n");
                exit(1);
            }

            // Look through the file to find all cmd args

            for (indexIndicator = 3; indexIndicator < argc; indexIndicator++) {

                while (ftell(fp) < seekEnd) { // Find all instances of argv

                    // Read the name to determine if correct file
                    readName(fileName, fp);
                    readSize(&fileSize, fp);

                    if (strcmp(argv[indexIndicator], fileName) == 0) {

                        fseek(fp, 0L, SEEK_CUR); // Call this whenever switching between reading and writing

                        // write the deleted byte
                        fwrite(&deletionByte, sizeof(char), 1, fp);

                        fseek(fp, 0L, SEEK_CUR); // Call this whenever switching between reading and writing

                    }
                    else { // skip the deleted byte
                        fseek(fp, 1, SEEK_CUR);
                    }

                    // skip the mode and data
                    fseek(fp, fileSize + 4, SEEK_CUR);

                }

            }

            fclose(fp);

            break;
        case('x'):
            // check if the archive file exists
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

                    readMode(&fileMode, fp);

                    // extract the file
                    if (fileDeleted == 0) {
                        extractFile(fileName, fileSize, fileMode, fp);
                    }
                    else {
                        // if the file is to be deleted, skip ahead
                        printf("FILE: %s is to be deleted, skipping extraction\n", fileName);
                        fseek(fp, fileSize, SEEK_CUR);
                    }

                }

                fclose(fp);

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
            exit(1);

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
