/*
 * CAN_Lcfg.c
 *
 *  Created on: Nov 16, 2023
 *      Author: abduy
 */

#include "CAN_Cfg.h"
#include "driverlib/can.h"



const CANConfig_t CANConfigs[] = {
                                  {.CAN_ON_OFF =1,
                                   .BaudRate = 125000,
                                   .CAN_Base=CAN0_BASE_e,
                                   .CAN_Interrupts = CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS},

                                   {.CAN_ON_OFF =0,
                                    .BaudRate = 125000,
                                    .CAN_Base=CAN1_BASE_e,
                                    .CAN_Interrupts = CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS}

};


CANConfigMsgs_t CANMsgConfigs[] = {
                                       {
                                        .CAN_Base = CAN0_BASE_e,
                                        .Msg_ID = 6,
                                        .flags = MSG_OBJ_TYPE_RX_REMOTE,
                                        .Msg =
                                        {
                                        .ui32MsgID = 0x33,
                                        .ui32MsgIDMask = 0xff,
                                        .ui32MsgLen = 8,
                                        .ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER
                                        }
                                       },
                                        {
                                         .CAN_Base = CAN0_BASE_e,
                                         .Msg_ID = 7,
                                         .flags = MSG_OBJ_TYPE_RXTX_REMOTE,
                                         .Msg =
                                         {
                                         .ui32MsgID = 0x23,
                                         .ui32MsgIDMask = 0xff,
                                         .ui32MsgLen = 8,
                                         .ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER
                                         }
                                       },
                                       {
                                        .CAN_Base = CAN0_BASE_e,
                                        .Msg_ID = 8,
                                        .flags = MSG_OBJ_TYPE_TX,
                                        .Msg =
                                        {
                                        .ui32MsgID = 0x33,
                                        .ui32MsgIDMask = 0xff,
                                        .ui32MsgLen = 8,
                                        .ui32Flags = MSG_OBJ_TX_INT_ENABLE
                                        }
                                      }
};

CANConfigMsgs_t* CAN_getMsgConfig(void){
    return &CANMsgConfigs[0];
}

CANConfigMsgs_t* CAN_getMsgConfigSpecificMsg(uint8_t id){
    uint8_t i;
    for(i = 0; i < NUMBER_OF_INITIALIZED_CAN_MESSAGES; i++){
        if(CANMsgConfigs[i].Msg_ID == id) {
            return &(CANMsgConfigs[i]);
        }
    }
    return 0;
}

CANConfig_t* CAN_getConfig(void){
    return &CANConfigs[0];
}

void CAN_SetAutoRemoteData(uint8_t myMsg_ID, uint8_t* data){
    int i;
    for(i = 0; i < NUMBER_OF_INITIALIZED_CAN_MESSAGES; i++){
        if( CANMsgConfigs[i].Msg_ID == myMsg_ID ){
            CANMsgConfigs[i].Msg.pui8MsgData = data;
            break;
        }
    }
}
