//
// Created by Administrator on 2024/12/13.
//

#include "../Inc/bsp_can.h"
//
// Created by Administrator on 2024/12/13.
//



#include "can.h"

motor_t  motor_chassis_3508_info[4];
motor_t  motor_bigyaw_6020_info;
void can1_init(void) {
    CAN_FilterTypeDef can_filter;

    can_filter.FilterActivation = CAN_FILTER_ENABLE;
    can_filter.FilterBank = 0;
    can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    can_filter.FilterIdHigh =     0x201>>5;
    can_filter.FilterIdLow =      0x202>>5;
    can_filter.FilterMaskIdLow =  0x203>>5;
    can_filter.FilterMaskIdHigh = 0x204>>5;
    can_filter.FilterMode = CAN_FILTERMODE_IDLIST;
    can_filter.FilterScale = CAN_FILTERSCALE_16BIT;

    HAL_CAN_ConfigFilter(&hcan1, &can_filter);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void can2_init(void) {
    CAN_FilterTypeDef can_filter;

    can_filter.FilterActivation = CAN_FILTER_ENABLE;
    can_filter.FilterBank = 1;
    can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    can_filter.FilterIdHigh =     0x205>>5;
    can_filter.FilterIdLow =      0x206>>5;
    can_filter.FilterMaskIdLow =  0x207>>5;
    can_filter.FilterMaskIdHigh = 0x208>>5;
    can_filter.FilterMode = CAN_FILTERMODE_IDLIST;
    can_filter.FilterScale = CAN_FILTERSCALE_16BIT;

    HAL_CAN_ConfigFilter(&hcan2, &can_filter);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
}

void set_chassis_3508_v(int16_t v1, int16_t v2, int16_t v3, int16_t v4)
{
    CAN_TxHeaderTypeDef  tx_header1;
    uint8_t              tx_data[8];


    tx_header1.StdId= 0x200;  //  0x200(1~4) or 0x1ff(5~8)
    tx_header1.DLC =8;
    tx_header1.IDE=CAN_ID_STD;
    tx_header1.RTR=CAN_RTR_DATA;


    tx_data[0]=   (v1)>>8 &  0xff;
    tx_data[1]=    v1     &  0xff;
    tx_data[2]=   (v2)>>8 &  0xff;
    tx_data[3]=    v2     &  0xff;
    tx_data[4]=   (v3)>>8 &  0xff;
    tx_data[5]=    v3     &  0xff;
    tx_data[6]=   (v4)>>8 &  0xff;
    tx_data[7]=    v4     &  0xff;
    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0)
    {
        HAL_CAN_AddTxMessage(&hcan1, &tx_header1, tx_data, (uint32_t *)CAN_TX_MAILBOX0);
    }
}


void set_bigyaw_6020_v( int16_t v1)
{
    CAN_TxHeaderTypeDef  tx_header2;
    uint8_t              tx_data2[2];

    tx_header2.StdId= 0x1fe;
    tx_header2.DLC =8;
    tx_header2.IDE=CAN_ID_STD;
    tx_header2.RTR=CAN_RTR_DATA;

    tx_data2[0]=   (v1)>>8 &  0xff;
    tx_data2[1]=    v1     &  0xff;

    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) > 0)
    {
        HAL_CAN_AddTxMessage(&hcan2, &tx_header2, tx_data2, (uint32_t *)CAN_TX_MAILBOX1);
    }
}

void send_msg( int16_t tx2c1,int16_t tx2c2,int16_t  tx2c3,int16_t  tx2c4)
{
    CAN_TxHeaderTypeDef  tx_header3;
    uint8_t              tx_data3[8];

    tx_header3.StdId= 0x211;
    tx_header3.DLC =8;
    tx_header3.IDE=CAN_ID_STD;
    tx_header3.RTR=CAN_RTR_DATA;

    tx_data3[0]=   (tx2c1)>>8 &  0xff;
    tx_data3[1]=    tx2c1     &  0xff;
    tx_data3[2]=   (tx2c2)>>8 &  0xff;
    tx_data3[3]=    tx2c2     &  0xff;
    tx_data3[4]=   (tx2c3)>>8  &  0xff;
    tx_data3[5]=    tx2c3      &  0xff;
    tx_data3[6]=   (tx2c4)>>8  &  0xff;
    tx_data3[7]=    tx2c4      &  0xff;

    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) > 0)
    {
        HAL_CAN_AddTxMessage(&hcan1, &tx_header3, tx_data3, (uint32_t *)CAN_TX_MAILBOX1);
    }

}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rx_header1;
    uint8_t rx_data[8];


    if (hcan->Instance == CAN1) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header1, rx_data);
    }//receive can data

    switch (rx_header1.StdId) {
        case 0x201:
        case 0x202:
        case 0x203:
        case 0x204:                 //0x200+ID;
        {
            uint8_t index = rx_header1.StdId - 0x201;                  // get motor index by can_id
            motor_chassis_3508_info[index].rotor_angle = ((rx_data[0] << 8) | rx_data[1]);
            motor_chassis_3508_info[index].rotor_speed = ((rx_data[2] << 8) | rx_data[3]);
            motor_chassis_3508_info[index].torque_current = ((rx_data[4] << 8) | rx_data[5]);
            motor_chassis_3508_info[index].temp = rx_data[6];

            break;
        }
    }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef  rx_header2;
    uint8_t              rx_data2[8];

    if(hcan->Instance == CAN2)    //steer
    {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header2, rx_data2);
    }//receive can data

    switch(rx_header2.StdId)
    {
        case 0x205:
        case 0x206:
        case 0x207:
        case 0x208:		//0x204+ID;
        {
            // get motor index by can_id
            motor_bigyaw_6020_info.rotor_angle    = ((rx_data2[0] << 8) | rx_data2[1]);
            motor_bigyaw_6020_info.rotor_speed    = ((rx_data2[2] << 8) | rx_data2[3]);
            motor_bigyaw_6020_info.torque_current = ((rx_data2[4] << 8) | rx_data2[5]);
            motor_bigyaw_6020_info.temp           =   rx_data2[6];

            if       ((motor_bigyaw_6020_info.last_encode-motor_bigyaw_6020_info.rotor_angle)>4096)
            {
                motor_bigyaw_6020_info.circle_count++;
            }
            else if ((motor_bigyaw_6020_info.last_encode-motor_bigyaw_6020_info.rotor_angle)<-4096)
            {
                motor_bigyaw_6020_info.circle_count--;
            }
            motor_bigyaw_6020_info.last_encode =motor_bigyaw_6020_info.rotor_angle;
            motor_bigyaw_6020_info.add_encode=(motor_bigyaw_6020_info.circle_count)*8192+motor_bigyaw_6020_info.rotor_angle;
            break;
        }

    }

}


