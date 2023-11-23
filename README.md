# instructables_Tiva-c_Receive_from_Nodemcu
The project is a CAN bus communication between Nodemcu and Tiva-C. This repo contains the code for Instructables's project link
## Description :
- Tiva-C wired with mcp2551
- Nodemcu wired with mcp2515
- mcp2515 uses SPI module on Nodemcu to generate the CAN Signal.
- the CAN Bus Communication is between CANH and CANL on both mcp2515 and mcp2551 
- Tiva-C Receives multiple messages from Nodemcu.
- Tiva-C filters the IDs on the CAN Bus to receive the IDs it wants to receive.
- Tiva-C's code is friendly and configurable to allow the user to re-configure it to suit his application.
- Tiva-C's Code is separated into two parts. 
    - Static -: Shouldn't be changed
    - Dynamic -: you can change it to suit your application.
