#include "io.h"

int main() {
  uart_init();
  uart_writeText("Hello world!\n");
  while (1) {
  }
}
