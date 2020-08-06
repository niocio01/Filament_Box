#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "GUI/input.h"

int8_t direction = 0;
static uint8_t prevNextCode = 0;
static uint16_t store = 0;
bool buttonState = false;

void initEncoder(void)
{
    pinMode(ENC_PIN_CLK, INPUT);
    pinMode(ENC_PIN_DT, INPUT);
    pinMode(ENC_PIN_SW, INPUT);
    pinMode(ENC_PIN_SW, INPUT_PULLUP);

    gpio_pad_select_gpio(GPIO_NUM_38);
    gpio_set_direction(GPIO_NUM_38, GPIO_MODE_INPUT);
    //gpio_set_pull_mode(GPIO_NUM_38, GPIO_PULLUP_ONLY);
    //gpio_pullup_en(GPIO_NUM_38);

    // attachInterrupt(ENC_PIN_CLK, pinA_ISR, CHANGE);

    xTaskCreate(input_task, "input_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}

void input_task(void *pvParameter)
{
    while (1)
    {
        static int8_t val;

        if ((val = read_rotary()))
        {
            direction += val;
        }

        buttonState = gpio_get_level(GPIO_NUM_38);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary(void)
{
    static int8_t rot_enc_table[] = {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};

    prevNextCode <<= 2;
    if (digitalRead(ENC_PIN_DT))
        prevNextCode |= 0x02;
    if (digitalRead(ENC_PIN_CLK))
        prevNextCode |= 0x01;
    prevNextCode &= 0x0f;

    // If valid then store as 16 bit data.
    if (rot_enc_table[prevNextCode])
    {
        store <<= 4;
        store |= prevNextCode;
        //if (store==0xd42b) return 1;
        //if (store==0xe817) return -1;
        if ((store & 0xff) == 0x2b)
            return -1;
        if ((store & 0xff) == 0x17)
            return 1;
    }
    return 0;
}

int readEncMovement(void)
{
    int temp = direction;
    direction = 0;
    return temp;
}

bool readEncButton(void)
{
    return (!buttonState);
}
