#include <stdio.h>
#include <ctype.h>

/* I'm a terrible speller, but not this terrible.  For students who were *
 * not in class, some of these misspellings (like the 'e' on "potato")   *
 * were added for comedic effect.                                        *
 *                                                                       *
 * And, of course, some of them are just my terrible spelling.           */
char *words[] = {
  "antilope",
  "badger",
  "crocodile",
  "donkey",
  "elephant",
  "fork",
  "girraffe",
  "hedgehog",
  "ice cream",
  "jackelope",
  "kangeroo",
  "lima",
  "monkey",
  "newt",
  "octopus",
  "potatoe",
  "quail",
  "rat",
  "shrubbery",
  "tomatoe",
  "ukelale",
  "violin",
  "wumbo",
  "x86",
  "yellow",
  "zenith",
};

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Please supply a letter!\n");
    return 1;
  }

  printf("%s begins with %c\n", words[toupper(argv[1][0]) - 'A'], argv[1][0]);

  return 0;
}
