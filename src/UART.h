/********************************************************************
* UART.h - Defines forward-facing functions for UART duplex 
*          communication.
*
* 05/15/2013 Adam Berry
*********************************************************************/
 
// Forward Facing UART Functions
extern void UARTInit(void);
extern void UARTPutChar(INT8U ch);
extern void UARTPutStr(const INT8U *str);
extern INT8U UARTGetChar(void);