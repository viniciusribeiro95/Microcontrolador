extern volatile unsigned int rx_flag;
void uart_init(void);

unsigned char uart_getc();
void uart_gets();

void uart_putc(unsigned char c);

void uart_puts(char *str);
