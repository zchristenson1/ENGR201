#ifndef UART_FR5969UART_H_
#define UART_FR5969UART_H_

#define comma (0)
#define space (1)
#define LFCR (2)

void uart_init(void);
void uart_print(char *c_str);
void whoami();
void uart_data(unsigned int num, char mark);


#endif /* UART_FR5969UART_H_ */
