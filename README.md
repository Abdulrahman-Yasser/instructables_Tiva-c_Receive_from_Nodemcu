# instructables_Tiva-c_Receive_from_Nodemcu
The project is a CAN bus communication between Nodemcu and Tiva-C. This repo contains the code for Instructables's project link
## Description :
- Tiva-C wired with mcp2551.
- Nodemcu wired with mcp2515.
- mcp2515 uses SPI module on Nodemcu to generate the CAN Signal.
- mcp2515 needs 5V to work fine.
- the CAN Bus Communication is between CANH and CANL on both mcp2515 and mcp2551.
- Tiva-C Receives multiple messages from Nodemcu.
- Tiva-C filters the IDs on the CAN Bus to receive the IDs it wants to receive.
- Tiva-C's code is friendly and configurable to allow the user to re-configure it to suit his application.
- Tiva-C's Code is separated into two parts. 
    - Static -: Shouldn't be changed.
    - Dynamic -: you can change it to suit your application.

![fdf8466e11c941098480c8dc32a0388d6e87fec4](https://github.com/Abdulrahman-Yasser/instructables_Tiva-c_Receive_from_Nodemcu/assets/63866803/4316e8ee-6152-44d8-8663-757751337b50)
![mcp2551-can-bus-shield-for-tiva-c-tm4c123-arm-board](https://github.com/Abdulrahman-Yasser/instructables_Tiva-c_Receive_from_Nodemcu/assets/63866803/7be3a25b-91c5-4c4f-8cda-5541318d8c62)
![WhatsApp Image 2023-11-23 at 6 47 58 PM](https://github.com/Abdulrahman-Yasser/instructables_Tiva-c_Receive_from_Nodemcu/assets/63866803/7570e75e-b2ad-421d-aff5-b8730c4d6398)
