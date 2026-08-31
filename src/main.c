#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "definitions.h"
#include "driver/flash_eeprom.h"

int main(void) {
    SYS_Initialize(NULL);

    if (FLASH_EEPROM_Init() != FLASH_EEPROM_OK) {
//        GREEN_LED_Set();
    }
    //    FLASH_EEPROM_EraseAll();

    TMR2_Start();

    while (1) {
         GREEN_LED_Set();
    }
}
