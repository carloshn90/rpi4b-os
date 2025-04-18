#define PERIPHERAL_BASE 0xFE000000

void uart_init();
void uart_writeText(char* buffer);
void uart_update();

void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);
