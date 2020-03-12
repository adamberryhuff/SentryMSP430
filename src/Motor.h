// Forward Facing Motor Functions
extern void MotorInit(void);
extern void MotorIn(void);
extern void MotorOut(void);
extern INT8U MotorEnable2s(void);

// Lock States
typedef enum {
	LOCKED,
	UNLOCKED,
	INIT
} LOCK_STATE;

// TACCR0: VLOCLK = 12 kHz. TAACR0 is 8 Bits.
//         Timer A Uses 12 kHz / 8 = 1.5 kHz
//         TACCR0 = Second * 1.5 kHz - 1
#define TACCR0_CNT       3500
#define TACCR0_INIT      8520

// Macros
#define MOTOR_UNLOCK     P2OUT &= ~0x01; P2OUT |= 0x02
#define MOTOR_LOCK       P2OUT |= 0x01; P2OUT &= ~0x02
#define MOTOR_ENABLE     P2OUT |= BIT5
#define MOTOR_DISABLE    P2OUT &= ~BIT5
