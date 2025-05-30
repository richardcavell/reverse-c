/* reverse.c
 * version 1.3 (30 May 2025)
 *
 * Part of reverse-c, which is
 * a program by Richard Cavell
 * (c) 2017-2025
 *
 * https://github.com/richardcavell/reverse-c
 */

#include <stdio.h>
#include <stdlib.h>
#include "reverse.h"

/* Set BUFFER_SIZE to a higher value if you have RAM to spare */
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 3000
#endif

struct buffer
{
    struct buffer * prev;      /* If NULL, then this is the first buffer */
    char data[BUFFER_SIZE];
};

static void free_all_buffers(struct buffer * pbuf)
{
    while (pbuf != NULL)
    {
        struct buffer * const prev = pbuf->prev;
        free(pbuf);
        pbuf = prev;
    }
}

/* Does not return */
static void error(const char * str, struct buffer * pbuf, const int code)
{
  /* The only way we end up in here is if a library call returns
     disappointment. It might be that the library call set errno to
     something informative, but there's no guarantee */

    perror(str);

    free_all_buffers(pbuf);

    /* The operating system receives a failure code */
    exit(code);
}

static struct buffer * alloc_buffer(struct buffer * const pbuf)
{
    struct buffer * const pnew = malloc(sizeof *pnew);

    if (pnew == NULL) /* malloc() failed */
        error("Error while trying to allocate memory", pbuf,
               RVRS_FAIL_MALLOC);

    /* We create a single-linked list, first-in-last-out */
    pnew->prev = pbuf;

    return pnew;
}

static void reverse_buffer(struct buffer * const pbuf)
{
    size_t i, j;

    for (i = 0, j = BUFFER_SIZE - 1;
         j > i;
         ++i, --j)
    {
        const char tmp = pbuf->data[i];
        pbuf->data[i]  = pbuf->data[j];
        pbuf->data[j]  = tmp;
    }
}

int main(void)
{
    struct buffer * pbuf = NULL;
    size_t nmemb;

    /* Fill the buffers with data and reverse the data */
    do
    {
        pbuf = alloc_buffer(pbuf);

        /* This is faster than a getchar() loop */
        nmemb = fread(pbuf->data, sizeof(char), BUFFER_SIZE, stdin);

        if (ferror(stdin))
            error("Error while reading input", pbuf,
                   RVRS_FAIL_INPUT); /* terminates */

        reverse_buffer(pbuf);
    }
    while (nmemb == BUFFER_SIZE);

    /* Output and free the buffers */
    while (pbuf)
    {
        struct buffer * const prev = pbuf->prev;

        if (fwrite(&pbuf->data[BUFFER_SIZE-nmemb],
                   sizeof(char), nmemb, stdout) < nmemb)
            error("Error while writing output", pbuf,
                   RVRS_FAIL_OUTPUT); /* terminates */

        free(pbuf);

        pbuf = prev;
        nmemb = BUFFER_SIZE;
    }

    return EXIT_SUCCESS;
}
