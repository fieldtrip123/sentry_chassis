//
// Created by Administrator on 2024/12/13.
//

#ifndef SENTRY_CHASSIS_CHASSIS_H
#define SENTRY_CHASSIS_CHASSIS_H

#include "main.h"
#include "remote.h"
#define  ROOT2  0.707106781
#define  RADIUS 1
#define M3508_REDUCTION_RATIO  19
void speed_solve(Run_Data * run_data);
void  calc_motor_v(int16_t v1,int16_t v2,int16_t v3,int16_t v4);
void chassisTask();







#endif //SENTRY_CHASSIS_CHASSIS_H