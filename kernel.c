#include "fb.h"
#include "io.h"

int main() {
  /* uart_init(); */
  fb_init();

  /* uart_writeText("Hello world!\n"); */
  drawString(100, 100, "Hello world!", 0x0f);
  while (1) {
    /* uart_update(); */
  }
}
