/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AP_HAL/AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_F4BY

#include "GPIO.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* F4BY headers */
#include <drivers/drv_led.h>
#include <drivers/drv_tone_alarm.h>
#include <drivers/drv_gpio.h>
#include <modules/px4iofirmware/protocol.h>
#include <arch/board/board.h>
#include <board_config.h>

#define LOW     0
#define HIGH    1

extern const AP_HAL::HAL& hal;

using namespace F4BY;

F4BYGPIO::F4BYGPIO()
{}

void F4BYGPIO::init()
{
    _led_fd = open(LED0_DEVICE_PATH, O_RDWR);
    if (_led_fd == -1) {
        AP_HAL::panic("Unable to open " LED0_DEVICE_PATH);
    }
    if (ioctl(_led_fd, LED_OFF, LED_BLUE) != 0) {
        hal.console->printf("GPIO: Unable to setup GPIO LED BLUE\n");
    }
    if (ioctl(_led_fd, LED_OFF, LED_RED) != 0) {
         hal.console->printf("GPIO: Unable to setup GPIO LED RED\n");
    }

    _tone_alarm_fd = open(TONEALARM0_DEVICE_PATH, O_WRONLY);
    if (_tone_alarm_fd == -1) {
        AP_HAL::panic("Unable to open" TONEALARM0_DEVICE_PATH);
    }

    _gpio_fmu_fd = open(F4BY_DEVICE_PATH, 0);
    if (_gpio_fmu_fd == -1) {
    	AP_HAL::panic("Unable to open GPIO");
    }
}

void F4BYGPIO::pinMode(uint8_t pin, uint8_t output)
{
    switch (pin) {
    case F4BY_GPIO_D1_PIN:
		ioctl(_gpio_fmu_fd, output?GPIO_SET_OUTPUT:GPIO_SET_INPUT, GPIO_EXT_1);
		break;
	case F4BY_GPIO_D2_PIN:
		ioctl(_gpio_fmu_fd, output?GPIO_SET_OUTPUT:GPIO_SET_INPUT, GPIO_EXT_2);
		break;
	case F4BY_GPIO_D3_PIN:
		ioctl(_gpio_fmu_fd, output?GPIO_SET_OUTPUT:GPIO_SET_INPUT, GPIO_EXT_3);
		break;
    }
}

int8_t F4BYGPIO::analogPinToDigitalPin(uint8_t pin)
{
    switch (pin) {
    default: 
    	{
    	}
    }
    return -1;
}


uint8_t F4BYGPIO::read(uint8_t pin) {

    switch (pin) {

#ifdef GPIO_EXT_1
        case F4BY_GPIO_D1_PIN:{
            uint32_t relays = 0;
            ioctl(_gpio_fmu_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & GPIO_EXT_1)?HIGH:LOW;
        }
#endif

#ifdef GPIO_EXT_2
        case F4BY_GPIO_D2_PIN:{
            uint32_t relays = 0;
            ioctl(_gpio_fmu_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & GPIO_EXT_2)?HIGH:LOW;
        }
#endif

#ifdef GPIO_EXT_3
        case F4BY_GPIO_D3_PIN:{
            uint32_t relays = 0;
            ioctl(_gpio_fmu_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & GPIO_EXT_3)?HIGH:LOW;
        }
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_POWER1
        case F4BY_GPIO_EXT_IO_RELAY1_PIN: {
            uint32_t relays = 0;
            ioctl(_gpio_io_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & F4BYIO_P_SETUP_RELAYS_POWER1)?HIGH:LOW;
        }
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_POWER2
        case F4BY_GPIO_EXT_IO_RELAY2_PIN: {
            uint32_t relays = 0;
            ioctl(_gpio_io_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & F4BYIO_P_SETUP_RELAYS_POWER2)?HIGH:LOW;
        }
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_ACC1
        case F4BY_GPIO_EXT_IO_ACC1_PIN: {
            uint32_t relays = 0;
            ioctl(_gpio_io_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & F4BYIO_P_SETUP_RELAYS_ACC1)?HIGH:LOW;
        }
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_ACC2
        case F4BY_GPIO_EXT_IO_ACC2_PIN: {
            uint32_t relays = 0;
            ioctl(_gpio_io_fd, GPIO_GET, (unsigned long)&relays);
            return (relays & F4BYIO_P_SETUP_RELAYS_ACC2)?HIGH:LOW;
        }
#endif

    case F4BY_GPIO_FMU_SERVO_PIN(0) ... F4BY_GPIO_FMU_SERVO_PIN(7): {
        uint32_t relays = 0;
        ioctl(_gpio_fmu_fd, GPIO_GET, (unsigned long)&relays);
        return (relays & (1U<<(pin-F4BY_GPIO_FMU_SERVO_PIN(0))))?HIGH:LOW;
    }
    }
    return LOW;
}

void F4BYGPIO::write(uint8_t pin, uint8_t value)
{
    switch (pin) {

#ifdef CONFIG_ARCH_BOARD_F4BY
        case HAL_GPIO_A_LED_PIN:    // Arming LED
            if (value == LOW) {
                ioctl(_led_fd, LED_ON, LED_RED);
            } else {
                ioctl(_led_fd, LED_OFF, LED_RED);
            }
            break;

        

        case HAL_GPIO_C_LED_PIN:    // GPS LED 
            if (value == LOW) { 
                ioctl(_led_fd, LED_ON, LED_BLUE);
            } else { 
                ioctl(_led_fd, LED_OFF, LED_BLUE);
            }
            break;

          
#endif

        case F4BY_GPIO_PIEZO_PIN:    // Piezo beeper 
            if (value == LOW) { // this is inverted 
                ioctl(_tone_alarm_fd, TONE_SET_ALARM, 3);    // Alarm on !! 
                //::write(_tone_alarm_fd, &user_tune, sizeof(user_tune));
            } else { 
                ioctl(_tone_alarm_fd, TONE_SET_ALARM, 0);    // Alarm off !! 
            }
            break;

#ifdef GPIO_EXT_1
        case F4BY_GPIO_D1_PIN:
            ioctl(_gpio_fmu_fd, value==LOW?GPIO_CLEAR:GPIO_SET, GPIO_EXT_1);
            break;
#endif

#ifdef GPIO_EXT_2
        case F4BY_GPIO_D2_PIN:
            ioctl(_gpio_fmu_fd, value==LOW?GPIO_CLEAR:GPIO_SET, GPIO_EXT_2);
            break;
#endif

#ifdef GPIO_EXT_3
        case F4BY_GPIO_D3_PIN:
            ioctl(_gpio_fmu_fd, value==LOW?GPIO_CLEAR:GPIO_SET, GPIO_EXT_3);
            break;
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_POWER1
        case F4BY_GPIO_EXT_IO_RELAY1_PIN:
            ioctl(_gpio_io_fd, value==LOW?GPIO_CLEAR:GPIO_SET, F4BYIO_P_SETUP_RELAYS_POWER1);
            break;
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_POWER2
        case F4BY_GPIO_EXT_IO_RELAY2_PIN:
            ioctl(_gpio_io_fd, value==LOW?GPIO_CLEAR:GPIO_SET, F4BYIO_P_SETUP_RELAYS_POWER2);
            break;
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_ACC1
        case F4BY_GPIO_EXT_IO_ACC1_PIN:
            ioctl(_gpio_io_fd, value==LOW?GPIO_CLEAR:GPIO_SET, F4BYIO_P_SETUP_RELAYS_ACC1);
            break;
#endif

#ifdef F4BYIO_P_SETUP_RELAYS_ACC2
        case F4BY_GPIO_EXT_IO_ACC2_PIN:
            ioctl(_gpio_io_fd, value==LOW?GPIO_CLEAR:GPIO_SET, F4BYIO_P_SETUP_RELAYS_ACC2);
            break;
#endif

    case F4BY_GPIO_FMU_SERVO_PIN(0) ... F4BY_GPIO_FMU_SERVO_PIN(7):
        ioctl(_gpio_fmu_fd, value==LOW?GPIO_CLEAR:GPIO_SET, 1U<<(pin-F4BY_GPIO_FMU_SERVO_PIN(0)));
        break;
    }
}

void F4BYGPIO::toggle(uint8_t pin)
{
    write(pin, !read(pin));
}

/* Alternative interface: */
AP_HAL::DigitalSource* F4BYGPIO::channel(uint16_t n) {
    return new F4BYDigitalSource(0);
}

/* Interrupt interface: */
bool F4BYGPIO::attach_interrupt(uint8_t interrupt_num, AP_HAL::Proc p, uint8_t mode)
{
    return true;
}

/*
  return true when USB connected
 */
bool F4BYGPIO::usb_connected(void)
{
    /*
      we use a combination of voltage on the USB connector and the
      open of the /dev/ttyACM0 character device. This copes with
      systems where the VBUS may go high even with no USB connected
      (such as AUAV-X2)
     */
    return stm32_gpioread(GPIO_OTGFS_VBUS) && _usb_connected;
}


F4BYDigitalSource::F4BYDigitalSource(uint8_t v) :
    _v(v)
{}

void F4BYDigitalSource::mode(uint8_t output)
{}

uint8_t F4BYDigitalSource::read() {
    return _v;
}

void F4BYDigitalSource::write(uint8_t value) {
    _v = value;
}

void F4BYDigitalSource::toggle() {
    _v = !_v;
}

#endif // CONFIG_HAL_BOARD
