/*
 * gpio_driver.h
 *
 * Created: 14/06/2022 15:56:22
 *  Author: aluno
 */ 

void gpio_config(uint8_t gpio_num, uint8_t dir);
void gpio_write(uint8_t gpio_num, bool val);
bool gpio_read(uint8_t gpio_num);
void gpio_toggle(uint8_t gpio_num);
uint8_t gpio_dir(uint8_t gpio_num);