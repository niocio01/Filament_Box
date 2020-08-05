#ifndef __INPUT_H__
#define __INPUT_H__

#define ENC_PIN_CLK 36 // In Phase B
#define ENC_PIN_DT 37 // In Phase A
#define ENC_PIN_SW 38 // In Switch

void initEncoder(void);
void input_task(void *pvParameter);

int readEncMovement(void);
int8_t read_rotary(void);

bool readEncButton(void);

#endif // __INPUT_H__