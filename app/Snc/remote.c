//
// Created by Administrator on 2024/12/13.
//
//
// Created by Administrator on 2024/12/3.
//

#include "remote.h"
#include "main.h"
#include "usart.h"



extern Run_Data  run_data;
RC_Ctrl RC_CtrlData;

 uint8_t UART_RECV[512];



void remoteTask()
{
    RC_Robot_Ctrl();
}

void RemoteDataProcess(volatile uint8_t *pData,RC_Ctrl *rc_ctrl)
{
    if(pData == NULL)
    {
        return;
    }

    rc_ctrl->rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;//长度11bit
    rc_ctrl->rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;//长度11bit
    rc_ctrl->rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) | ((int16_t)pData[4] << 10)) & 0x07FF;//长度11bit
    rc_ctrl->rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;//长度11bit

    rc_ctrl->rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
    rc_ctrl->rc.s2 = ((pData[5] >> 4) & 0x0003);

    rc_ctrl->mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    rc_ctrl->mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
    rc_ctrl->mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);

    rc_ctrl->mouse.press_l = pData[12];
    rc_ctrl->mouse.press_r = pData[13];

    rc_ctrl->key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);

}

void RC_Init(void)//初始化
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1,UART_RECV,117);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

void RC_Robot_Ctrl()//遥控控制函数 给运动参数结构体赋值
{
    if(RC_CtrlData.rc.s2==RC_SW_DOWN)//急停
    {

    }
    if(RC_CtrlData.rc.s2==RC_SW_MID&&RC_CtrlData.rc.s1==RC_SW_DOWN)//遥控
    {
        run_data.Vx =   (RC_CtrlData.rc.ch3-RC_CH_VALUE_OFFSET);
        run_data.Vy =   -(RC_CtrlData.rc.ch2-RC_CH_VALUE_OFFSET);
        run_data.Vw =   -(RC_CtrlData.rc.ch0-RC_CH_VALUE_OFFSET);

    }

    if(RC_CtrlData.rc.s2==RC_SW_UP&&RC_CtrlData.rc.s1==RC_SW_UP)
    {

    }
}



