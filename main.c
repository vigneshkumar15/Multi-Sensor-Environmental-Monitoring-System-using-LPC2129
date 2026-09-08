#include "header.h"
float temperature;
unsigned int light_value;
unsigned int soil_value;
unsigned int water_status;
void gpio_init(void)
{
    IODIR0 &= ~(1 << 5);
    IODIR0 |= (1 << 17);
    IODIR0 |= (1 << 18);
    IODIR0 |= (1 << 19);
    IOCLR0 = (1 << 17);
    IOCLR0 = (1 << 18);
    IOCLR0 = (1 << 19);
}

void led_indication(void)
{
    IOCLR0 = (1 << 17);
    IOCLR0 = (1 << 18);
    IOCLR0 = (1 << 19);
    if(water_status == 1)
    {
        IOSET0 = 1 << 19;
    }
    else if(temperature >= 40.0)
    {
        IOSET0 = 1 << 19;
    }
    else if(soil_value < 30)
    {
        IOSET0 = 1 << 18;
    }
    else if(light_value < 300)
    {
        IOSET0 = 1 << 18;
    }
    else
    {
        IOSET0 = 1 << 17;
    }
}

void lcd_display(void)
{
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_string("TEMP:");
    lcd_integer((int)temperature);
    lcd_data('C');
    lcd_cmd(0xC0);
    if(water_status == 1)
    {
        lcd_string("Status: ALERT");
    }
    else if(temperature >= 40.0)
    {
        lcd_string("Status: HOT");
    }
    else if(soil_value < 30)
    {
        lcd_string("Status: WATER");
    }
    else if(light_value < 300)
    {
        lcd_string("Status: DARK");
    }
    else
    {
        lcd_string("Status: SAFE");
    }
}

void uart_display(void)
{
    uart0_tx_string("\r\n");
    uart0_tx_string("Temperature : ");
    uart0_integer((int)temperature);
    uart0_tx_string(" C\r\n");
    uart0_tx_string("Light Level : ");
    if(light_value < 300)
        uart0_tx_string("LOW\r\n");
    else
        uart0_tx_string("NORMAL\r\n");
    uart0_tx_string("Soil Moisture : ");
    uart0_integer(soil_value);
    uart0_tx_string(" %\r\n");
    uart0_tx_string("Water Sensor : ");
    if(water_status == 1)
        uart0_tx_string("DETECTED\r\n");
    else
        uart0_tx_string("NOT DETECTED\r\n");
    uart0_tx_string("System Status : ");
    if(water_status == 1)
        uart0_tx_string("ALERT\r\n");
    else if(temperature >= 40.0)
        uart0_tx_string("HOT\r\n");
    else if(soil_value < 30)
        uart0_tx_string("WATER\r\n");
    else if(light_value < 300)
        uart0_tx_string("DARK\r\n");
    else
        uart0_tx_string("SAFE\r\n");
    uart0_tx_string("LED Status : ");
    if(water_status == 1 || temperature >= 40.0)
    {
        uart0_tx_string("RED\r\n");
    }
    else if(soil_value < 30 || light_value < 300)
    {
        uart0_tx_string("YELLOW\r\n");
    }
    else
    {
        uart0_tx_string("GREEN\r\n");
    }
}

int main()
{
    unsigned int temp_adc;
    gpio_init();
    uart0_init(9600);
    adc_init();
    lcd_init();
    lcd_cmd(0x80);
    lcd_string("MULTI SENSOR");
    lcd_cmd(0xC0);
    lcd_string("MONITORING SYS");
    uart0_tx_string("\r\n");
    uart0_tx_string("Multi-Sensor Environmental Monitoring System\r\n");
    delay_sec(2);
    while(1)
    {
        temp_adc = adc_read(1);
        temperature = temperature_conversion(temp_adc);
        light_value = ldr_sensor_read();
        soil_value = soil_sensor_read();
        water_status = water_sensor_read();
        led_indication();
        lcd_display();
        uart_display();
        delay_sec(2);
    }
}
