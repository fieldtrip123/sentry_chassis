#ifndef SENTRY_CHASSIS_BSP_CAN_H
#define SENTRY_CHASSIS_BSP_CAN_H


#include "main.h"

typedef struct {

    uint16_t can_id;         //id
    int16_t  set_current;    //设定电流
    uint16_t rotor_angle;    //机械角度
    int16_t  rotor_speed;    //转速
    int16_t  torque_current; //扭矩电流
    uint8_t  temp;           //温度
    int    circle_count;
    int	   target_count;
    int16_t  add_target;
    int16_t  add_encode;
    int16_t  last_encode;
    int16_t  last_target;
}    motor_t;


void can1_init(void);
void can2_init(void);
void set_chassis_3508_v(int16_t v1, int16_t v2, int16_t v3, int16_t v4);
void set_bigyaw_6020_v(int16_t v1);
void send_msg( int16_t tx2c1,int16_t tx2c2,int16_t flag1,int16_t flag2);





#endif //SENTRY_CHASSIS_BSP_CAN_H