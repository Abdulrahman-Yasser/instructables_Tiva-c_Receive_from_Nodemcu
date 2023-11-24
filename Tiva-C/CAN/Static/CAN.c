/*
 * CAN.c
 *
 *  Created on: Nov 16, 2023
 *      Author: abduy
 */


#include "CAN.h"
#include "../Dynamic/CAN_Cfg.h"

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"


volatile uint32_t g_ui32MsgCount = 0;

volatile bool g_bErrFlag = 0;

volatile bool g_bRXFlag_TX6 = 0;
volatile bool g_bRXFlag_RX7 = 0;

void DefaultIntHandler(void);

void DefaultIntHandler(void){

}

void CAN_my_Init(CANConfig_t* myConfigs){
    int i;
    for(i = 0; i < NUMBER_OF_CAN_CHANNELS; i++ ){
        if(myConfigs[i].CAN_ON_OFF == 0){
            continue;
        }
        if(myConfigs[i].CAN_Base == CAN0_BASE_e){

            /*
            * PORT's PIN Initialization
            */

            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

            while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

            GPIOPinConfigure(GPIO_PB4_CAN0RX);
            GPIOPinConfigure(GPIO_PB5_CAN0TX);

            GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

            /*
             * CAN module Initialization
             */

            SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

            CANInit(CAN0_BASE);
            CANBitRateSet(CAN0_BASE, SysCtlClockGet(), myConfigs[i].BaudRate);

            CANIntEnable(CAN0_BASE, myConfigs[i].CAN_Interrupts);
            CANIntRegister(CAN0_BASE, CANIntHandler);
            IntEnable(INT_CAN0);
            CANEnable(CAN0_BASE);

        }else if(myConfigs[i].CAN_Base == CAN1_BASE_e){

            /*
            * PORT's PIN Initialization
            */

            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

            while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

            GPIOPinConfigure(GPIO_PA0_CAN1RX);
            GPIOPinConfigure(GPIO_PA1_CAN1TX);

            GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

            /*
             * CAN module Initialization
             */

            SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN1);

            CANInit(CAN1_BASE);
            CANBitRateSet(CAN1_BASE, SysCtlClockGet(), myConfigs[i].BaudRate);

            CANIntEnable(CAN1_BASE, myConfigs[i].CAN_Interrupts);
            CANIntRegister(CAN1_BASE, CANIntHandler);
            IntEnable(INT_CAN1);
            CANEnable(CAN1_BASE);

        }
    }

}

void CAN_MsgInint(CANConfigMsgs_t* myConfigs){
    int i;
    for(i = 0; i < NUMBER_OF_INITIALIZED_CAN_MESSAGES; i++ ){
        if(myConfigs[i].CAN_Base == CAN0_BASE_e){
            CANMessageSet(CAN0_BASE, myConfigs[i].Msg_ID, &myConfigs[i].Msg, myConfigs[i].flags);
        }else if(myConfigs[i].CAN_Base == CAN1_BASE_e){
            CANMessageSet(CAN1_BASE, myConfigs[i].Msg_ID, &myConfigs[i].Msg, myConfigs[i].flags);
        }
    }
}

void CAN_MsgSend(uint8_t ObjID, uint8_t MsgID, uint8_t* data, uint8_t len){
    tCANMsgObject original;
    CANConfigMsgs_t* myMsg;
    myMsg = CAN_getMsgConfigSpecificMsg(ObjID);

    if(myMsg->flags != MSG_OBJ_TYPE_TX){
        return;
    }

    original.pui8MsgData = myMsg->Msg.pui8MsgData;
    original.ui32Flags = myMsg->Msg.ui32Flags;
    original.ui32MsgID = myMsg->Msg.ui32MsgID;
    original.ui32MsgIDMask = myMsg->Msg.ui32MsgIDMask;
    original.ui32MsgLen = myMsg->Msg.ui32MsgLen;

    myMsg->Msg.pui8MsgData = data;
    myMsg->Msg.ui32MsgLen = len;
    myMsg->Msg.ui32MsgID = MsgID;

    if(myMsg->CAN_Base == CAN0_BASE_e){
        CANMessageSet(CAN0_BASE, myMsg->Msg_ID, &myMsg->Msg, myMsg->flags); // send as msg object 1
    }else if(myMsg->CAN_Base == CAN1_BASE_e){
        CANMessageSet(CAN1_BASE, myMsg->Msg_ID, &myMsg->Msg, myMsg->flags); // send as msg object 1
    }

    myMsg->Msg.pui8MsgData = original.pui8MsgData;
    myMsg->Msg.ui32Flags = original.ui32Flags;
    myMsg->Msg.ui32MsgID = original.ui32MsgID;
    myMsg->Msg.ui32MsgIDMask = original.ui32MsgIDMask;
    myMsg->Msg.ui32MsgLen = original.ui32MsgLen;
}

void CAN_MsgGet(CAN_Base_t myCAN, uint32_t MsgID, tCANMsgObject *psMssgObject, bool bClrPendingInt){
    if(myCAN == CAN0_BASE_e){
        CANMessageGet(CAN0_BASE, MsgID, psMssgObject, bClrPendingInt);
    }else if(myCAN == CAN1_BASE_e){
        CANMessageGet(CAN1_BASE, MsgID, psMssgObject, bClrPendingInt);
    }
}

void CAN_CallBackINT_set(CAN_Base_t myCAN, void (*pCallBackFun)(void)){
    if(myCAN == CAN0_BASE_e){
        CANIntRegister(CAN0_BASE, pCallBackFun); // use dynamic vector table allocation
    }else if(myCAN == CAN1_BASE_e){
        CANIntRegister(CAN1_BASE, pCallBackFun); // use dynamic vector table allocation
    }
}

void CANIntHandler(void)
{
    uint32_t InterruptMsg, ui32status, INT_Status;  /* Declare a variable to store interrupt status. */
    /* Read the CAN interrupt status to find the cause of the interrupt. */
    InterruptMsg = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    INT_Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

    if(InterruptMsg == CAN_INT_INTID_STATUS){
        if(INT_Status & CAN_STS_RXOK){
            ui32status = CANStatusGet(CAN0_BASE, CAN_STS_TXREQUEST);
            /*
             * In case it response to a Automatic Transmit Remote message
             */
            if(ui32status > 0){
                InterruptMsg = 0;
                while(ui32status > 0){
                    ui32status = ui32status >> 1;
                    InterruptMsg++;
                }
                ui32status = 0;
            }else{
                /*
                 * In case it response to a remote message
                 */
                ui32status = CANStatusGet(CAN0_BASE, CAN_STS_NEWDAT);
                if(ui32status > 0){
                    InterruptMsg = 0;
                    while(ui32status > 0){
                        ui32status = ui32status >> 1;
                        InterruptMsg++;
                    }
                    ui32status = 0;
                }
            }
        }
    }

    /* If the cause is a controller status interrupt, then get the status. */
    switch(InterruptMsg) {
    case CAN_INT_INTID_STATUS:
        /* Read the controller status. This will return a field of status
         * error bits that can indicate various errors. Error processing
         * is not done in this example for simplicity. Refer to the
         * API documentation for details about the error status bits.
         * The act of reading this status will clear the interrupt. If the
         * CAN peripheral is not connected to a CAN bus with other CAN devices
         * present, then errors will occur and will be indicated in the
         * controller status.
         */
        InterruptMsg = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
        break;
        /* Set a flag to indicate some errors may have occurred. */

    /* Check if the cause is message object 1, which is what we are using for
     * sending messages.
     */
    case 6:
        /* Getting to this point means that the TX interrupt occurred on
         * message object 6, and the message TX is complete. Clear the
         * message object interrupt.
         */
        CANIntClear(CAN0_BASE, 6);

        g_ui32MsgCount++;
        g_bRXFlag_TX6 = 1;

        /* Increment a counter to keep track of how many messages have been
         * sent. In a real application, this could be used to set flags to
         * indicate when a message is sent.
         */

        /* Since the message was sent, clear any error flags. */
        g_bErrFlag = 0;
        break;

    case 7:
        /* Getting to this point means that the RX interrupt occurred on
         * message object 7, and the message RX is complete. Clear the
         * message object interrupt.
         */
        CANIntClear(CAN0_BASE, 7);

        g_ui32MsgCount++;
        g_bRXFlag_RX7 = 1;

        /* Increment a counter to keep track of how many messages have been
         * sent. In a real application, this could be used to set flags to
         * indicate when a message is sent.
         */

        /* Since the message was sent, clear any error flags. */
        g_bErrFlag = 0;
        break;

    default:
        /* Spurious interrupt handling can go here.
         * This part is executed when an unexpected interrupt occurs,
         * which should not normally happen.
         */
        break;
}
}
