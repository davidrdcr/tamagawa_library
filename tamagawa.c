#include "tamagawa.h"
#include "string.h"

extern UART_HandleTypeDef huart6;
struct TamagawaInterface ti;
uint8_t tx_size = 0, rx_size = 0;
uint8_t tx;

uint8_t USART6_RX_Buff[11];
uint8_t tamagawa_rx_cnt = 0;
uint8_t usart_data;

float position  = 0;
short turns = 0;

void tx_prepare(uint8_t *tx, uint8_t *tx_size, uint8_t *rx_size);
void tamagawa_tx(uint8_t tx_size, uint8_t *tx_buffer);
uint8_t crc(uint8_t *s, uint8_t len);

void tamagawa_read(uint8_t data_id)
{

	ti.data_id = data_id;
	tamagawa_rx_cnt = 0;
	tx_prepare(&tx, &tx_size, &rx_size);
	tamagawa_tx(tx_size, &tx);
}

void tx_prepare(uint8_t *tx, uint8_t *tx_size, uint8_t *rx_size)
{
    switch(ti.data_id)
    {
        case DATA_ID_0:
            *tx = 0x02;
            *tx_size = 1;
            *rx_size = 6;
            break;

        case DATA_ID_1:
            *tx = 0x8A;
            *tx_size = 1;
            *rx_size = 6;
            break;

        case DATA_ID_2:
            *tx = 0x92;
            *tx_size = 1;
            *rx_size = 4;
            break;

        case DATA_ID_3:
            *tx = 0x1A;
            *tx_size = 1;
            *rx_size = 11;
            break;

        case DATA_ID_7:
            *tx = 0xBA;
            *tx_size = 1;
            *rx_size = 6;
            break;

        case DATA_ID_8:
            *tx = 0xC2;
            *tx_size = 1;
            *rx_size = 6;
            break;

        case DATA_ID_C:
            *tx = 0x62;
            *tx_size = 1;
            *rx_size = 6;
            break;

        case DATA_ID_6:
            *tx++ = 0x32;
            *tx++ = ti.tx.adf;
            *tx++ = ti.tx.edf;
            *tx_size = 4;
            *rx_size = 4;
            *tx = crc(tx - 3, *tx_size - 1);
            break;

        case DATA_ID_D:
            *tx++ = 0xEA;
            *tx++ = ti.tx.adf;
            *tx_size = 3;
            *rx_size = 4;
            *tx = crc(tx - 2, *tx_size - 1);
            break;

        default:
            break;
    }
}

void tamagawa_tx(uint8_t tx_size, uint8_t *tx_buffer)
{
	while(tx_size--)
    {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  1);
		uint8_t tx_temp;
		tx_temp = *tx_buffer++;
		HAL_UART_Transmit(&huart6,&tx_temp, sizeof(tx_temp),10);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  0);
    }
}

void rx_parse(uint8_t *p)
{
    switch(ti.data_id)
    {
        case DATA_ID_0:
        {
            ti.rx.cf = p[0];
            ti.rx.sf = p[1];
            ti.rx.abs = p[2] | (p[3] << 8) | (p[4] << 16);
            ti.rx.crc = p[5];
            position  = ((double)(ti.rx.abs) / 131071) * 360;
            break;
        }
        case DATA_ID_7:
            break;
        case DATA_ID_8:
            break;
        case DATA_ID_C:
            ti.rx.cf = p[0];
            ti.rx.sf = p[1];
            ti.rx.abs = p[2] | (p[3] << 8) | (p[4] << 16);
            ti.rx.crc = p[5];
            break;

        case DATA_ID_1:
            ti.rx.cf = p[0];
            ti.rx.sf = p[1];
            ti.rx.abm = p[2] | (p[3] << 8) | (p[4] << 16);
            ti.rx.crc = p[5];
            turns = ti.rx.abm;
            break;

        case DATA_ID_2:
            ti.rx.cf = p[0];
            ti.rx.sf = p[1];
            ti.rx.enid = p[2];
            ti.rx.crc = p[3];
            break;

        case DATA_ID_3:
            ti.rx.cf = p[0];
            ti.rx.sf = p[1];
            ti.rx.abs = p[2] | (p[3] << 8) | (p[4] << 16);
            ti.rx.enid = p[5];
            ti.rx.abm = p[6] | (p[7] << 8) | (p[8] << 16);
            ti.rx.almc = p[9];
            ti.rx.crc = p[10];
            position  = ((double)(ti.rx.abs) / 131071) * 360;
            turns = ti.rx.abm;

            break;

        case DATA_ID_6:
            break;
        case DATA_ID_D:
            ti.rx.cf = p[0];
            ti.rx.adf = p[1];
            ti.rx.edf = p[2];
            ti.rx.crc = p[3];
            break;

        default:break;
    }

}

uint8_t crc(uint8_t *s, uint8_t len)
{
    uint8_t crc = 0;
    uint8_t val;
    int  i, j;
    uint8_t data[12];

    strncpy((char *)data, (const char *)s, len);

    for(i = 0; i < len; i++)
        for(j = 0; j < 8; j++)
        {
            val = (data[i] >> 7) ^ (crc >> 7);

            crc <<= 1;
            data[i] <<= 1;
            crc |= val;
        }

    return crc;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(&huart6, &usart_data, 1);
	if(tamagawa_rx_cnt < rx_size){
		USART6_RX_Buff[tamagawa_rx_cnt] = usart_data;
		tamagawa_rx_cnt++;
	}

	if(tamagawa_rx_cnt == rx_size)
	{
		rx_parse(USART6_RX_Buff);
	}
}
