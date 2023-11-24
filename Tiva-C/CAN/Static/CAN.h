/*
 * CAN.h
 *
 *  Created on: Nov 16, 2023
 *      Author: abduy
 */

#ifndef CAN_STATIC_CAN_H_
#define CAN_STATIC_CAN_H_

#include "CAN_datatype.h"
#include "driverlib/can.h"

extern volatile bool g_bRXFlag_TX6;
extern volatile bool g_bRXFlag_RX7;



void CAN_my_Init(CANConfig_t* myConfigs);

void CAN_MsgInint(CANConfigMsgs_t* myConfigs);

void CAN_MsgSend(uint8_t ObjID, uint8_t MsgID, uint8_t* data, uint8_t len);

void CAN_MsgGet(CAN_Base_t myCAN, uint32_t MsgID, tCANMsgObject *psMssgObject, bool bClrPendingInt);

void CAN_CallBackINT_set(CAN_Base_t myCAN, void (*pCallBackFun)(void));

void CANIntHandler(void);

#endif /* CAN_STATIC_CAN_H_ */
