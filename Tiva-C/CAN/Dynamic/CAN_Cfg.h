/*
 * CAN_Cfg.h
 *
 *  Created on: Nov 16, 2023
 *      Author: abduy
 */

#ifndef CAN_DYNAMIC_CAN_CFG_H_
#define CAN_DYNAMIC_CAN_CFG_H_

#include "../Static/CAN_datatype.h"

#define NUMBER_OF_CAN_CHANNELS  2

#define NUMBER_OF_INITIALIZED_CAN_MESSAGES 2

CANConfigMsgs_t* CAN_getMsgConfig(void);

CANConfig_t* CAN_getConfig(void);

CANConfigMsgs_t* CAN_getMsgConfigSpecificMsg(uint8_t id);

void CAN_SetAutoRemoteData(uint8_t myMsg_ID, uint8_t* data);

#endif /* CAN_DYNAMIC_CAN_CFG_H_ */
