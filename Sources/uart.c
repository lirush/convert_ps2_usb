#include "uart.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"


void InitUart1(void)
{
	PORT_InitTypeDef PortInit;
	UART_InitTypeDef UART_InitStructure;
  
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB,ENABLE);

  /* Fill PortInit structure*/
  PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PortInit.PORT_PD = PORT_PD_DRIVER;
  PortInit.PORT_GFEN = PORT_GFEN_OFF;
  PortInit.PORT_FUNC = PORT_FUNC_ALTER;
  PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
  PortInit.PORT_MODE = PORT_MODE_DIGITAL;

  /* Configure PORTB pins 5 (UART1_TX) as output */
  PortInit.PORT_OE = PORT_OE_OUT;
  PortInit.PORT_Pin = PORT_Pin_5;
  PORT_Init(MDR_PORTB, &PortInit);

  /* Configure PORTB pins 6 (UART1_RX) as input */
  PortInit.PORT_OE = PORT_OE_IN;
  PortInit.PORT_Pin = PORT_Pin_6;
  PORT_Init(MDR_PORTB, &PortInit);	

  /* Enables the CPU_CLK clock on UART1 */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);

  /* Set the HCLK division factor = 1 for UART1*/
  UART_BRGInit(MDR_UART1, UART_HCLKdiv2);

  /* Initialize UART_InitStructure */
  UART_InitStructure.UART_BaudRate                = 115200;
  UART_InitStructure.UART_WordLength              = UART_WordLength8b;
  UART_InitStructure.UART_StopBits                = UART_StopBits1;
  UART_InitStructure.UART_Parity                  = UART_Parity_No;
  UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF;
  UART_InitStructure.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

  /* Configure UART1 parameters*/
  UART_Init (MDR_UART1,&UART_InitStructure);

  /* Enables UART1 peripheral */
  UART_Cmd(MDR_UART1,ENABLE);	
}

void UART1SendData(char *data, unsigned short int len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		/* пока буфер не пуст ждем */
    while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE)!= SET)
    {
    }
    UART_SendData (MDR_UART1,data[i]);	
	}
}
