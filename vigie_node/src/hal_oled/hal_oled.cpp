#include "hal_oled.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "../app_config.h"

// Image décalée de 2 colonnes
static U8G2_SSD1306_128X64_NONAME_F_HW_I2C s_u8g2(U8G2_R0, U8X8_PIN_NONE);

bool hal_oled_init(void)
{
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    if (!s_u8g2.begin()) {
        return false;
    }
    s_u8g2.setFont(u8g2_font_6x10_tf);
    return true;
}

void hal_oled_clear(void) { s_u8g2.clearBuffer(); }
void hal_oled_print(int line, const char *text) { s_u8g2.drawStr(0, 10 * (line + 1), text); }
void hal_oled_flush(void) { s_u8g2.sendBuffer(); }
