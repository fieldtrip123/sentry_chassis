//
// Created by Administrator on 2024/12/13.
//

#include "../Inc/chassis.h"
#include "pid.h"
#include "bsp_can.h"
Run_Data  run_data;
int16_t target_speed[4];
motor_t chassis_3508_info[4];
extern pid_struct_t  chassis_3508_pid[4];

void chassisTask()
{

    speed_solve(&run_data);

}

void speed_solve(Run_Data * run_data)
{
    int16_t v1,v2,v3,v4,vx,vy,vw;
    vx=run_data->Vx;
    vy=run_data->Vy;
    vw=run_data->Vw;
    v1=-ROOT2*vx+ROOT2*vy+vw*RADIUS;
    v2=-ROOT2*vx-ROOT2*vy+vw*RADIUS;
    v3= ROOT2*vx-ROOT2*vy+vw*RADIUS;
    v4= ROOT2*vx+ROOT2*vy+vw*RADIUS;
    calc_motor_v(v1,v2,v3,v4);
}

void  calc_motor_v(int16_t v1,int16_t v2,int16_t v3,int16_t v4)
{
    target_speed[0]=v1*M3508_REDUCTION_RATIO;
    target_speed[1]=v2*M3508_REDUCTION_RATIO;
    target_speed[2]=v3*M3508_REDUCTION_RATIO;
    target_speed[3]=v4*M3508_REDUCTION_RATIO;

    for(int i=0;i<4;i++)
    {
          chassis_3508_info[i].set_current=pid_calc(&chassis_3508_pid[i], target_speed[i], chassis_3508_info[i].rotor_speed);
    }
    //  4个电机赋速度值
    set_chassis_3508_v(
            chassis_3508_info[0].set_current,
            chassis_3508_info[1].set_current,
            chassis_3508_info[2].set_current,
            chassis_3508_info[3].set_current
    );
}
