#ifndef HEADER_H
#define HEADER_H

#include <lpc21xx.h>

void delay_ms(unsigned int ms);
void delay_sec(unsigned int sec);

void adc_init(void);
unsigned int adc_read(unsigned int ch_num);

void uart0_init(unsigned int baud);
void uart0_tx(unsigned char data);
void uart0_tx_string(char *ptr);
void uart0_integer(int num);

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_string(char *ptr);
void lcd_integer(int num);

float temperature_conversion(unsigned int adc_value);
unsigned int ldr_sensor_read(void);
unsigned int soil_sensor_read(void);
unsigned int water_sensor_read(void);

void gpio_init(void);
void led_indication(void);
void lcd_display(void);
void uart_display(void);

#endif
