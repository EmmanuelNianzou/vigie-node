#pragma once
bool hal_oled_init(void);
void hal_oled_clear(void);
void hal_oled_print(int line, const char *text); // 6 lignes de 21 caractères
void hal_oled_flush(void);
