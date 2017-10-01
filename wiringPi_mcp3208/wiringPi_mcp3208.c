/* gpio - Pi gpio pins via /sys/etc */
/* see http://elinux.org/RPi_Low-level_peripherals */

/* Malte Steiner 2017 */

#include "m_pd.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h>

#define ADC_SPI_CHANNEL 0
//#define ADC_SPI_SPEED 500000
#define ADC_SPI_SPEED 1000000
#define ADC_NUM_CHANNELS 8

static t_class *wiringPi_mcp3208_class;

typedef struct _wiringPi_mcp3208 {
    t_object x_obj;
    t_outlet *x_out1;
    int x_adc_pin;
    int x_spi_pin;
} t_wiringPi_mcp3208;

uint16_t readADC(int _channel){
	uint8_t spi_data[3];
	uint8_t input_mode = 1; // single ended = 1, differential = 0
	uint16_t result;

	if(_channel > 7 | _channel < 0){
		return -1;
	}

	spi_data[0] = 0x04; // start flag
	spi_data[0] |= (input_mode<<1); // shift input_mode
	spi_data[0] |= (_channel>>2) & 0x01; // add msb of channel in our first command byte

	spi_data[1] = _channel<<6;
	spi_data[2] = 0x00;

	wiringPiSPIDataRW(ADC_SPI_CHANNEL, spi_data, 3);

	result = ((spi_data[1] & 0x0f)<<8) | spi_data[2];
	return result;
}

static void wiringPi_mcp3208_bang(t_wiringPi_mcp3208 *x) {
    int value, i;
    t_atom readings[x->x_adc_pin];
    for (i=0; i<ADC_NUM_CHANNELS; i++) {
		value = readADC(i);
		SETFLOAT(&readings[i], value);
    }
    
    outlet_list(x->x_out1, 0, x->x_adc_pin, readings);
//    outlet_float(x->x_out1, value);
}

static void *wiringPi_mcp3208_new(t_floatarg f, t_floatarg g)
{
    t_wiringPi_mcp3208 *x = (t_wiringPi_mcp3208 *)pd_new(wiringPi_mcp3208_class);
    x->x_out1 = outlet_new(&x->x_obj, gensym("list"));
    post("Using SPI port %f and reading %f ADCs", f, g);
    x->x_spi_pin = (int) f;
    x->x_adc_pin = (int) g;
    return (x);
}

void wiringPi_mcp3208_setup_wiringPi(t_wiringPi_mcp3208 *x)
{
    wiringPiSetupSys();
    wiringPiSPISetup(ADC_SPI_CHANNEL, ADC_SPI_SPEED);
    post("mcp_3208 is ready to read!");
}


void wiringPi_mcp3208_setup(void)
{
    wiringPi_mcp3208_class = class_new(gensym("wiringPi_mcp3208"), 
		(t_newmethod)wiringPi_mcp3208_new, 
		0, sizeof(t_wiringPi_mcp3208), 
		0, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addbang(wiringPi_mcp3208_class, wiringPi_mcp3208_bang);
    class_addmethod(wiringPi_mcp3208_class, (t_method)wiringPi_mcp3208_setup_wiringPi, gensym("setup_wiringPi"), 0);
    post("wiringPi_mcp3208 version 0.01");
}
