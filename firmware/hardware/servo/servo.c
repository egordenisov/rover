#include "servo.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pins.h"
#include <string.h>


typedef struct {
    uint min_lf;
    uint min_rf;
    uint min_lb;
    uint min_rb;

    uint max_lf;
    uint max_rf;
    uint max_lb;
    uint max_rb;

    bool invert_lf;
    bool invert_rf;
    bool invert_lb;
    bool invert_rb;
} rover_servo_ctx_t;

rover_servo_ctx_t servo_ctx = {0};

// Target pwm freq = 50 Hz

void rover_servo_init(){
    memset(&servo_ctx, 0, sizeof(rover_servo_ctx_t));
    servo_ctx.min_lf = 1000;
    servo_ctx.min_rf = 1000;
    servo_ctx.min_lb = 1000;
    servo_ctx.min_rb = 1000;

    servo_ctx.max_lf = 2000;
    servo_ctx.max_rf = 2000;
    servo_ctx.max_lb = 2000;
    servo_ctx.max_rb = 2000;

    servo_ctx.invert_lf = false;
    servo_ctx.invert_rf = false;
    servo_ctx.invert_lb = false;
    servo_ctx.invert_rb = false;

    // Tell GPIO they are allocated to the PWM
    gpio_set_function(SERVO_LF_PIN, GPIO_FUNC_PWM);
    gpio_set_function(SERVO_RF_PIN, GPIO_FUNC_PWM);
    gpio_set_function(SERVO_LB_PIN, GPIO_FUNC_PWM);
    gpio_set_function(SERVO_RB_PIN, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num_lf = pwm_gpio_to_slice_num(SERVO_LF_PIN);
    uint slice_num_rf = pwm_gpio_to_slice_num(SERVO_RF_PIN);
    uint slice_num_lb = pwm_gpio_to_slice_num(SERVO_LB_PIN);
    uint slice_num_rb = pwm_gpio_to_slice_num(SERVO_RB_PIN);

    // This 133 MHz  -> 1 MHz -> 50 Hz
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.f);
    pwm_config_set_wrap(&config, 20000 - 1);

    // Init can be duplicated if slice nums are equals
    pwm_init(slice_num_lf, &config, true);
    pwm_init(slice_num_rf, &config, true);
    pwm_init(slice_num_lb, &config, true);
    pwm_init(slice_num_rb, &config, true);

    pwm_set_gpio_level(SERVO_LF_PIN, (servo_ctx.max_lf + servo_ctx.min_lf) / 2);
    pwm_set_gpio_level(SERVO_RF_PIN, (servo_ctx.max_rf + servo_ctx.min_rf) / 2);
    pwm_set_gpio_level(SERVO_LB_PIN, (servo_ctx.max_lb + servo_ctx.min_lb) / 2);
    pwm_set_gpio_level(SERVO_RB_PIN, (servo_ctx.max_rb + servo_ctx.min_rb) / 2);
}

int rover_set_servo_angle_deg (void* phy_ctx, float lf, float rf, float lb, float rb) {
    if (lf < -180 || lf > 180 || rf < -180 || rf > 180) {
        return -1;
    }
    if (lb < -180 || lb > 180 || rb < -180 || rb > 180) {
        return -1;
    }

    if (servo_ctx.invert_lf){
        lf = -lf;
    }
    if (servo_ctx.invert_rf){
        rf = -rf;
    }
    if (servo_ctx.invert_lb){
        lb = -lb;
    }
    if (servo_ctx.invert_rb){
        rb = -rb;
    }

    uint value_lf = (uint) ((float)(servo_ctx.max_lf - servo_ctx.min_lf)) * ((lf + 180.f) / 360.f) + servo_ctx.min_lf;
    uint value_rf = (uint) ((float)(servo_ctx.max_rf - servo_ctx.min_rf)) * ((rf + 180.f) / 360.f) + servo_ctx.min_rf;
    uint value_lb = (uint) ((float)(servo_ctx.max_lb - servo_ctx.min_lb)) * ((lb + 180.f) / 360.f) + servo_ctx.min_lb;
    uint value_rb = (uint) ((float)(servo_ctx.max_rb - servo_ctx.min_rb)) * ((rb + 180.f) / 360.f) + servo_ctx.min_rb;

    if (value_lf < servo_ctx.min_lf || value_lf > servo_ctx.max_lf ||
        value_rf < servo_ctx.min_rf || value_rf > servo_ctx.max_rf ||
        value_lb < servo_ctx.min_lb || value_lb > servo_ctx.max_lb ||
        value_rb < servo_ctx.min_rb || value_rb > servo_ctx.max_rb ){
        return -2;
    }

    pwm_set_gpio_level(SERVO_LF_PIN, value_lf);
    pwm_set_gpio_level(SERVO_RF_PIN, value_rf);
    pwm_set_gpio_level(SERVO_LB_PIN, value_lb);
    pwm_set_gpio_level(SERVO_RB_PIN, value_rb);

    return 0;
}