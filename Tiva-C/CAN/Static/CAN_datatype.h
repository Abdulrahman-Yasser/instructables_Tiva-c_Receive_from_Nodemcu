/*
 * CAN_datatype.h
 *
 *  Created on: Nov 16, 2023
 *      Author: abduy
 */

#ifndef CAN_STATIC_CAN_DATATYPE_H_
#define CAN_STATIC_CAN_DATATYPE_H_



#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "driverlib/can.h"

typedef enum{
    CAN0_BASE_e,
    CAN1_BASE_e,
}CAN_Base_t;

//typedef enum{
//    CAN_MSGOBJ_TX_TYPE,
//    CAN_MSGOBJ_TX_REMOTE_TYPE,
//    CAN_MSGOBJ_RX_TYPE,
//    CAN_MSGOBJ_RX_REMOTE_TYPE,
//    CAN_MSGOBJ_RXTX_TYPE
//}CAN_MsgObjType;

typedef struct{
    bool CAN_ON_OFF;
    CAN_Base_t CAN_Base;
    uint32_t BaudRate;
    uint8_t CAN_Interrupts;
}CANConfig_t;

typedef struct{
    CAN_Base_t CAN_Base;
    uint32_t Msg_ID;
    tMsgObjType flags;
    tCANMsgObject Msg;
}CANConfigMsgs_t;

#endif /* CAN_STATIC_CAN_DATATYPE_H_ */
