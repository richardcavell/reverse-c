/* reverse.c (alternative version)
 * Version 1.3 (30 May 2025)
 *
 * A program by Richard Cavell
 * (c) 2017-2025
 *
 * https://github.com/richardcavell
 */

#include <stdio.h>
#include <stdlib.h>
#include "reverse.h"

/* Increase this if you have enough RAM */
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 3000
#endif

struct buffer
{
    struct buffer * prev;        /* If NULL, then this is the first buffer */
    char text[BUFFER_SIZE];
};

int main(void)
{
    int c;
    struct buffer * current_buffer = NULL;
    char * current_pos = NULL;

    /* Read in every char that is available,
     * allocating a new buffer when necessary */
    while ((c = getchar()) != EOF)
    {
        if (current_buffer == NULL || current_pos == current_buffer->text)
        {
            struct buffer * const next_buffer = malloc(sizeof *next_buffer);

            if (next_buffer == NULL)
            {
                perror("Error: Out of memory");

                while (current_buffer)
                {
                    struct buffer * const prev = current_buffer->prev;
                    free(current_buffer);
                    current_buffer = prev;
                }

                exit(RVRS_FAIL_MALLOC);
            }

            next_buffer->prev = current_buffer;
            current_buffer = next_buffer;
            current_pos = &current_buffer->text[BUFFER_SIZE];
        }

        /* Push the chars into the buffer in reverse order */
        *--current_pos = c;
    }

    /* Output and free every text buffer */
    while (current_buffer)
    {
        struct buffer * const prev = current_buffer->prev;

        while (current_pos < &current_buffer->text[BUFFER_SIZE])
            putchar(*current_pos++);

        free(current_buffer);

        current_buffer = prev;

        if (current_buffer)
            current_pos = current_buffer->text;
    }

    return EXIT_SUCCESS;
}
