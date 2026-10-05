#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "definitions.h"
#include "middleware.h"
#include "software.h"

int main(void) {
    SYS_Initialize(NULL);

    GREEN_LED_Set();

    NVM_EEPROM_Initialize();

    TMR2_Start();

    while (1) {

    }
}