#define _POSIX_C_SOURCE 200809L

#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "utils/strings/strings.h"

#define PATH_DELIM "/"

// Simple function to add a word in last position of a string array
static char **add_word(char *word, char **can_path_array, size_t *len)
{
    can_path_array = realloc(can_path_array, (*len + 1) * sizeof(char *));
    can_path_array[*len] = strdup(word);
    *len += 1;

    return can_path_array;
}

// Simple function to pop last word of an array
static char **pop_word(char **can_path_array, size_t *len)
{
    if (*len == 0)
    {
        // nothing to pop
        return can_path_array;
    }

    // free last word
    free(can_path_array[*len - 1]);

    // reduice array size
    can_path_array = realloc(can_path_array, (*len - 1) * sizeof(char *));
    *len -= 1;

    return can_path_array;
}

// Build the canonical path
static char **build_path(char *path, char **can_path_array, size_t *len)
{
    char *file = strtok(path, PATH_DELIM);

    while (file != NULL)
    {
        if (strcmp(file, "..") == 0)
        {
            // case .. pop last word
            can_path_array = pop_word(can_path_array, len);
        }
        else if (strcmp(file, ".") != 0)
        {
            // case != .
            can_path_array = add_word(file, can_path_array, len);
        }

        // next word
        file = strtok(NULL, PATH_DELIM);
    }

    return can_path_array;
}

char *my_realpath(char *path)
{
    if (path == NULL)
    {
        return NULL;
    }

    // check if dir exist
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        return NULL;
    }
    closedir(dir);

    // build the canonical path and store dir in an array
    char **can_path_array = NULL;
    size_t len = 0;
    can_path_array = build_path(path, can_path_array, &len);

    // concat array in a single string, allocated in heap
    char *can_path = NULL;
    if (path[0] == '/')
    {
        // add root slash
        my_strcat(&can_path, "/");
    }
    for (size_t i = 0; i < len; ++i)
    {
        my_strcat(&can_path, can_path_array[i]);
        free(can_path_array[i]);
        if (i < len - 1)
        {
            my_strcat(&can_path, "/");
        }
    }
    free(can_path_array);

    return can_path;
}
