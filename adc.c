#include "header.h"

#define DONE ((ADDR >> 31) & 1)

void adc_init(void)
{
    PINSEL1 = 0x15400000;
    ADCR = 0x00200400;
}

unsigned int adc_read(unsigned int ch_num)
{
    unsigned int result = 0;

    ADCR |= (1 << ch_num);
    ADCR |= (1 << 24);

    while(DONE == 0);

    ADCR &= ~(1 << 24);
    ADCR &= ~(1 << ch_num);

    result = (ADDR >> 6) & 0x3FF;

    return result;
}

float temperature_conversion(unsigned int adc_value)
{
    float voltage;
    float temperature;

    voltage = (adc_value * 3.3) / 1023.0;

    temperature = voltage * 100.0;

    return temperature;
}

unsigned int ldr_sensor_read(void)
{
    return adc_read(2);
}

unsigned int soil_sensor_read(void)
{
    unsigned int adc_value;
    unsigned int moisture;

    adc_value = adc_read(3);

    moisture = (adc_value * 100) / 1023;

    return moisture;
}

unsigned int water_sensor_read(void)
{
    if(IOPIN0 & (1 << 5))
        return 1;
    else
        return 0;
}
