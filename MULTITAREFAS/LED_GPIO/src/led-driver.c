/*
 * led_driver.c
 *
 * Created: 09/06/2022 16:36:58
 *  Author: aluno
 */ 

#include <asf.h>
#include <led-driver.h>

bool ledValue;

void led_config() {
	ledValue = false;
}

void led_on(uint8_t led_num) {
	ledValue = false;
	port_pin_set_output_level(led_num, ledValue);
}

void led_off(uint8_t led_num) {
	ledValue = true;
	port_pin_set_output_level(led_num, ledValue);
}

void led_toggle(uint8_t led_num) {
	ledValue = !ledValue;
	port_pin_set_output_level(led_num, ledValue);
}