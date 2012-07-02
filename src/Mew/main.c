#include <stdio.h>
#include <stdlib.h>
#include "../lib/Ditto/ditto.h"

int main(int argc, char ** argv)
{
  printf("Calling ditto_init()\n");
  if (ditto_init() != 0)
    exit(-1);
  printf("Calling ditto_setTitle()\n");
  ditto_setTitle("Mew - Running the Ditto engine");
  printf("Calling ditto_loop()\n");
  ditto_loop();
  printf("Calling ditto_destory()\n");
  ditto_destoy();
  return 0;
}
