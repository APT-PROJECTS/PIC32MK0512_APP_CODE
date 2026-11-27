#include "definitions.h"
#include "driver.h"
#include "middleware.h"
#include "software.h"

//--------------------------------------------------
// STATIC STATE VARIABLE
//--------------------------------------------------
static STATE_ENGINE_STATES currentState = STATE_INIT;

//--------------------------------------------------
// STATE MACHINE TASKS
//--------------------------------------------------

void STATE_ENGINE_Tasks(void) {
    static int ctr = 0, v2 = 0;
    static float v1 = 0.0;

    switch (currentState) {
        case STATE_INIT:
        {
            FLASH_EEPROM_Init();
            Metadata_Init();
            Tuner_Init();
            currentState = STATE_RUN;
            break;
        }

        case STATE_RUN:
        {
            recieve_command_uart();
            send_byte_uart();
            CANbus_read();
            CAN_Send_Status();

            ctr++;
            if (ctr >= 10000) {
                UART_Tuner_Send_Zero_Angle(v1, 0.0, 0.0, v2);
                CAN_Tuner_Send_Zero_Angle(v1, 0.0, 0.0, v2);
                ctr = 0;
                v1 = (v1 < 360.0) ? (v1 + 12.0) : 0.0;
                v2 = (v2 == 0) ? 1 : 0;
            }
            break;
        }

        default:
            break;
    }
}
