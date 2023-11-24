// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string


#define CAN0_INT 16                              // Set INT to pin 2
MCP_CAN CAN0(5);     // Set CS to pin 10

void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input

}
byte i = 0;
byte data[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void loop()
{
  byte sndStat;
  byte i;
  for(i = 0; i < 6; i++){
    // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
    sndStat = CAN0.sendMsgBuf(0x23 | 0x40000000, 8, data);

    if(sndStat == CAN_OK){
      Serial.println("Message Sent Successfully and received !");
    } else {
      Serial.println("Error Sending Message...");
    }
    data[0] = i++;
    data[1] = i++;
    data[2] = i++;
    data[3] = i++;
    data[4] = i++;
    data[5] = i++;
    data[6] = i++;
    data[7] = i++;
    delay(500);   // send data per 100ms
  }

  for(i = 0; i < 6; i++){
    while(digitalRead(CAN0_INT));
    if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
    {
      CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
      
      if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
        sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
      else
        sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    
      Serial.print(msgString);
    
      if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
        sprintf(msgString, " REMOTE REQUEST FRAME");
        Serial.print(msgString);
      } else {
        for(byte i = 0; i<len; i++){
          sprintf(msgString, " 0x%.2X", rxBuf[i]);
          Serial.print(msgString);
        }
      }
            
      Serial.println();
    }
  }

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
