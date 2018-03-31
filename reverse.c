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
#include "reverse.h"

/* Set BUFFER_SIZE to a higher value if you have RAM to spare */
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 200
#endif

struct buffer
{
    struct buffer *prev;
    char data[BUFFER_SIZE];
};

void free_all_buffers(struct buffer *pbuf)
{
    while (pbuf != NULL)
    {
        struct buffer *prev = pbuf->prev;
        free(pbuf);
        pbuf = prev;
    }
}

  /* Does not return */
void error(const char *str, struct buffer *pbuf, const int code)
{
    perror(str);
    free_all_buffers(pbuf);

    /* The operating system receives a fail code */
    exit(code);
}

struct buffer *alloc_buffer(struct buffer *pbuf)
{
    struct buffer *pnew = malloc(sizeof *pnew);

    if (pnew == NULL) /* malloc() failed */
        error("Error while trying to allocate memory", pbuf,
               RVRS_FAIL_MALLOC);

    pnew->prev = pbuf;

    return pnew;
}

void reverse_buffer(struct buffer *pbuf)
{
    size_t i, j;

    for (i = 0, j = BUFFER_SIZE - 1;
         j > i;
         ++i, --j)
    {
        char tmp      = pbuf->data[i];
        pbuf->data[i] = pbuf->data[j];
        pbuf->data[j] = tmp;
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
            error("Error while reading input", p,
                   RVRS_FAIL_INPUT); /* terminates */

        reverse_buffer(p);
    }
    while (nmemb == BUFFER_SIZE);

    /* Output and free the buffers */
    while (p)
    {
        struct buffer *prev = p->prev;

        if (fwrite(&p->data[BUFFER_SIZE-nmemb], 1, nmemb, stdout) < nmemb)
            error("Error while writing output", p,
                   RVRS_FAIL_OUTPUT); /* terminates */

        free(p);

        p = prev;
        nmemb = BUFFER_SIZE;
    }

    return EXIT_SUCCESS;
}
