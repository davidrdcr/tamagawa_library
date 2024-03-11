# tamagawa_library
Note: This library is a modified version derived from the repository library of [imuncle](https://github.com/imuncle), Embedded_Peripheral_Libs. This code has been tested with the TS5700N8501 encoder.

## 1. Interface Setup

The encoder utilizes the RS-485 protocol. To operate it, a transceiver supporting RS-485 with a minimum speed of 2.5 Mbps is required. In this instance, a MAX485 transceiver was employed.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/diagram.png?raw=true)

##	2. Clock Configuration

For UART communication at a speed of 2.5 Mbps, it's essential to configure the High-Speed Clock as a Crystal/Ceramic Resonator and adjust the frequency to one suitable for the UART peripheral speed.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e3.png?raw=true)
![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e4.png?raw=true)

##	3.  UART Peripheral Configuration

The UART should be set to asynchronous mode. Configure the UART peripheral to **2.5 Mbps or 2500000 bits/s, 8 data bits, 1 stop bit**.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e1.png?raw=true)

Configure the DMA for reception in **circular mode**.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e2.png?raw=true)

##	4.  Pin Configuration for Direction Control

To manage transmission and reception of the MAX485, utilize a GPIO pin. In this instance, PA10 is employed. If another pin is used, ensure to modify it within the `tamagawa_tx` function of the `tamagawa.c` file.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e5.png?raw=true)

##	5.  Library Integration

Add the `tamagawa.c` file to the src folder and include the header `tamagawa.h` in the inc folder of the project.

## 	6.  Library Utilization

    #include "tamagawa.h"
    
    (...)
  
    int main (){
    
      (...)
      
      HAL_UART_Receive_DMA (&huart6, &usart_data, 1);
      
      while (1){
      
    	tamagawa_read(DATA_ID_3);
    	HAL_Delay(15); 
    	
    	(...) 
      }
      
    }

## Notes

 - The library records the encoder's position and the number of turns in the variables `position` and `turns`, respectively.
 - Never transmit any requests to the encoder while it is transmitting data. Doing so could potentially damage the interface. For this reason, it is advisable to maintain a delay between each reading.
