#ifndef BLINK_H
#define BLINK_H


#define BLINK_GPIO (gpio_num_t)2
#define LED_BUILTIN 4

void initBlink(void);
void blink_task(void *pvParameter);

#endif // BLINK_H