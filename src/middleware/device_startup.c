#include "definitions.h"
#include "device.h"
#include "driver.h"
#include "middleware.h"

uint32_t devsn[4];
uint32_t devid;

uint8_t eepromBuffer[EEPROM_DATA_SIZE];

float readMatrix[ROWS][COLS];
float writeMatrix[ROWS][COLS];

static int mcuID = 0;

void Get_PIC_ID(void) {
    devsn[0] = DEVSN0;
    devsn[1] = DEVSN1;
    devsn[2] = DEVSN2;
    devsn[3] = DEVSN3;

    devid = DEVID & 0x0FFFFFFFU;
}

void NVM_Memory_Initialize(void) {
    FLASH_EEPROM_Init();
    Load_Matrix();
}

void Load_Matrix(void) {
    __builtin_memset(eepromBuffer, 0, EEPROM_DATA_SIZE);
    __builtin_memset(readMatrix, 0, sizeof (readMatrix));
    __builtin_memset(writeMatrix, 0, sizeof (writeMatrix));

    FLASH_EEPROM_Read_Page(eepromBuffer, sizeof (readMatrix));
    //--------------------------------------------------
    // Copy eeprom buffer to read matrix
    //--------------------------------------------------
    __builtin_memcpy(readMatrix, eepromBuffer, sizeof (readMatrix));

    mcuID = R_FF01_01;

    //--------------------------------------------------
    // Check ESC ID
    //--------------------------------------------------
    if (mcuID <= 0) {
        Get_Config();
    } else {
        Update_Config();
    }

    //--------------------------------------------------
    // Copy read matrix to write matrix     
    //--------------------------------------------------
    __builtin_memcpy(writeMatrix, readMatrix, sizeof (writeMatrix));
}