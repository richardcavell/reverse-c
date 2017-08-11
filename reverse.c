/* reverse.c
 *
 * A program by Richard Cavell
 * (c) 2017
 */

#include <stdio.h>
#include <stdlib.h>

#define BUF 1

struct block
{
  struct block *prev;
  char text[BUF];
};

int main(void)
{
  int c;
  struct block *p = NULL;
  char *t = NULL;

  while ((c = getchar()) != EOF)
  {
    if (p == NULL || t == p->text)
    {
      struct block *q = malloc(sizeof *q);

      if (q == NULL)
      {
        fprintf(stderr, "Error: Out of memory\n");
        exit(EXIT_FAILURE);
      }

      q->prev = p;
      p = q;
      t = &p->text[BUF];
    }

    *--t = c;
  }

  while (p)
  {
    struct block *q = p->prev;

    while (t < &p->text[BUF])
      putchar(*t++);

    free(p);

    p = q;

    if (p)
      t = p->text;
  }

  return EXIT_SUCCESS;
}
