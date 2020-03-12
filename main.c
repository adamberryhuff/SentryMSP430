////////////////////////////////////////////////////////////////////
// HEADER FILES - Master, UART, Motor, OS.                        //
////////////////////////////////////////////////////////////////////
#include "includes.h"
#include "UART.h"
#include "Motor.h"
#include "mss/mss.h"

////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES                                            //
////////////////////////////////////////////////////////////////////
void main(void);
void ControlTask(void *param);
static void InitControlTasks(void);

////////////////////////////////////////////////////////////////////
// TASK INSTANCES - Task State & Task Control Timer.              //
////////////////////////////////////////////////////////////////////
struct TaskInstance ControlTaskInst = {MSS_TASK_CTX_STATE_INIT_VAL, MSS_TIMER_INVALID_HDL};

////////////////////////////////////////////////////////////////////
// TASK INSTANCES - Task State & Task Control Timer.              //
////////////////////////////////////////////////////////////////////
typedef enum {
	INVALID,
	VALID
} SECURITY_STATE;
LockState LockState = INIT;
INT8U SendKey = FALSE;
INT8U Manual = FALSE;
INT8U SecurityKeyStr[] = "1X5u!j8*";

////////////////////////////////////////////////////////////////////
// main       - Instantiates modules and OS                       //
// Parameters - None	                                          //
// Return     - None											  //
////////////////////////////////////////////////////////////////////
void main(void) {
	WATCHDOG_STOP;							// Stop watchdog timer

	// Instantiate UART, Motor, and OS
	UARTInit();
	MotorInit();
	mss_init();

	// Key code push button code
	P1DIR &= ~0x80; // Input P1.7
	P1IE |= 0x80;   // P1.7 Interrupt Enable
	P1IES |= 0x80;  // High-to-low interrupt
	
	// Manual Override interrupt
	P2DIR &= ~0x10; // Input P1.7
	P2IE |= 0x10;   // P1.7 Interrupt Enable
	P2IES |= 0x10;  // High-to-low interrupt

    InitControlTasks();					    // Instantiate tasks

    mss_run();								// Run the OS

    // Enable low power mode with interrupts
    LOW_POWER_ISR;
}

////////////////////////////////////////////////////////////////////
// ControlTask - Checks for new user input and toggles motor      //
// Parameters  - void *param: Optional parameter (unused)	      //
// Return     - None											  //
////////////////////////////////////////////////////////////////////
void ControlTask(void *param) {
	MSS_BEGIN(ControlTaskInst.State);

	// Incoming character & lock state
	static INT8U ch;
	static INT8U rx_buffer[RX_BUFFER_LEN];
	static INT8U buffer_index = 0;
	static INT8U last_char = 0x00;
	SECURITY_STATE security_state = INVALID;
	INT8U cmp_index;

    FOREVER() {
    	ch = UARTGetChar();

    	// Store char in buffer
    	if (ch != last_char) {
    		rx_buffer[buffer_index] = ch;

    		// Send back current state
    		if (ch == STATE_CHECK_CH) {
    			if (LockState == LOCKED) {
    				UARTPutChar('l');
    			} else if (LockState == UNLOCKED || LockState == INIT) {
    				UARTPutChar('u');
    			} else {}
    			// reset index
    			buffer_index = 0;
    		}

    		// Buffer full
    		if (buffer_index == RX_BUFFER_LEN-1) {
    			security_state = VALID;

    			// Validate key
    			for (cmp_index = 0; cmp_index < KEY_LEN; cmp_index++) {
    				if (rx_buffer[cmp_index] != SECURITY_KEY[cmp_index]) {
    					security_state = INVALID;
    				}
    			}

    			// Lock and Unlock States
    			if (rx_buffer[buffer_index] == 'l' 
					&& (LockState == UNLOCKED || LockState == INIT) 
					&& security_state == VALID
				) {
    				MotorOut();
    				MotorEnable2s();
    				UARTPutChar(ch);
    				LockState = LOCKED;
    				// State change: 1.5 seconds delay
    				MSS_TIMER_DELAY_MS(
						ControlTaskInst.Timer, CH_MOTOR_STATE, ControlTaskInst.State
					);
    			} else if (rx_buffer[buffer_index] == 'u' && LockState == LOCKED 
					&& security_state == VALID
				) {
    			    MotorIn();
    				MotorEnable2s();
    				UARTPutChar(ch);
    				LockState = UNLOCKED;
    				// State change: 1.5 seconds delay
    				MSS_TIMER_DELAY_MS(
						ControlTaskInst.Timer, CH_MOTOR_STATE+50, ControlTaskInst.State
					);
    			} else {}
    			buffer_index = 0;
    			last_char = ch;
    		} else if (ch != STATE_CHECK_CH) {
    			buffer_index++;
    		} else {}
    		last_char = ch;
    	}
		
		// Pairing button pressed.
    	if (SendKey == TRUE) {
    		UARTPutChar('s');
    		for (cmp_index = 0; cmp_index < RX_BUFFER_LEN-1; cmp_index++) {
    			MSS_TIMER_DELAY_MS(
					ControlTaskInst.Timer, CNTL_TSK_FREQ-25, ControlTaskInst.State
				);
    			UARTPutChar(SecurityKeyStr[i]);
    		}
			MSS_TIMER_DELAY_MS(ControlTaskInst.Timer, CNTL_TSK_FREQ, ControlTaskInst.State);
			SendKey = FALSE;
			buffer_index = 0;
		}
		
		// Manual Override Activated
		if (Manual == TRUE) {
			if (LockState == LOCKED) {
				MotorIn();
				MotorEnable2s();
				UARTPutChar('u');
				LockState = UNLOCKED;
			} else if (LockState == UNLOCKED) {
				MotorOut();
				MotorEnable2s();
				UARTPutChar('l');
				LockState = LOCKED;
			} else {}
			MSS_TIMER_DELAY_MS(ControlTaskInst.Timer, CNTL_TSK_FREQ, ControlTaskInst.State);
			buffer_index = 0;
			Manual = FALSE;
		}
    }

	MSS_FINISH();
}

////////////////////////////////////////////////////////////////////
// InitControlTasks - Starts ControlTask periodic timer.          //
// Parameters       - None	      								  //
// Return           - None										  //
////////////////////////////////////////////////////////////////////
static void InitControlTasks(void)
{
    ControlTaskInst.Timer = mss_timer_create(CNTL_TSK_ID);
}

////////////////////////////////////////////////////////////////////
// Port_1     - Port 1 Interrupt to send key through UART.        //
// Parameters - None	      								      //
// Return     - Interrupt										  //
////////////////////////////////////////////////////////////////////
#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
	SendKey = TRUE;
	P1IFG &= ~0x80;
}

////////////////////////////////////////////////////////////////////
// Port_2     - Port 2 Interrupt to manually override lock.       //
// Parameters - None	      								      //
// Return     - Interrupt										  //
////////////////////////////////////////////////////////////////////
#pragma vector=PORT2_VECTOR
__interrupt void Port_2 (void) {
	Manual = TRUE;
	P2IES ^= 0x10;
	P2IFG &= ~0x10;
}