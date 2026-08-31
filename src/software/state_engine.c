#include <math.h>
#include "definitions.h"
#include "state_engine.h"
#include "../driver/driver_UART.h"
#include "../driver/driver_CANbus.h"
#include "../middleware/tuning.h"
#include "../middleware/mcu_config.h"

#define SLOW_ROUTINE_COUNT 5

#define FAULT 0
#define CALIBERATION 1
#define STOP 2
#define PMSM 3
#define ROTOR_HOLD 4

#define OFF 0
#define RPM_CONTROL 3
#define TORQUE_CONTROL 2
#define VEHICLE_CONTROL 1
#define ROTOR_POS_ID 4

//--------------------------------------------------
// STATIC STATE VARIABLE
//--------------------------------------------------
static STATE_ENGINE_STATES currentState = STATE_INIT;

//state variables===============================================================

static int system_state = CALIBERATION;
static uint32_t fault_code = 0;
static int slow_routine_counter = 0;

//motor variables===============================================================	
static int poles = 10;
static float Vauxin = 0;
static float Vdc = 0, Ibat = 0, fIbat = 0;
static float Iu = 0, Iv = 0, Iw = 0;
static float cosMR, sinMR;
static float cosHold = 0.816496610641479, sinHold = 0;
static float MRcos0, MRsin0;
static float wMR = 0;

static float Tm = 0, Tesc = 0;
static float Imosfet = 200;

//control variables=============================================================	    
static float dt = 0.00004;
static float du = 0, dv = 0, dw = 0;
static float dutyD = 0, dutyQ = 0;
static float OV_fault = 60.0, UV_fault = 36.0, Ibat_fault = 400.0, Iph_fault = 500.0, ESC_TfaultC = 110, motor_TfaultC = 110.0;
static float rpm_fault = 10000;

static float kp_sless_con = 20.0, ki_sless_con = 200.0;
static float dw_sless_sat = 2000;
static float kp_icon = 0.0005, ki_icon = 0.2;
static float duty_sat = 0.9, dq_duty_sat = 1.41;

static float a_input = 0, b_input = 0;
static float Idref = 5.0, Iqref = 0;
static float Id = 0, Iq = 0;
static float Ed = 0, Eq = 0;
static int drive_state = 0;

static float rpm_to_speed = 1;

static float Iph_act = 0;

static float kstall = 1;

static float w_actt = 0;

static uint32_t caliberation_counter = 0;

static float Tfault_motorC = 110, Tfault_ESCC = 110;

static float abs_max_rpm = 7000;

static float Id_desired = 0, Iq_desired = 0;
static float rpm = 0, torque = 0, abs_rpm;
static float Id_desired_integrator = 0;

static float Id_rms = 0, Iq_rms = 0;
static float Id_limit = 50, Iq_limit = 50;
static float PfId_limit = 0, NfId_limit = 0;
static float Iph_limit = 100, Ibat_limit = 50;

static float Ilimit = 8, rpm_limit = 650;

static float k_brake = 1.0;

static float Iph_actual = 0;

static float pdi = 0.2, ndi = 0.2;

static int Vmode = 2, Immob = 0, reverse = 0;

static int CAN_baud = 500;

static float fCONFIG_RPM_KP = 0.1;
static float fCONFIG_RPM_KI = 0.1;

static float stall_Ilimit = 100;

static float Ibat_tune = 100;

static float IdisL = 0, IcrgL = 0;
static long bat_can_timer = 0;

static int cruise_mode = 0;

static float Vdc_act = 60;

void state_init(void) //To be called first in main()
{
    Load_Read_Matrix();
    Tuner_Init();

    if (R_FF04_01 == 1.0f) //for reversing the direction of rotation
    {
        R_FF03_01 = 180.0f - R_FF03_01;
        if (R_FF03_02 == 1.0f)
            R_FF03_02 = 0.0f;
        else
            R_FF03_02 = 1.0f;
    }

    MRcos0 = cos(R_FF03_01 * 3.14159265 / 180);
    MRsin0 = sin(R_FF03_01 * 3.14159265 / 180);

    drive_init(&poles, &CONFIG_THROTTLE_ZERO, &CONFIG_THROTTLE_MAX, motor_TfaultC, ESC_TfaultC, &rpm_to_speed);
}

void drive_init(int * p_adr, float * CONFIG_THROTTLE_ZERO_adr, float * CONFIG_THROTTLE_MAX_adr, float motor_TfaultC, float ESC_TfaultC, float * rpm_to_speed_adr) {
    *p_adr = poles;
    *CONFIG_THROTTLE_ZERO_adr = CONFIG_THROTTLE_ZERO;
    *CONFIG_THROTTLE_MAX_adr = CONFIG_THROTTLE_MAX;
    abs_max_rpm = fabs(CONFIG_H_RPM);
    Tfault_motorC = motor_TfaultC;
    Tfault_ESCC = ESC_TfaultC;
    *rpm_to_speed_adr = CONFIG_RPM_TO_KMPH;
    rpm_limit = CONFIG_H_RPM;
    Ibat_limit = CONFIG_IDC_PMAX;
    IdisL = CONFIG_IDC_DEFAULT;
    IcrgL = CONFIG_IDC_NMAX;
    Ibat_tune = CONFIG_IDC_PMAX;
    Iph_limit = CONFIG_H_IPH;
    stall_Ilimit = CONFIG_H_IPH;
}

//--------------------------------------------------
// STATE MACHINE TASKS
//--------------------------------------------------

void STATE_ENGINE_Tasks(void) {
    static int ctr = 0, v2 = 0;
    static float v1 = 0.0;

    switch (currentState) {
        case STATE_INIT:
        {
            state_init();
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
