/* reverse.c
 * version 1.0 (24 March 2018)
 *
 * A program by Richard Cavell
 * (c) 2017-2018
 *
 * https://github.com/richardcavell/reverse-c
 */

#include <stdio.h>
#include <stdlib.h>

/* Increase this if you have RAM to spare */
#define BUFFER_SIZE 200

struct text_buffer
{
    struct text_buffer *prev;
    char text[BUFFER_SIZE];
};

void free_all_text_buffers(struct text_buffer *p)
{
    while (p != NULL)
    {
        struct text_buffer *prev = p->prev;
        free(p);
        p = prev;
    }
}

struct text_buffer *alloc_text_buffer(struct text_buffer *p)
{
    struct text_buffer *q = malloc(sizeof *q);

    if (q == NULL) /* malloc() failed */
    {
        fprintf(stderr, "Error: Out of memory\n");

        free_all_text_buffers(p);

        exit(EXIT_FAILURE);
    }

    q->prev = p;

    return q;
}

void reverse_buffer(struct text_buffer *p)
{
    int i, j;

    for (i = 0, j = BUFFER_SIZE - 1;
         j > i;
         ++i, --j)
    {
        char tmp   = p->text[i];
        p->text[i] = p->text[j];
        p->text[j] = tmp;
    }
}

int main(void)
{
    struct text_buffer *p = NULL;
    size_t nmemb;

    /* Fill the text_buffers with text and reverse the text */
    do
    {
        p = alloc_text_buffer(p);

        /* This is faster than a getchar() loop */
        nmemb = fread(p->text, 1, BUFFER_SIZE, stdin);

        if (ferror(stdin))
        {
            fprintf(stderr, "Error while reading input\n");
            free_all_text_buffers(p);
            return EXIT_FAILURE;
        }

        reverse_buffer(p);
    }
    while (nmemb == BUFFER_SIZE);

    /* Output and free the text_buffers */
    while (p)
    {
        struct text_buffer *prev = p->prev;

        if (fwrite(&p->text[BUFFER_SIZE-nmemb], 1, nmemb, stdout) < nmemb)
        {
            fprintf(stderr, "Error while writing output\n");
            free_all_text_buffers(p);
            return EXIT_FAILURE;
        }

        free(p);

        p = prev;
        nmemb = BUFFER_SIZE;
    }

    return EXIT_SUCCESS;
}
