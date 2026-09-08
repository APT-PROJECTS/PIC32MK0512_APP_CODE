#include "definitions.h"
#include "driver.h"
#include "middleware.h"

#define COPY_FLOAT_TO_TX(offset, value)                                        \
  do {                                                                         \
    float temp = (value);                                                      \
    __builtin_memcpy((uint8_t *)(send_tuning_data + offset), &temp, 4);        \
  } while (0)

#define COPY_RX_TO_FLOAT(value, offset)                                        \
  do {                                                                         \
    float temp;                                                                \
    __builtin_memcpy(&temp, (uint8_t *)(read_tuning_data + offset), 4);        \
    (value) = temp;                                                            \
  } while (0)

#define SEND_CAN_FLOAT(value)                                                  \
  do {                                                                         \
    float temp = (value);                                                      \
    __builtin_memcpy(&data[4], &temp, sizeof(temp));                           \
    CANbus_write(CAN_ID_TUNER_MCU, data, sizeof(data));                        \
  } while (0)

static unsigned char devsn_string[18] = {0x20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x2F};

static uint16_t received_rows_mask = 0;
static bool all_rows_received = false;

static uint8_t read_tuning_data[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t send_tuning_data[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static int MCU_ID = 0;

void Tuner_Init(void) {
    received_rows_mask = 0;
    all_rows_received = false;
}

void Get_Config(void) {
    // Row 1
    R_FF01_01 = R_FF01_01;
    R_FF01_02 = CONFIG_H_IPH;
    R_FF01_03 = CONFIG_H_RPM;
    R_FF01_04 = CONFIG_POLES;

    // Row 2
    R_FF02_01 = CONFIG_RPH;
    R_FF02_02 = CONFIG_LD;
    R_FF02_03 = CONFIG_LQ;
    R_FF02_04 = CONFIG_V_CONSTANT;

    // Row 3
    R_FF03_01 = CONFIG_ZERO_ANGLE;
    R_FF03_02 = CONFIG_SENSOR_DIRECTION;
    R_FF03_03 = CONFIG_ZERO_ANGLE_ESTIMATE;
    R_FF03_04 = CONFIG_SENSOR_DIRECTION_ESTIMATE;

    // Row 4
    R_FF04_01 = CONFIG_ROTATION_DIRECTION;
    R_FF04_02 = CONFIG_CAN_BAUD;
    R_FF04_03 = CONFIG_RPM_TO_KMPH;
    R_FF04_04 = CONFIG_RPM_FAULT;

    // Row 5
    R_FF05_01 = CONFIG_MOTOR_DERATE;
    R_FF05_02 = CONFIG_MOTOR_TFAULT;
    R_FF05_03 = CONFIG_MCU_DERATE;
    R_FF05_04 = CONFIG_MCU_TFAULT;

    // Row 6
    R_FF06_01 = CONFIG_VDC_MAX;
    R_FF06_02 = CONFIG_IDC_DEFAULT;
    R_FF06_03 = CONFIG_OV_FAULT;
    R_FF06_04 = CONFIG_UV_FAULT;

    // Row 7
    R_FF07_01 = CONFIG_IDC_PMAX;
    R_FF07_02 = CONFIG_IDC_NMAX;
    R_FF07_03 = CONFIG_IPH_FAULT;
    R_FF07_04 = CONFIG_IDC_FAULT;

    // Row 8
    R_FF08_01 = CONFIG_MOTOR_MODE;
    R_FF08_02 = CONFIG_REVERSE_RPM;
    R_FF08_03 = CONFIG_L_RPM;
    R_FF08_04 = CONFIG_M_RPM;

    // Row 9
    R_FF09_01 = CONFIG_THROTTLE_ZERO;
    R_FF09_02 = CONFIG_THROTTLE_MAX;
    R_FF09_03 = CONFIG_GEN_VLIMIT;
    R_FF09_04 = CONFIG_AUTO_GEN;

    // Row 10
    R_FF0A_01 = CONFIG_RPM_KP;
    R_FF0A_02 = CONFIG_RPM_KI;
    R_FF0A_03 = CONFIG_L_ACCELERATION;
    R_FF0A_04 = CONFIG_M_ACCELERATION;

    // Row 11
    R_FF0B_01 = CONFIG_L_IDC;
    R_FF0B_02 = CONFIG_M_IDC;
    R_FF0B_03 = CONFIG_L_IPH;
    R_FF0B_04 = CONFIG_M_IPH;

    // Row 12
    R_FF0C_01 = CONFIG_BRAKE_IPH;
    R_FF0C_02 = CONFIG_BRAKE_ITIME;
    R_FF0C_03 = CONFIG_GEN_VMARGIN;
    R_FF0C_04 = CONFIG_NA1;

    // Row 13
    R_FF0D_01 = CONFIG_IPH_MAX;
    R_FF0D_02 = CONFIG_FMOTOR_MAX;
    R_FF0D_03 = CONFIG_MOTOR_TMAX;
    R_FF0D_04 = CONFIG_MCU_TMAX;

    // Row 14
    R_FF0E_01 = CONFIG_VMAX;
    R_FF0E_02 = CONFIG_VMIN;
    R_FF0E_03 = CONFIG_IDC_MAX;
    R_FF0E_04 = CONFIG_NA2;

    // Row 15
    R_FF0F_01 = CONFIG_RPM_KP_MIN;
    R_FF0F_02 = CONFIG_RPM_KP_MAX;
    R_FF0F_03 = CONFIG_RPM_KI_MIN;
    R_FF0F_04 = CONFIG_RPM_KI_MAX;
}

void Update_Config(void) {
    // Row 1
    R_FF01_01 = R_FF01_01;
    CONFIG_H_IPH = R_FF01_02;
    CONFIG_H_RPM = R_FF01_03;
    CONFIG_POLES = R_FF01_04;

    // Row 2
    CONFIG_RPH = R_FF02_01;
    CONFIG_LD = R_FF02_02;
    CONFIG_LQ = R_FF02_03;
    CONFIG_V_CONSTANT = R_FF02_04;

    // Row 3
    CONFIG_ZERO_ANGLE = R_FF03_01;
    CONFIG_SENSOR_DIRECTION = R_FF03_02;
    CONFIG_ZERO_ANGLE_ESTIMATE = R_FF03_03;
    CONFIG_SENSOR_DIRECTION_ESTIMATE = R_FF03_04;

    // Row 4
    CONFIG_ROTATION_DIRECTION = R_FF04_01;
    CONFIG_CAN_BAUD = R_FF04_02;
    CONFIG_RPM_TO_KMPH = R_FF04_03;
    CONFIG_RPM_FAULT = R_FF04_04;

    // Row 5
    CONFIG_MOTOR_DERATE = R_FF05_01;
    CONFIG_MOTOR_TFAULT = R_FF05_02;
    CONFIG_MCU_DERATE = R_FF05_03;
    CONFIG_MCU_TFAULT = R_FF05_04;

    // Row 6
    CONFIG_VDC_MAX = R_FF06_01;
    CONFIG_IDC_DEFAULT = R_FF06_02;
    CONFIG_OV_FAULT = R_FF06_03;
    CONFIG_UV_FAULT = R_FF06_04;

    // Row 7
    CONFIG_IDC_PMAX = R_FF07_01;
    CONFIG_IDC_NMAX = R_FF07_02;
    CONFIG_IPH_FAULT = R_FF07_03;
    CONFIG_IDC_FAULT = R_FF07_04;

    // Row 8
    CONFIG_MOTOR_MODE = R_FF08_01;
    CONFIG_REVERSE_RPM = R_FF08_02;
    CONFIG_L_RPM = R_FF08_03;
    CONFIG_M_RPM = R_FF08_04;

    // Row 9
    CONFIG_THROTTLE_ZERO = R_FF09_01;
    CONFIG_THROTTLE_MAX = R_FF09_02;
    CONFIG_GEN_VLIMIT = R_FF09_03;
    CONFIG_AUTO_GEN = R_FF09_04;

    // Row 10
    CONFIG_RPM_KP = R_FF0A_01;
    CONFIG_RPM_KI = R_FF0A_02;
    CONFIG_L_ACCELERATION = R_FF0A_03;
    CONFIG_M_ACCELERATION = R_FF0A_04;

    // Row 11
    CONFIG_L_IDC = R_FF0B_01;
    CONFIG_M_IDC = R_FF0B_02;
    CONFIG_L_IPH = R_FF0B_03;
    CONFIG_M_IPH = R_FF0B_04;

    // Row 12
    CONFIG_BRAKE_IPH = R_FF0C_01;
    CONFIG_BRAKE_ITIME = R_FF0C_02;
    CONFIG_GEN_VMARGIN = R_FF0C_03;
    CONFIG_NA1 = R_FF0C_04;

    // Row 13
    CONFIG_IPH_MAX = R_FF0D_01;
    CONFIG_FMOTOR_MAX = R_FF0D_02;
    CONFIG_MOTOR_TMAX = R_FF0D_03;
    CONFIG_MCU_TMAX = R_FF0D_04;

    // Row 14
    CONFIG_VMAX = R_FF0E_01;
    CONFIG_VMIN = R_FF0E_02;
    CONFIG_IDC_MAX = R_FF0E_03;
    CONFIG_NA2 = R_FF0E_04;

    // Row 15
    CONFIG_RPM_KP_MIN = R_FF0F_01;
    CONFIG_RPM_KP_MAX = R_FF0F_02;
    CONFIG_RPM_KI_MIN = R_FF0F_03;
    CONFIG_RPM_KI_MAX = R_FF0F_04;
}

/*=====================================================================
 * ======================UART BASED TUNING==============================
 ======================================================================*/

void UART_Tuner_Send_PIC_ID(void) {
    devsn_string[0] = 0x20;

    for (uint8_t i = 0; i < 4; i++) {
        devsn_string[(i * 4) + 1] = (devsn[i] >> 24) & 0xFF;
        devsn_string[(i * 4) + 2] = (devsn[i] >> 16) & 0xFF;
        devsn_string[(i * 4) + 3] = (devsn[i] >> 8) & 0xFF;
        devsn_string[(i * 4) + 4] = devsn[i] & 0xFF;
    }

    devsn_string[17] = 0x2F;

    send_data_uart(devsn_string, 18);
}

void UART_Tuner_Send_Zero_Angle(float theta, float theta0, float theta1,
        int CONFIG_SENSOR_DIRECTION_estimate) {
    R_FF03_03 = theta; // update the zero angle
    R_FF03_04 = CONFIG_SENSOR_DIRECTION_estimate; // update correct direction of sensor
    UART_Tuner_Read(0x03);
}

void UART_Tuner_Read(uint8_t adr_HB) {
    send_tuning_data[0] = 0xFF;
    send_tuning_data[1] = adr_HB;

    send_tuning_data[18] = 0xFF;
    send_tuning_data[19] = adr_HB;

    switch (adr_HB) {
        case 0x00:
            COPY_FLOAT_TO_TX(2, R_PROJECT_ID);
            COPY_FLOAT_TO_TX(6, R_FIRMWARE_ID);
            COPY_FLOAT_TO_TX(10, R_BOOT_FLAG);
            COPY_FLOAT_TO_TX(14, R_APP_SIZE);
            break;

        case 0x01:
            COPY_FLOAT_TO_TX(2, R_FF01_01);
            COPY_FLOAT_TO_TX(6, R_FF01_02);
            COPY_FLOAT_TO_TX(10, R_FF01_03);
            COPY_FLOAT_TO_TX(14, R_FF01_04);
            break;

        case 0x02:
            COPY_FLOAT_TO_TX(2, R_FF02_01);
            COPY_FLOAT_TO_TX(6, R_FF02_02);
            COPY_FLOAT_TO_TX(10, R_FF02_03);
            COPY_FLOAT_TO_TX(14, R_FF02_04);
            break;

        case 0x03:
            COPY_FLOAT_TO_TX(2, R_FF03_01);
            COPY_FLOAT_TO_TX(6, R_FF03_02);
            COPY_FLOAT_TO_TX(10, R_FF03_03);
            COPY_FLOAT_TO_TX(14, R_FF03_04);
            break;

        case 0x04:
            COPY_FLOAT_TO_TX(2, R_FF04_01);
            COPY_FLOAT_TO_TX(6, R_FF04_02);
            COPY_FLOAT_TO_TX(10, R_FF04_03);
            COPY_FLOAT_TO_TX(14, R_FF04_04);
            break;

        case 0x05:
            COPY_FLOAT_TO_TX(2, R_FF05_01);
            COPY_FLOAT_TO_TX(6, R_FF05_02);
            COPY_FLOAT_TO_TX(10, R_FF05_03);
            COPY_FLOAT_TO_TX(14, R_FF05_04);
            break;

        case 0x06:
            COPY_FLOAT_TO_TX(2, R_FF06_01);
            COPY_FLOAT_TO_TX(6, R_FF06_02);
            COPY_FLOAT_TO_TX(10, R_FF06_03);
            COPY_FLOAT_TO_TX(14, R_FF06_04);
            break;

        case 0x07:
            COPY_FLOAT_TO_TX(2, R_FF07_01);
            COPY_FLOAT_TO_TX(6, R_FF07_02);
            COPY_FLOAT_TO_TX(10, R_FF07_03);
            COPY_FLOAT_TO_TX(14, R_FF07_04);
            break;

        case 0x08:
            COPY_FLOAT_TO_TX(2, R_FF08_01);
            COPY_FLOAT_TO_TX(6, R_FF08_02);
            COPY_FLOAT_TO_TX(10, R_FF08_03);
            COPY_FLOAT_TO_TX(14, R_FF08_04);
            break;

        case 0x09:
            COPY_FLOAT_TO_TX(2, R_FF09_01);
            COPY_FLOAT_TO_TX(6, R_FF09_02);
            COPY_FLOAT_TO_TX(10, R_FF09_03);
            COPY_FLOAT_TO_TX(14, R_FF09_04);
            break;

        case 0x0A:
            COPY_FLOAT_TO_TX(2, R_FF0A_01);
            COPY_FLOAT_TO_TX(6, R_FF0A_02);
            COPY_FLOAT_TO_TX(10, R_FF0A_03);
            COPY_FLOAT_TO_TX(14, R_FF0A_04);
            break;

        case 0x0B:
            COPY_FLOAT_TO_TX(2, R_FF0B_01);
            COPY_FLOAT_TO_TX(6, R_FF0B_02);
            COPY_FLOAT_TO_TX(10, R_FF0B_03);
            COPY_FLOAT_TO_TX(14, R_FF0B_04);
            break;

        case 0x0C:
            COPY_FLOAT_TO_TX(2, R_FF0C_01);
            COPY_FLOAT_TO_TX(6, R_FF0C_02);
            COPY_FLOAT_TO_TX(10, R_FF0C_03);
            COPY_FLOAT_TO_TX(14, R_FF0C_04);
            break;

        case 0x0D:
            COPY_FLOAT_TO_TX(2, R_FF0D_01);
            COPY_FLOAT_TO_TX(6, R_FF0D_02);
            COPY_FLOAT_TO_TX(10, R_FF0D_03);
            COPY_FLOAT_TO_TX(14, R_FF0D_04);
            break;

        case 0x0E:
            COPY_FLOAT_TO_TX(2, R_FF0E_01);
            COPY_FLOAT_TO_TX(6, R_FF0E_02);
            COPY_FLOAT_TO_TX(10, R_FF0E_03);
            COPY_FLOAT_TO_TX(14, R_FF0E_04);
            break;

        case 0x0F:
            COPY_FLOAT_TO_TX(2, R_FF0F_01);
            COPY_FLOAT_TO_TX(6, R_FF0F_02);
            COPY_FLOAT_TO_TX(10, R_FF0F_03);
            COPY_FLOAT_TO_TX(14, R_FF0F_04);
            break;

        default:
            send_tuning_data[18] = 0xF1;
            break;
    }

    send_data_uart(send_tuning_data, 20);
}

void UART_Tuner_Write(void) {
    //--------------------------------------------------
    // Store 4 float values into write matrix
    //--------------------------------------------------
    switch (read_tuning_data[1]) {
        case 0x01:
            COPY_RX_TO_FLOAT(W_FF01_01, 2);
            COPY_RX_TO_FLOAT(W_FF01_02, 6);
            COPY_RX_TO_FLOAT(W_FF01_03, 10);
            COPY_RX_TO_FLOAT(W_FF01_04, 14);
            break;

        case 0x02:
            COPY_RX_TO_FLOAT(W_FF02_01, 2);
            COPY_RX_TO_FLOAT(W_FF02_02, 6);
            COPY_RX_TO_FLOAT(W_FF02_03, 10);
            COPY_RX_TO_FLOAT(W_FF02_04, 14);
            break;

        case 0x03:
            COPY_RX_TO_FLOAT(W_FF03_01, 2);
            COPY_RX_TO_FLOAT(W_FF03_02, 6);
            COPY_RX_TO_FLOAT(W_FF03_03, 10);
            COPY_RX_TO_FLOAT(W_FF03_04, 14);
            break;

        case 0x04:
            COPY_RX_TO_FLOAT(W_FF04_01, 2);
            COPY_RX_TO_FLOAT(W_FF04_02, 6);
            COPY_RX_TO_FLOAT(W_FF04_03, 10);
            COPY_RX_TO_FLOAT(W_FF04_04, 14);
            break;

        case 0x05:
            COPY_RX_TO_FLOAT(W_FF05_01, 2);
            COPY_RX_TO_FLOAT(W_FF05_02, 6);
            COPY_RX_TO_FLOAT(W_FF05_03, 10);
            COPY_RX_TO_FLOAT(W_FF05_04, 14);
            break;

        case 0x06:
            COPY_RX_TO_FLOAT(W_FF06_01, 2);
            COPY_RX_TO_FLOAT(W_FF06_02, 6);
            COPY_RX_TO_FLOAT(W_FF06_03, 10);
            COPY_RX_TO_FLOAT(W_FF06_04, 14);
            break;

        case 0x07:
            COPY_RX_TO_FLOAT(W_FF07_01, 2);
            COPY_RX_TO_FLOAT(W_FF07_02, 6);
            COPY_RX_TO_FLOAT(W_FF07_03, 10);
            COPY_RX_TO_FLOAT(W_FF07_04, 14);
            break;

        case 0x08:
            COPY_RX_TO_FLOAT(W_FF08_01, 2);
            COPY_RX_TO_FLOAT(W_FF08_02, 6);
            COPY_RX_TO_FLOAT(W_FF08_03, 10);
            COPY_RX_TO_FLOAT(W_FF08_04, 14);
            break;

        case 0x09:
            COPY_RX_TO_FLOAT(W_FF09_01, 2);
            COPY_RX_TO_FLOAT(W_FF09_02, 6);
            COPY_RX_TO_FLOAT(W_FF09_03, 10);
            COPY_RX_TO_FLOAT(W_FF09_04, 14);
            break;

        case 0x0A:
            COPY_RX_TO_FLOAT(W_FF0A_01, 2);
            COPY_RX_TO_FLOAT(W_FF0A_02, 6);
            COPY_RX_TO_FLOAT(W_FF0A_03, 10);
            COPY_RX_TO_FLOAT(W_FF0A_04, 14);
            break;

        case 0x0B:
            COPY_RX_TO_FLOAT(W_FF0B_01, 2);
            COPY_RX_TO_FLOAT(W_FF0B_02, 6);
            COPY_RX_TO_FLOAT(W_FF0B_03, 10);
            COPY_RX_TO_FLOAT(W_FF0B_04, 14);
            break;

        case 0x0C:
            COPY_RX_TO_FLOAT(W_FF0C_01, 2);
            COPY_RX_TO_FLOAT(W_FF0C_02, 6);
            COPY_RX_TO_FLOAT(W_FF0C_03, 10);
            COPY_RX_TO_FLOAT(W_FF0C_04, 14);
            break;

        default:
            return;
    }

    //--------------------------------------------------
    // Prepare EEPROM write buffer with the complete matrix.
    //--------------------------------------------------
    memset(eepromBuffer, 0xFF, EEPROM_DATA_SIZE);

    memcpy(eepromBuffer, writeMatrix, sizeof (writeMatrix));

    //--------------------------------------------------
    // Write complete matrix to EEPROM so this row survives reset.
    //--------------------------------------------------
    if (FLASH_EEPROM_Write_Page(eepromBuffer, sizeof (writeMatrix)) ==
            FLASH_EEPROM_OK) {
        GREEN_LED_Set();

        //--------------------------------------------------
        // Update runtime matrix
        //--------------------------------------------------
        // memcpy(readMatrix, writeMatrix, sizeof (readMatrix));

        all_rows_received = true;

        //--------------------------------------------------
        // ACK only after the row has been committed.
        //--------------------------------------------------
        send_data_uart(&read_tuning_data[0], 20);
    } else {
        //        GREEN_LED_Set();
    }
}

void UART_Tuner_Command(uint8_t *command_adr) {
    //--------------------------------------------------
    // Copy full command
    //--------------------------------------------------
    memcpy(read_tuning_data, command_adr, 20);

    //--------------------------------------------------
    // WRITE COMMAND
    //--------------------------------------------------
    if (read_tuning_data[18] == 0xFF) {
        //--------------------------------------------------
        // Validate packet
        //--------------------------------------------------
        if (read_tuning_data[1] == read_tuning_data[19]) {
            //--------------------------------------------------
            // Store row
            //--------------------------------------------------
            UART_Tuner_Write();
        }
    }//--------------------------------------------------
        // READ COMMAND
        //--------------------------------------------------
    else {
        UART_Tuner_Read(read_tuning_data[1]);
    }
}

/*=====================================================================
 * ======================CAN BASED TUNING==============================
 ======================================================================*/

void CAN_Tuner_Send_PIC_ID_1(void) {
    uint8_t txBuf[8];

    // devsn[0]
    for (int i = 0; i < 4; i++)
        txBuf[i] = (devsn[0] >> (24 - i * 8)) & 0xFF;

    // devsn[1]
    for (int i = 0; i < 4; i++)
        txBuf[i + 4] = (devsn[1] >> (24 - i * 8)) & 0xFF;

    CANbus_write(CAN_ID_TUNER_MCU, txBuf, sizeof (txBuf));
}

void CAN_Tuner_Send_PIC_ID_2(void) {
    uint8_t txBuf[8];

    // devsn[2]
    for (int i = 0; i < 4; i++)
        txBuf[i] = (devsn[2] >> (24 - i * 8)) & 0xFF;

    // devsn[3]
    for (int i = 0; i < 4; i++)
        txBuf[i + 4] = (devsn[3] >> (24 - i * 8)) & 0xFF;

    CANbus_write(CAN_ID_TUNER_MCU, txBuf, sizeof (txBuf));
}

void CAN_Tuner_Read(uint8_t row, uint8_t col) {
    uint8_t data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    data[0] = 0x00;
    data[1] = 0xFF;

    data[2] = row;
    data[3] = col;

    switch (row) {
        case 0x00:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_PROJECT_ID);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FIRMWARE_ID);
                    break;
                default: break;
            }
            break;

        case 0x01:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF01_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF01_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF01_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF01_04);
                    break;
                default: break;
            }
            break;

        case 0x02:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF02_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF02_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF02_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF02_04);
                    break;
                default: break;
            }
            break;

        case 0x03:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF03_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF03_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF03_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF03_04);
                    break;
                default: break;
            }
            break;

        case 0x04:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF04_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF04_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF04_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF04_04);
                    break;
                default: break;
            }
            break;

        case 0x05:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF05_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF05_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF05_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF05_04);
                    break;
                default: break;
            }
            break;

        case 0x06:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF06_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF06_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF06_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF06_04);
                    break;
                default: break;
            }
            break;

        case 0x07:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF07_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF07_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF07_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF07_04);
                    break;
                default: break;
            }
            break;

        case 0x08:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF08_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF08_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF08_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF08_04);
                    break;
                default: break;
            }
            break;

        case 0x09:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF09_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF09_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF09_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF09_04);
                    break;
                default: break;
            }
            break;

        case 0x0A:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF0A_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF0A_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF0A_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF0A_04);
                    break;
                default: break;
            }
            break;

        case 0x0B:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF0B_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF0B_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF0B_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF0B_04);
                    break;
                default: break;
            }
            break;

        case 0x0C:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF0C_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF0C_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF0C_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF0C_04);
                    break;
                default: break;
            }
            break;

        case 0x0D:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF0D_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF0D_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF0D_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF0D_04);
                    break;
                default: break;
            }
            break;

        case 0x0E:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF0E_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF0E_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF0E_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF0E_04);
                    break;
                default: break;
            }
            break;

        case 0x0F:
            switch (col) {
                case 0x01: SEND_CAN_FLOAT(R_FF0F_01);
                    break;
                case 0x02: SEND_CAN_FLOAT(R_FF0F_02);
                    break;
                case 0x03: SEND_CAN_FLOAT(R_FF0F_03);
                    break;
                case 0x04: SEND_CAN_FLOAT(R_FF0F_04);
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
}

void CAN_Tuner_Write_Matrix(uint8_t row, uint8_t col, uint8_t *data) {
    float temp;

    __builtin_memcpy(&temp, &data[4], sizeof (temp));

    switch (row) {
        case 0x01:
            switch (col) {
                case 0x01: W_FF01_01 = temp;
                    break;
                case 0x02: W_FF01_02 = temp;
                    break;
                case 0x03: W_FF01_03 = temp;
                    break;
                case 0x04: W_FF01_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x02:
            switch (col) {
                case 0x01: W_FF02_01 = temp;
                    break;
                case 0x02: W_FF02_02 = temp;
                    break;
                case 0x03: W_FF02_03 = temp;
                    break;
                case 0x04: W_FF02_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x03:
            switch (col) {
                case 0x01: W_FF03_01 = temp;
                    break;
                case 0x02: W_FF03_02 = temp;
                    break;
                case 0x03: W_FF03_03 = temp;
                    break;
                case 0x04: W_FF03_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x04:
            switch (col) {
                case 0x01: W_FF04_01 = temp;
                    break;
                case 0x02: W_FF04_02 = temp;
                    break;
                case 0x03: W_FF04_03 = temp;
                    break;
                case 0x04: W_FF04_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x05:
            switch (col) {
                case 0x01: W_FF05_01 = temp;
                    break;
                case 0x02: W_FF05_02 = temp;
                    break;
                case 0x03: W_FF05_03 = temp;
                    break;
                case 0x04: W_FF05_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x06:
            switch (col) {
                case 0x01: W_FF06_01 = temp;
                    break;
                case 0x02: W_FF06_02 = temp;
                    break;
                case 0x03: W_FF06_03 = temp;
                    break;
                case 0x04: W_FF06_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x07:
            switch (col) {
                case 0x01: W_FF07_01 = temp;
                    break;
                case 0x02: W_FF07_02 = temp;
                    break;
                case 0x03: W_FF07_03 = temp;
                    break;
                case 0x04: W_FF07_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x08:
            switch (col) {
                case 0x01: W_FF08_01 = temp;
                    break;
                case 0x02: W_FF08_02 = temp;
                    break;
                case 0x03: W_FF08_03 = temp;
                    break;
                case 0x04: W_FF08_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x09:
            switch (col) {
                case 0x01: W_FF09_01 = temp;
                    break;
                case 0x02: W_FF09_02 = temp;
                    break;
                case 0x03: W_FF09_03 = temp;
                    break;
                case 0x04: W_FF09_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x0A:
            switch (col) {
                case 0x01: W_FF0A_01 = temp;
                    break;
                case 0x02: W_FF0A_02 = temp;
                    break;
                case 0x03: W_FF0A_03 = temp;
                    break;
                case 0x04: W_FF0A_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x0B:
            switch (col) {
                case 0x01: W_FF0B_01 = temp;
                    break;
                case 0x02: W_FF0B_02 = temp;
                    break;
                case 0x03: W_FF0B_03 = temp;
                    break;
                case 0x04: W_FF0B_04 = temp;
                    break;
                default: return;
            }
            break;

        case 0x0C:
            switch (col) {
                case 0x01: W_FF0C_01 = temp;
                    break;
                case 0x02: W_FF0C_02 = temp;
                    break;
                case 0x03: W_FF0C_03 = temp;
                    break;
                case 0x04: W_FF0C_04 = temp;
                    break;
                default: return;
            }
            break;

        default:
            return;
    }

    CANbus_write(CAN_ID_TUNER_MCU, data, 8);
}

void CAN_Tuner_Write_Flash(void) {
    //--------------------------------------------------
    // Prepare EEPROM write buffer with the complete matrix.
    //--------------------------------------------------
    memset(eepromBuffer, 0xFF, EEPROM_DATA_SIZE);
    memcpy(eepromBuffer, writeMatrix, sizeof (writeMatrix));
    FLASH_EEPROM_Write_Page(eepromBuffer, sizeof (writeMatrix));
}

void CAN_Send_Status(void) {
    static int ctr = 0;
    static uint8_t data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    data[0] = CAN_CMD_TUNER_STATUS;
    ctr++;

    if (ctr >= 5000) {
        ctr = 0;
        CANbus_write(CAN_ID_TUNER_MCU, data, 8);
    }
}

void CAN_Tuner_Send_Zero_Angle(float theta, float theta0, float theta1,
        int CONFIG_SENSOR_DIRECTION_estimate) {
    static uint8_t data[8];
    static float temp;

    R_FF03_03 = theta; // update the zero angle
    R_FF03_04 = CONFIG_SENSOR_DIRECTION_estimate; // update correct direction of sensor

    data[0] = 0x00;
    data[1] = 0xFF;
    data[2] = 0x03;

    data[3] = 0x01;
    temp = R_FF03_01;
    memcpy(&data[4], &temp, sizeof (temp));
    CANbus_write(CAN_ID_TUNER_MCU, data, sizeof (data));

    data[3] = 0x02;
    temp = R_FF03_02;
    memcpy(&data[4], &temp, sizeof (temp));
    CANbus_write(CAN_ID_TUNER_MCU, data, sizeof (data));

    data[3] = 0x03;
    temp = R_FF03_03;
    memcpy(&data[4], &temp, sizeof (temp));
    CANbus_write(CAN_ID_TUNER_MCU, data, sizeof (data));

    data[3] = 0x04;
    temp = R_FF03_04;
    memcpy(&data[4], &temp, sizeof (temp));
    CANbus_write(CAN_ID_TUNER_MCU, data, sizeof (data));
}

void CAN_Tuner_Handler(uint8_t *CANRXBuffer) {
    uint8_t row = CANRXBuffer[2];
    uint8_t col = CANRXBuffer[3];

    switch (CANRXBuffer[0]) {
        case CAN_CMD_TUNER_PIC_ID_1:
        {
            CAN_Tuner_Send_PIC_ID_1();
            break;
        }

        case CAN_CMD_TUNER_PIC_ID_2:
        {
            CAN_Tuner_Send_PIC_ID_2();
            break;
        }

        case CAN_CMD_TUNER_READ:
        {
            CAN_Tuner_Read(row, col);
            break;
        }

        case CAN_CMD_TUNER_WRITE_MATRIX:
        {
            CAN_Tuner_Write_Matrix(row, col, CANRXBuffer);
            break;
        }

        case CAN_CMD_TUNER_WRITE_FLASH:
        {
            CAN_Tuner_Write_Flash();
            break;
        }

        default:
            // Unknown tuner command
            break;
    }
}