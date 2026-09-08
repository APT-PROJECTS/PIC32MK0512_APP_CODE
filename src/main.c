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