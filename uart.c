#include "header.h"
void uart0_init(unsigned int baud)
{
    unsigned int pclk;
    unsigned int result;
    int a[] = {15,60,30,15,0};
    pclk = a[VPBDIV] * 1000000;
    result = pclk / (16 * baud);
    PINSEL0 |= 0x00000005;
    U0LCR = 0x83;
    U0DLL = result & 0xFF;
    U0DLM = (result >> 8) & 0xFF;
    U0LCR = 0x03;
}

void uart0_tx(unsigned char data)
{
    U0THR = data;
    while(((U0LSR >> 5) & 1) == 0);
}

void uart0_tx_string(char *ptr)
{
    while(*ptr != '\0')
    {
        uart0_tx(*ptr);
        ptr++;
    }
}

void uart0_integer(int num)
{
    int a[10];
    int i = 0;
    if(num == 0)
    {
        uart0_tx('0');
        return;
    }
    if(num < 0)
    {
        uart0_tx('-');
        num = -num;
    }
    while(num > 0)
    {
        a[i] = (num % 10) + 48;
        num = num / 10;
        i++;
    }
    for(i = i - 1; i >= 0; i--)
    {
        uart0_tx(a[i]);
    }
}
