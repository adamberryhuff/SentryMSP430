////////////////////////////////////////////////////////////////////
// HEADER FILES - Master & UART module headers.                   //
////////////////////////////////////////////////////////////////////
#include "includes.h"
#include "UART.h"

INT8U RxChar = 0x00;

////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES - UART Module function prototypes.         //
////////////////////////////////////////////////////////////////////
void UARTInit(void);
void UARTPutChar(INT8U ch);
void UARTPutStr(const INT8U *str);
INT8U UARTGetChar(void);

////////////////////////////////////////////////////////////////////
// UARTInit   - Initializes a full duplex UART protocol           //
// Parameters - None                                              //
// Return     - None                                              //
////////////////////////////////////////////////////////////////////
void UARTInit(void)
{
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 8;                              // 1MHz 115200
    UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

////////////////////////////////////////////////////////////////////
// UARTPutChar - Sends one character over UART                    //
// Parameters  - INT8U ch - Character to be sent                  //
// Return      - None                                             //
////////////////////////////////////////////////////////////////////
void UARTPutChar(INT8U ch)
{
    while (!(TX_BUFF_READY));
    UCA0TXBUF = ch;
}

////////////////////////////////////////////////////////////////////
// UARTPutStr  - Sends a string over UART                         //
// Parameters  - INT8U *str - Pointer to the address of the       //
//                              first character of the string     //
// Return      - None                                             //
////////////////////////////////////////////////////////////////////
void UARTPutStr(const INT8U *str)
{
    INT8U ch_index = 0;
    INT8U ch = str[ch_index];

    while (ch != 0x00) {
        UARTPutChar(ch);
        ch_index++;
        ch = str[ch_index];
    }
}

////////////////////////////////////////////////////////////////////
// UARTGetChar - Returns the current character in the UART Buffer //
// Parameters  - None                                             //
// Return      - INT8U: Buffer Char                               //
////////////////////////////////////////////////////////////////////
INT8U UARTGetChar(void)
{
    return RxChar;
}

////////////////////////////////////////////////////////////////////
// USCI0RX_ISR - UART ISR for receiving character                 //
// Parameters  - None                                             //
// Return      - __interrupt                                      //
////////////////////////////////////////////////////////////////////
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    RxChar = UCA0RXBUF;
}

