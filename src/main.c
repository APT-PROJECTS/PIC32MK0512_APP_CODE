#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "definitions.h"
#include "middleware.h"

int main(void) {
    SYS_Initialize(NULL);

    EEPROM_Initialize();

    TMR2_Start();

    while (1) {
         GREEN_LED_Set();
    }
}

//${MP_CC_DIR}/xc32-objcopy -I ihex -O binary dist\default\production\PIC32MK1024_APP_CODE.X.production.hex dist\default\production\hub_sp_1.1_scu_14.bin

//"C:\Program Files\Microchip\xc32\v5.00\bin\xc32-objcopy.exe" -O binary -R .config_* -R .comment -R .debug* -R __c30_signature "dist\default\debug\PIC32MK1024_APP_CODE.X.debug.elf" "dist\default\debug\hub_sp_1.1_scu_14.bin"

//xc32-objcopy -O binary --only-section=.text* --only-section=.rodata* --only-section=.vectors dist/default/${IMAGE_TYPE}/PIC32MK1024_APP_CODE.X.${IMAGE_TYPE}.elf dist/default/${IMAGE_TYPE}/hub_sp_1.1_scu_14.bin 