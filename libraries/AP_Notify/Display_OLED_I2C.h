#pragma once

#include "Display.h"
#include "Display_SH1106_I2C.h"
#include "Display_SSD1306_I2C.h"

class Display_OLED_I2C: public Display {
public:
    virtual bool hw_init();
    virtual bool hw_update();
    virtual bool set_pixel(uint16_t x, uint16_t y);
    virtual bool clear_pixel(uint16_t x, uint16_t y);
    virtual bool clear_screen();

private:
    AP_HAL::OwnPtr<Display> _display;
    bool _initialized;
};
