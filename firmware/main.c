#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "pins.h"

#include "wheel4.h"
#include "hardware/servo/servo.h"
#include "hardware/hardware.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

typedef struct {
    wheel4_ctx_t wheel4;
} rover_t;

rover_t rover = {0};

int main (){

    stdio_init_all();

    rover_hardware_init();

    wheel4_init_struct_t w4_init = {
        .set_motors = NULL,
        .motors_ctx = NULL,

        .set_servos = rover_set_servo_angle_deg,
        .servos_ctx = NULL,

        .get_us = time_us_64,

        .wheel_base_m = 0.4f,
        .track_gauge_m = 0.2f,
    };

    wheel4_init(&rover.wheel4, &w4_init);

    float i = 0.f;
    bool up = true;


    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(1000);
    pwm_set_gpio_level(SERVO_LF_PIN, 1000);
    // sleep_ms(1000);
    // pwm_set_gpio_level(https://www.google.com/chromeSERVO_LF_PIN, 700);
    // sleep_ms(1000);
    // pwm_set_gpio_level(SERVO_LF_PIN, 800);

        // if (up){
        //     i += 1.f;
        //     if (i > 180.f) {
        //         i = 180.f;
        //         up = false;
        //     }
        // }
        // else {
        //     i -= 1.f;
        //     if (i < -180.f) {
        //         i = 0.f;
        //         up = true;
        //     }
        // }
        // sleep_ms(200);

    while(1){

    }
}