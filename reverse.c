/* reverse.c
 *
 * A program by Richard Cavell
 * (c) 2017-2018
 *
 * https://github.com/richardcavell/reverse-c
 */

#include <stdio.h>
#include <stdlib.h>

/* Increase this if you have RAM to spare */
#define BUF 200

struct block
{
    struct block *prev;
    char text[BUF];
};

void free_all_blocks(struct block *p)
{
    while (p != NULL)
    {
        struct block *prev = p->prev;
        free(p);
        p = prev;
    }
}

struct block *alloc_block(struct block *p)
{
    struct block *q = malloc(sizeof *q);

    if (q == NULL) /* malloc() failed */
    {
        fprintf(stderr, "Error: Out of memory\n");

        free_all_blocks(p);

        exit(EXIT_FAILURE);
    }

    q->prev = p;

    return q;
}

void reverse_buffer(struct block *p)
{
    int i, j;

    for (i = 0, j = BUF - 1;
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
    struct block *p = NULL;
    size_t nmemb;

    /* Fill the blocks with text and reverse the text */
    do
    {
        p = alloc_block(p);

        /* This is faster than a getchar() loop */
        nmemb = fread(p->text, 1, BUF, stdin);

        reverse_buffer(p);
    }
    while (nmemb == BUF);

    /* Output and free the blocks */
    while (p)
    {
        struct block *q = p->prev;

        if (fwrite(&p->text[BUF-nmemb], 1, nmemb, stdout) < nmemb)
        {
            fprintf(stderr, "Error while writing output\n");
            free_all_blocks(p);
            return EXIT_FAILURE;
        }

        free(p);

        p = q;
        nmemb = BUF;
    }

    return EXIT_SUCCESS;
}
