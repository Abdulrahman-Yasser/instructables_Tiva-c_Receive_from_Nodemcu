

/**
 * main.c
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"



#include "CAN/Static/CAN.h"

#include "CAN/Dynamic/CAN_Cfg.h"

void SystemInit(void);

static bool g_bTransmitNow = false;

int main(void)
{
    tCANMsgObject RxMsg;

    uint8_t my_Tx_Data[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    uint8_t my_Rx_Data[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    RxMsg.pui8MsgData = my_Rx_Data;

    SystemInit();

    CAN_my_Init(CAN_getConfig());

    CAN_MsgInint(CAN_getMsgConfig());



    int i = 0, j = 0;

    while(1){

        if(g_bRXFlag_TX6 == 1){
            g_bRXFlag_TX6 = 0;
            my_Tx_Data[0] = i++;
            my_Tx_Data[1] = i++;
            my_Tx_Data[2] = i++;
            my_Tx_Data[3] = i++;
            my_Tx_Data[4] = i++;
            my_Tx_Data[5] = i++;
            my_Tx_Data[6] = i++;
            my_Tx_Data[7] = i++;
        }

        if(g_bRXFlag_RX7 == 1){
            g_bRXFlag_RX7 = 0;
            CANMessageGet(CAN0_BASE, 7, &RxMsg, 0);
            if(RxMsg.pui8MsgData[0] == 0){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
            }else if(RxMsg.pui8MsgData[0] == 1){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
            }
        }

        if(g_bTransmitNow == true){
            g_bTransmitNow = false;
            CAN_MsgSend(6, 0x33, my_Tx_Data, 8);
            for(j = 0; j < 100; j++);
        }
    }
}



void SystemInit(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* Check if the peripheral access is enabled. */
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }


    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    /* Set GPIO_PORTF_BASE, GPIO_PIN_1 high */
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);




    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    /* enable interrupt to portf */
    IntEnable(INT_GPIOF);

}

void GPIOFIntHandler(void)
{


    /*check if interrupt is by SW1*/
    if (GPIOIntStatus(GPIO_PORTF_BASE,0) & GPIO_PIN_4)
    {
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
        g_bTransmitNow = true;
    }
    /* check if interrupt causes by PF0/SW2 */
    else if (GPIOIntStatus(GPIO_PORTF_BASE,0) & GPIO_PIN_0)
    {
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
        g_bTransmitNow = true;
    }
    else
    {
        /*do nothing*/
    }
}
