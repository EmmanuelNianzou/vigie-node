#pragma once
#include <stdbool.h>
typedef enum { INPUT_PIR, INPUT_OBSTACLE, INPUT_BUTTON, INPUT_COUNT } input_id_t;
void hal_inputs_init(void);
void hal_inputs_poll(bool active[INPUT_COUNT]);
