/* reverse.c (alternative version)
 *
 * A program by Richard Cavell
 * (c) 2017-2018
 *
 * https://github.com/richardcavell
 */

#include <stdio.h>
#include <stdlib.h>

/* Increase this if you have enough RAM */
#define BUFFER_SIZE 200

struct text_buffer
{
    struct text_buffer *prev;
    char text[BUFFER_SIZE];
};

int main(void)
{
    int c;
    struct text_buffer *current_buffer = NULL;
    char *current_pos = NULL;

    /* Read in every char that is available, reversing as we go */
    while ((c = getchar()) != EOF)
    {
        if (current_buffer == NULL || current_pos == current_buffer->text)
        {
            struct text_buffer *next_buffer = malloc(sizeof *next_buffer);

            if (next_buffer == NULL)
            {
                fprintf(stderr, "Error: Out of memory\n");

                while (current_buffer)
                {
                    struct text_buffer *prev = current_buffer->prev;
                    free(current_buffer);
                    current_buffer = prev;
                }

                exit(EXIT_FAILURE);
            }

            next_buffer->prev = current_buffer;
            current_buffer = next_buffer;
            current_pos = &current_buffer->text[BUFFER_SIZE];
        }

        *--current_pos = c;
    }

    /* Output and free every text buffer */
    while (current_buffer)
    {
        struct text_buffer *prev = current_buffer->prev;

        while (current_pos < &current_buffer->text[BUFFER_SIZE])
            putchar(*current_pos++);

        free(current_buffer);

        current_buffer = prev;

        if (current_buffer)
            current_pos = current_buffer->text;
    }

    return EXIT_SUCCESS;
}
