#ifndef MYLS_H
#define MYLS_H

#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

/**
 * @brief `int` Compares two elements for sorting in alphabetical order.
 *
 * @param a `const void *` a string
 * @param b `const void *` a string
 * @return 0 if a and b are equal, negative if a < b, and positive if a > b
 */
int compare(const void *a, const void *b);

/**
 * @brief `const char *` Transforms the permission ID provided by the stat struct
 *        into a string representation (rwx).
 *
 * @param mode `mode_t` file access permissions
 * @return the permission string (rwx)
 */
const char *get_permissions_string(mode_t mode);

/**
 * @brief `void` Displays the result in 'ls -l' format.
 *
 * @param totalBlocks `int` total block size of all files and directories in the current directory
 * @param num_files `int` number of files in the current directory
 * @param file_names `char **` array of strings containing names of files and directories in the current directory
 * @param st `struct stat` struct stat for file information
 */
void affiche(int totalBlocks, int num_files, char **file_names, struct stat st);

/**
 * @brief `void` Opens the current directory and uses the previous functions.
 *
 * @param cwd `char *` current directory path
 * @param num_files `int *` number of files in the current directory
 * @param max_files `int *` maximum size of the file_names array
 * @param file_names `char **` array of strings containing names of files and directories in the current directory
 * @param hasA `int` flag for option -a
 * @param hasR `int` flag for option -R
 */
void files(char *cwd, int *num_files, int *max_files, char **file_names, int hasA, int hasR);

/**
 * @brief `hasOption` Determines which options were entered by the user
 *        and modifies the corresponding variables.
 *
 * @param argc `int` number of elements in argv (main parameter)
 * @param argv `char **` array where potential options entered by the user are retrieved (main parameter)
 * @param hasA `int *` variable linked to option -a
 * @param hasR `int *` variable linked to option -R
 */
void hasOption(int argc, char **argv, int *hasA, int *hasR);

#endif // MYLS_H
