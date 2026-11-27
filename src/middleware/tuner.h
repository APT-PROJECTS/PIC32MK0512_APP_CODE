#ifndef _TUNING_H
#define _TUNING_H

#include "definitions.h"
#include "driver.h"

//--------------------------------------------------
// CAN ID
//--------------------------------------------------
#define TUNER_CAN_ID_SOFTWARE                   0xE0U  
#define TUNER_CAN_ID_MCU                        0xE1U

//--------------------------------------------------
// CAN CMD 0
//--------------------------------------------------
#define TUNER_CAN_CMD_0_STATUS                  0x01U

#define TUNER_CAN_CMD_0_PIC_ID                  0x02U
#define TUNER_CAN_CMD_1_PIC_MAC_ID_1            0x01U
#define TUNER_CAN_CMD_1_PIC_MAC_ID_2            0x02U
#define TUNER_CAN_CMD_1_PIC_MAC_ID_3            0x03U
#define TUNER_CAN_CMD_1_PIC_MAC_ID_4            0x04U
#define TUNER_CAN_CMD_1_PIC_DEVICE_ID           0x05U
    
#define TUNER_CAN_CMD_0_METADATA                0x03U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_1        0x01U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_2        0x02U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_3        0x03U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_4        0x04U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_5        0x05U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_6        0x06U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_7        0x07U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_8        0x08U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_9        0x09U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_10       0x0AU
#define TUNER_CAN_CMD_1_METADATA_CHUNK_11       0x0BU
#define TUNER_CAN_CMD_1_METADATA_CHUNK_12       0x0CU
#define TUNER_CAN_CMD_1_METADATA_CHUNK_13       0x0DU
#define TUNER_CAN_CMD_1_METADATA_CHUNK_14       0x0EU
#define TUNER_CAN_CMD_1_METADATA_CHUNK_15       0x0FU
#define TUNER_CAN_CMD_1_METADATA_CHUNK_16       0x10U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_17       0x11U
#define TUNER_CAN_CMD_1_METADATA_CHUNK_18       0x12U

#define TUNER_CAN_CMD_0_TUNING                  0x04U

#define TUNER_CAN_CMD_0_SOFTWARE_RESET          0x05U

#define TUNER_CAN_CMD_3_OPERATION_IDLE          0b000
#define TUNER_CAN_CMD_3_OPERATION_READ          0b001
#define TUNER_CAN_CMD_3_OPERATION_WRITE         0b010
#define TUNER_CAN_CMD_3_OPERATION_WRITE_FLASH   0b011 
#define TUNER_CAN_CMD_3_OPERATION_ZERO_ANGLE    0b100
    
#define TUNER_CAN_CMD_3_STATUS_IDLE             0b00
#define TUNER_CAN_CMD_3_STATUS_SUCCESS          0b01
#define TUNER_CAN_CMD_3_STATUS_FAIL             0b10   
    
//--------------------------------------------------
// MATRIX SIZE
//--------------------------------------------------
#define ROWS    16
#define COLS    4
    
//==================================================
// ROW 1
//==================================================
#define R_FF01_01                    tunerReadMatrix[1][0]
#define R_FF01_02                    tunerReadMatrix[1][1]
#define R_FF01_03                    tunerReadMatrix[1][2]
#define R_FF01_04                    tunerReadMatrix[1][3]

#define W_FF01_01                    tunerWriteMatrix[1][0]
#define W_FF01_02                    tunerWriteMatrix[1][1]
#define W_FF01_03                    tunerWriteMatrix[1][2]
#define W_FF01_04                    tunerWriteMatrix[1][3]

//==================================================
// ROW 2
//==================================================
#define R_FF02_01                    tunerReadMatrix[2][0]
#define R_FF02_02                    tunerReadMatrix[2][1]
#define R_FF02_03                    tunerReadMatrix[2][2]
#define R_FF02_04                    tunerReadMatrix[2][3]

#define W_FF02_01                    tunerWriteMatrix[2][0]
#define W_FF02_02                    tunerWriteMatrix[2][1]
#define W_FF02_03                    tunerWriteMatrix[2][2]
#define W_FF02_04                    tunerWriteMatrix[2][3]

//==================================================
// ROW 3
//==================================================
#define R_FF03_01                    tunerReadMatrix[3][0]
#define R_FF03_02                    tunerReadMatrix[3][1]
#define R_FF03_03                    tunerReadMatrix[3][2]
#define R_FF03_04                    tunerReadMatrix[3][3]

#define W_FF03_01                    tunerWriteMatrix[3][0]
#define W_FF03_02                    tunerWriteMatrix[3][1]
#define W_FF03_03                    tunerWriteMatrix[3][2]
#define W_FF03_04                    tunerWriteMatrix[3][3]

//==================================================
// ROW 4
//==================================================
#define R_FF04_01                    tunerReadMatrix[4][0]
#define R_FF04_02                    tunerReadMatrix[4][1]   //(NA)
#define R_FF04_03                    tunerReadMatrix[4][2]
#define R_FF04_04                    tunerReadMatrix[4][3]

#define W_FF04_01                    tunerWriteMatrix[4][0]
#define W_FF04_02                    tunerWriteMatrix[4][1]   //(NA)
#define W_FF04_03                    tunerWriteMatrix[4][2]
#define W_FF04_04                    tunerWriteMatrix[4][3]

//==================================================
// ROW 5
//==================================================
#define R_FF05_01                    tunerReadMatrix[5][0]
#define R_FF05_02                    tunerReadMatrix[5][1]
#define R_FF05_03                    tunerReadMatrix[5][2]
#define R_FF05_04                    tunerReadMatrix[5][3]

#define W_FF05_01                    tunerWriteMatrix[5][0]
#define W_FF05_02                    tunerWriteMatrix[5][1]
#define W_FF05_03                    tunerWriteMatrix[5][2]
#define W_FF05_04                    tunerWriteMatrix[5][3]

//==================================================
// ROW 6
//==================================================
#define R_FF06_01                    tunerReadMatrix[6][0]
#define R_FF06_02                    tunerReadMatrix[6][1]
#define R_FF06_03                    tunerReadMatrix[6][2]
#define R_FF06_04                    tunerReadMatrix[6][3]

#define W_FF06_01                    tunerWriteMatrix[6][0]
#define W_FF06_02                    tunerWriteMatrix[6][1]
#define W_FF06_03                    tunerWriteMatrix[6][2]
#define W_FF06_04                    tunerWriteMatrix[6][3]

//==================================================
// ROW 7
//==================================================
#define R_FF07_01                    tunerReadMatrix[7][0]
#define R_FF07_02                    tunerReadMatrix[7][1]
#define R_FF07_03                    tunerReadMatrix[7][2]
#define R_FF07_04                    tunerReadMatrix[7][3]

#define W_FF07_01                    tunerWriteMatrix[7][0]
#define W_FF07_02                    tunerWriteMatrix[7][1]
#define W_FF07_03                    tunerWriteMatrix[7][2]
#define W_FF07_04                    tunerWriteMatrix[7][3]

//==================================================
// ROW 8
//==================================================
#define R_FF08_01                    tunerReadMatrix[8][0]
#define R_FF08_02                    tunerReadMatrix[8][1]
#define R_FF08_03                    tunerReadMatrix[8][2]
#define R_FF08_04                    tunerReadMatrix[8][3]

#define W_FF08_01                    tunerWriteMatrix[8][0]
#define W_FF08_02                    tunerWriteMatrix[8][1]
#define W_FF08_03                    tunerWriteMatrix[8][2]
#define W_FF08_04                    tunerWriteMatrix[8][3]

//==================================================
// ROW 9
//==================================================
#define R_FF09_01                    tunerReadMatrix[9][0]
#define R_FF09_02                    tunerReadMatrix[9][1]
#define R_FF09_03                    tunerReadMatrix[9][2]
#define R_FF09_04                    tunerReadMatrix[9][3]

#define W_FF09_01                    tunerWriteMatrix[9][0]
#define W_FF09_02                    tunerWriteMatrix[9][1]
#define W_FF09_03                    tunerWriteMatrix[9][2]
#define W_FF09_04                    tunerWriteMatrix[9][3]

//==================================================
// ROW 10
//==================================================
#define R_FF0A_01                    tunerReadMatrix[10][0]
#define R_FF0A_02                    tunerReadMatrix[10][1]
#define R_FF0A_03                    tunerReadMatrix[10][2]
#define R_FF0A_04                    tunerReadMatrix[10][3]

#define W_FF0A_01                    tunerWriteMatrix[10][0]
#define W_FF0A_02                    tunerWriteMatrix[10][1]
#define W_FF0A_03                    tunerWriteMatrix[10][2]
#define W_FF0A_04                    tunerWriteMatrix[10][3]

//==================================================
// ROW 11
//==================================================
#define R_FF0B_01                    tunerReadMatrix[11][0]
#define R_FF0B_02                    tunerReadMatrix[11][1]
#define R_FF0B_03                    tunerReadMatrix[11][2]
#define R_FF0B_04                    tunerReadMatrix[11][3]

#define W_FF0B_01                    tunerWriteMatrix[11][0]
#define W_FF0B_02                    tunerWriteMatrix[11][1]
#define W_FF0B_03                    tunerWriteMatrix[11][2]
#define W_FF0B_04                    tunerWriteMatrix[11][3]

//==================================================
// ROW 12
//==================================================
#define R_FF0C_01                    tunerReadMatrix[12][0]
#define R_FF0C_02                    tunerReadMatrix[12][1]
#define R_FF0C_03                    tunerReadMatrix[12][2]
#define R_FF0C_04                    tunerReadMatrix[12][3]

#define W_FF0C_01                    tunerWriteMatrix[12][0]
#define W_FF0C_02                    tunerWriteMatrix[12][1]
#define W_FF0C_03                    tunerWriteMatrix[12][2]
#define W_FF0C_04                    tunerWriteMatrix[12][3]

//==================================================
// ROW 13
//==================================================
#define R_FF0D_01                    tunerReadMatrix[13][0]
#define R_FF0D_02                    tunerReadMatrix[13][1]
#define R_FF0D_03                    tunerReadMatrix[13][2]
#define R_FF0D_04                    tunerReadMatrix[13][3]

//==================================================
// ROW 14
//==================================================
#define R_FF0E_01                    tunerReadMatrix[14][0]
#define R_FF0E_02                    tunerReadMatrix[14][1]
#define R_FF0E_03                    tunerReadMatrix[14][2]
#define R_FF0E_04                    tunerReadMatrix[14][3]

//==================================================
// ROW 15
//==================================================
#define R_FF0F_01                    tunerReadMatrix[15][0]
#define R_FF0F_02                    tunerReadMatrix[15][1]
#define R_FF0F_03                    tunerReadMatrix[15][2]
#define R_FF0F_04                    tunerReadMatrix[15][3]
    
//==================================================
// EEPROM FIELD OFFSET
//================================================== 
#define EEPROM_TUNER_OFFSET         (EEPROM_METADATA_OFFSET +   \
                                        EEPROM_METADATA_SIZE)
#define EEPROM_TUNER_SIZE           (EEPROM_DATA_SIZE -         \
                                        EEPROM_TUNER_OFFSET)

//==================================================
// API
//================================================== 
void Tuner_Init(void);
void Get_Config(void);
void Update_Config(void);

/*=================UART TUNING==================*/
void UART_Tuner_Send_PIC_ID(void);
void UART_Tuner_Send_Metadata(void);
void UART_Tuner_Write(void); 
void UART_Tuner_Read(uint8_t); 
void UART_Tuner_Command(uint8_t *); 
void UART_Tuner_Send_Zero_Angle(float, float, float, int); 
void UART_Tuner_Software_Reset(void);

/*=================CAN TUNING====================*/
void CAN_Tuner_Send_PIC_ID(void);
void CAN_Tuner_Send_Metadata(void);
void CAN_Tuner_Read_Write(void);
void CAN_Send_Status(void);
void CAN_Tuner_Send_Zero_Angle(float theta, float theta0, float theta1, 
    int CONFIG_SENSOR_DIRECTION_estimate);
void CAN_Tuner_Software_Reset(void);
void CAN_Tuner_Handler(uint32_t id, uint8_t *rxBuffer);

#endif