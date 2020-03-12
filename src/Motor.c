////////////////////////////////////////////////////////////////////
// HEADER FILES - Master & Motor module headers.                  //
////////////////////////////////////////////////////////////////////
#include "includes.h"
#include "Motor.h"

////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES - Motor Module function prototypes.        //
////////////////////////////////////////////////////////////////////
void MotorInit(void);
INT8U MotorEnable2s(void);
void MotorOut(void);
void MotorIn(void);

////////////////////////////////////////////////////////////////////
// MODULE VARIABLES - Motor Module Variables.                     //
////////////////////////////////////////////////////////////////////
INT8U Enabled;
typedef enum {
    PUSH,
    PULL
} MOTOR_STATE;
MOTOR_STATE MotorState = PULL;

////////////////////////////////////////////////////////////////////
// MotorInit  - Initializes motor timer & set to in state         //
// Parameters - None                                              //
// Return     - None                                              //
////////////////////////////////////////////////////////////////////
void MotorInit(void)
{
    // Set as output for IN1, IN2, EN on H-Bridge
    P2DIR |= 0x23;

    // Initialize Timer A.
    BCSCTL3 = LFXT1S1;                        //Select VLOCLK for ACLCK (i.e. 12khz intosc)
    TACCTL0 = OUTMOD_2 + CCIE;                // TACCR0 interrupt enabled

    // This returns motor to unlocked state on init
    MOTOR_UNLOCK;
    MOTOR_ENABLE;
    TACCR0 = TACCR0_INIT;
    TACTL = TASSEL_1 + ID1 + ID0 + MC_1;    // ACLCK, 1/40 DIVIDER, upmode to TCCR0 value
}

////////////////////////////////////////////////////////////////////
// MotorEnable2s - Initializes motor timer & set to in state      //
// Parameters    - None                                           //
// Return        - INT8U: Enable outcome                          //
////////////////////////////////////////////////////////////////////
INT8U MotorEnable2s(void)
{
    // Motor is already moving
    if (Enabled == TRUE) {
        return FALSE;
    }
    MOTOR_ENABLE;
    Enabled = TRUE;
    TAR = 0;                                // Set the timer.
    TACCR0 = TACCR0_CNT;
    // Increase in time.
    if (MotorState == PULL) {
        TACCR0 += 20;
    }
    TACTL = TASSEL_1 + ID1 + ID0 + MC_1;    // ACLCK, 1/40 DIVIDER, upmode to TCCR0 value

    return TRUE;
}

////////////////////////////////////////////////////////////////////
// MotorIn    - Puts the motor to the in state                    //
// Parameters - None                                              //
// Return     - None                                              //
////////////////////////////////////////////////////////////////////
void MotorIn(void)
{
    if (Enabled != TRUE) {
        MOTOR_UNLOCK;
        MotorState = PULL;
    }
}

////////////////////////////////////////////////////////////////////
// MotorOut   - Puts the motor to the out state                   //
// Parameters - None                                              //
// Return     - None                                              //
////////////////////////////////////////////////////////////////////
void MotorOut(void)
{
    if (Enabled != TRUE) {
        MOTOR_LOCK;
        MotorState = PUSH;
    }
}

////////////////////////////////////////////////////////////////////
// Timer_A    - Disables the motor when timer hits                //
// Parameters - None                                              //
// Return     - __interrupt                                       //
////////////////////////////////////////////////////////////////////
#pragma vector=TIMER0_A0_VECTOR        // Timer A0 interrupt service routine
__interrupt void Timer_A (void)
{
    TACTL = MC_0;        // Timer Halted
    MOTOR_DISABLE;        // Disable Motor
    Enabled = FALSE;
}
