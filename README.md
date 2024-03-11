# tamagawa_library
Nota: 

## 1. Conexión

El encoder utiliza el protocolo RS-485. Para su uso, es necesario un transceivers RS-485 con una velocidad de al menos 2.5 Mbps. En este caso se utilizó un MAX485.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/diagram.png?raw=true)

##	2. Configuración del reloj

Para utilizar la comunicación por UART con una velocidad de 2.5 Mbps, es necesario configurar el High Speed Clock como Crystal/Ceramic Resonator y aumentar la frecuencia a una que pueda permitir la velocidad del periférico UART.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e3.png?raw=true)
![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e4.png?raw=true)

##	3.  Configuración del periférico UART

El UART debe ser configurado en modo asíncrono. El periférico UART debe ser configurado a **2.5 Mbps o 2500000 Bits/s, 8 bits, 1 bit de parada**.
![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e1.png?raw=true)

El DMA debe de configurarse para recepción en **modo circular**.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e2.png?raw=true)

##	4.  Configurar el pin para controlar la dirección.

Para poder controlar la transmición y recepción del MAX485, es necesario utilizar un pin GPIO.  En este caso, se utiliza el PA10.

![alt text](https://github.com/davidrdcr/tamagawa_library/blob/main/img/e5.png?raw=true)

##	5.  Agregar la librería

Agregar el archivo `tamagawa. c` a la carpeta src y agregar la cabecera `tamagawa.h` a la carpeta inc del proyecto.

## 	6.  Utilizar la librería 

    #include "tamagawa.h"
    uint32_t position = 0;
    short turns = 0;
    int main (){
    	
      HAL_UART_Receive_DMA (&huart6, &usart_data, 1);
      
      while (1){
    	tamagawa_read(DATA_ID_3);
    	HAL_Delay(15);
      }
      
    }

