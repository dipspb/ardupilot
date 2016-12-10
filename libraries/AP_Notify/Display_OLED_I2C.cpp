/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Display_OLED_I2C.h"

#include <utility>

//#include <AP_HAL/AP_HAL.h>
//#include <AP_HAL/I2CDevice.h>

bool Display_OLED_I2C::hw_init()
{
    if (Display_SH1106_I2C::hw_autodetect()) {
        _display = new Display_SH1106_I2C();
    } else {
        _display = new Display_SSD1306_I2C();
    }
    _initialized = true;
    return _display->hw_init();
}

bool Display_OLED_I2C::hw_update()
{
    return _initialized && _display->hw_update();
}

bool Display_OLED_I2C::set_pixel(uint16_t x, uint16_t y)
{
    return _initialized && _display->set_pixel(x, y);
}

bool Display_OLED_I2C::clear_pixel(uint16_t x, uint16_t y)
{
    return _initialized && _display->clear_pixel(x, y);
}
bool Display_OLED_I2C::clear_screen()
{
    return _initialized && _display->hw_init();
}
