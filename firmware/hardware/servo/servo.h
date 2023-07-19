#pragma once

void rover_servo_init();
int rover_set_servo_angle_deg (void* phy_ctx, float lf, float rf, float lb, float rb);