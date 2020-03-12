/********************************************************************
* Includes.h - Holds project type defines and macros.
*
* 05/15/2013 Adam Berry
*********************************************************************/
 
// MSP430 Library
#include 
// Data types
typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned short INT16U;
typedef signed   short INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;
// MSS Tasks
extern void ControlTask(void *param);
// Task ID's
#define CNTL_TSK_ID                     (0)
// Default Task Frequencies & Macros
#define CNTL_TSK_FREQ                 (100)
#define CH_MOTOR_STATE               (1500)
#define SECURITY_KEY           ("1X5u!j8*")
#define KEY_LEN                         (8)
#define RX_BUFFER_LEN                   (10)
#define STATE_CHECK_CH                ('~')
#define TRUE              1
#define FALSE            0
#define TX_BUFF_READY     IFG2&UCA0TXIFG
#define WATCHDOG_STOP     WDTCTL = WDTPW + WDTHOLD
#define LOW_POWER_ISR     __bis_SR_register(GIE)
#define FOREVER()       while(1)