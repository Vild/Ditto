#include <stdio.h>
#include <stdlib.h>
#include "../lib/Ditto/ditto.h"

int main(int argc, char **argv) {
  printf("Calling ditto_init()\n");
  ditto_init();
  printf("Calling ditto_destory()\n");
  ditto_destoy();
  return 0;
}
