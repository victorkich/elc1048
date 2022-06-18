/*
 * gpio_driver.c
 *
 * Created: 14/06/2022 15:56:45
 *  Author: aluno
 */ 


#include <asf.h>
#include <gpio-driver.h>

struct port_config port;

void gpio_config(uint8_t gpio_num, uint8_t dir) {
	port.direction = dir;
	port_pin_set_config(gpio_num, &port);
}

void gpio_write(uint8_t gpio_num, bool val) {
	port_pin_set_output_level(gpio_num, val);
}

bool gpio_read(uint8_t gpio_num) {
	return port_pin_get_output_level(gpio_num);
}

void gpio_toggle(uint8_t gpio_num) {
	port_pin_toggle_output_level(gpio_num);
}

uint8_t gpio_dir(uint8_t gpio_num) {
	return port.direction;
}
