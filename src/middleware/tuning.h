#ifndef _TUNING_H    /* Guard against multiple inclusion */
#define _TUNING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "definitions.h"

//--------------------------------------------------
// CAN TUNER
//--------------------------------------------------
#define CAN_ID_TUNER_SOFTWARE           0xE0  
#define CAN_ID_TUNER_MCU                0xE1
    
#define CAN_CMD_TUNER_READ              0x00
#define CAN_CMD_TUNER_WRITE_MATRIX      0x01
#define CAN_CMD_TUNER_WRITE_FLASH       0x02
#define CAN_CMD_TUNER_STATUS            0x03
#define CAN_CMD_TUNER_PIC_ID_1          0x04
#define CAN_CMD_TUNER_PIC_ID_2          0x05


void Tuner_Init(void);

/*=================UART TUNER==================*/
void UART_Tuner_Send_PIC_ID(void);
void UART_Tuner_Write(void); 
void UART_Tuner_Read(uint8_t); 
void UART_Tuner_Command(uint8_t *); 
void UART_Tuner_Send_Zero_Angle(float, float, float, int); 

/*=================CAN TUNER====================*/
void CAN_Tuner_Send_PIC_ID_1(void);
void CAN_Tuner_Send_PIC_ID_2(void);
void CAN_Tuner_Read(uint8_t row, uint8_t col);
void CAN_Tuner_Write_Matrix(uint8_t row, uint8_t col, uint8_t *data);
void CAN_Tuner_Write_Flash(void);
void CAN_Send_Status(void);
void CAN_Tuner_Send_Zero_Angle(float theta, float theta0, float theta1, 
    int CONFIG_SENSOR_DIRECTION_estimate);
void CAN_Tuner_Handler(uint8_t *CANRXBuffer);

#ifdef __cplusplus
}
#endif

#endif /* _TUNING_H */

