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

struct buffer
{
    struct buffer *prev;
    char data[BUFFER_SIZE];
};

void free_all_buffers(struct buffer *p)
{
    while (p != NULL)
    {
        struct buffer *prev = p->prev;
        free(p);
        p = prev;
    }
}

void error(const char *s, struct buffer *p)
{
    perror(s);
    free_all_buffers(p);
    exit(EXIT_FAILURE);
}

struct buffer *alloc_buffer(struct buffer *p)
{
    struct buffer *q = malloc(sizeof *q);

    if (q == NULL) /* malloc() failed */
        error("Error while trying to allocate another buffer", p);

    q->prev = p;

    return q;
}

void reverse_buffer(struct buffer *p)
{
    int i, j;

    for (i = 0, j = BUFFER_SIZE - 1;
         j > i;
         ++i, --j)
    {
        char tmp   = p->data[i];
        p->data[i] = p->data[j];
        p->data[j] = tmp;
    }
}

int main(void)
{
    struct buffer *p = NULL;
    size_t nmemb;

    /* Fill the buffers with data and reverse the data */
    do
    {
        p = alloc_buffer(p);

        /* This is faster than a getchar() loop */
        nmemb = fread(p->data, 1, BUFFER_SIZE, stdin);

        if (ferror(stdin))
            error("Error while reading input", p); /* terminates */

        reverse_buffer(p);
    }
    while (nmemb == BUFFER_SIZE);

    /* Output and free the buffers */
    while (p)
    {
        struct buffer *prev = p->prev;

        if (fwrite(&p->data[BUFFER_SIZE-nmemb], 1, nmemb, stdout) < nmemb)
            error("Error while writing output", p); /* terminates */

        free(p);

        p = prev;
        nmemb = BUFFER_SIZE;
    }

    return EXIT_SUCCESS;
}
