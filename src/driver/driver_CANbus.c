#include <sys/kmem.h>
#include "definitions.h"
#include "driver.h"
#include "middleware.h"

#define CANSTB_PIN LATCbits.LATC15

void CANbus_init(void) {
    CANSTB_PIN = 0; //Enable CABbus transreciever
}

void CANbus_write(uint32_t Sadr, uint8_t * Sdata, uint8_t Sdata_L) {
    bool sstat = 0;
    sstat = CAN1_MessageTransmit(Sadr, Sdata_L, Sdata, 1, 0, 0);
    sstat = sstat;
}

void CANbus_read(void) {
    uint8_t Rdata[8];
    bool rstat = 0;
    uint8_t msg_len = 0;
    uint32_t msg_id = 0;
    uint32_t timestamp = 0;
    CANFD_MSG_RX_ATTRIBUTE atri = 0;

    rstat = CAN1_MessageReceive(&msg_id, &msg_len, &Rdata[0], &timestamp, 2, &atri);

    if (rstat) {
        rstat = 0;

        switch (msg_id) {

            case 0xC0:
            {
                C0_send_TU_data_CAN();
                break;
            }

            case CAN_ID_TUNER_SOFTWARE:
            {
                CAN_Tuner_Handler(Rdata);
                break;
            }
            
            case BOOT_CAN_ID_HOST:
            {
                Run_Boot_Handler(msg_id, Rdata);
                break;
            }

            default:
                break;
        }
    }
    Nop();
}