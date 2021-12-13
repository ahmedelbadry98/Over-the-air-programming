/***************************************************************************************/
/**************************** OTA_P ****************************************************/
/***************************************************************************************/
/** This file is developed by Ahmed ElBadry All copyrights are reserved ****************/
/***************************************************************************************/

#include "TYPES.h"


#include "RCC_INTERFACE.h"
#include "GPIO_INTERFACE.h"
#include "USART_INTERFACE.h"
#include "NVIC_INTERFACE.h"
#include "FPEC_interface.h"


#include "ESP8266_INTERFACE.h"

typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

void func(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08002000;

	addr_to_call = *(Function_t*)(0x08002004);
	addr_to_call();
}


void Parser_voidParseRecord(u8* Copy_u8BufData);
extern u8 volatile DataCome[1900] ;
volatile u8  u8RecBuffer[100]   ;
volatile u8  Counter    = 0;
volatile u8  EndFlag     =0;
volatile u8  u8BLWriteReq    = 1;
u64 temp=0;




void send_rec(u8 volatile data[]){

	for(int i=80;i<1900;i++){


	    if(data[i]== ':'){
	            for(int j=(i+1);j<1900;j++){



	                                             if(data[j] == ':' ){
	                                              temp=(j-3);
	                                              Counter=0;
	                                              for(int x=i;x<=temp;x++){u8RecBuffer[Counter]=data[x];Counter++;}
	                                              j=2000;           }


	                                             if(data[j] == '@' ){

	                                              temp=(j-3);
	                                              Counter=0;
	                                              for(int x=i;x<=temp;x++){u8RecBuffer[Counter]=data[x];Counter++;}
	                                              j=2000;i=2000;     }

	                                             if(data[j] == '#' ){

	                                              temp=(j-3);
	                                              Counter=0;
	                                              for(int x=i;x<=temp;x++){u8RecBuffer[Counter]=data[x];Counter++;}
	                                              j=2000;i=2000; EndFlag=1;     }
	                                        }

 Parser_voidParseRecord(u8RecBuffer);   //parse the record

	                      }

	}//for(i)

}//function






void main ( void ){

	/* Create Variable To Receive Data From Server */
	int LOC_u8Data ;
	u8 status=0;

	/* Initialize RCC to work with 8MHz */
	MRCC_VidInit();

	/* Enable USART 1 */
	MRCC_VidEnablePeripheralClock( APB2_BUS , USART1_RCC );

	/* Enable GPIOA */
	MRCC_VidEnablePeripheralClock( APB2_BUS , GPIOA_RCC  );

	/* Configure A9  (Tx1) as Output AFPP @ 2MHz */
	MGPIO_VidSetPinDirection  ( GPIOA , PIN9 , OUTPUT_SPEED_2MHZ_AFPP ) ;

	/* Configure A10 (RX1) as input Floating     */
	MGPIO_VidSetPinDirection  ( GPIOA , PIN10 , INPUT_FLOATING        ) ;

	/* Configure A0  (A0) as Output PP @ 2MHz */
	MGPIO_VidSetPinDirection  ( GPIOA , PIN0 , OUTPUT_SPEED_2MHZ_PP   ) ;
	MGPIO_VidSetPinDirection  ( GPIOA , PIN1 , OUTPUT_SPEED_2MHZ_PP   ) ;

	/* Enable NVIC For USART1 */
	MNVIC_VidSetPriority      ( USART1 , 1 , NO_SUB_PRIORITY , GROUP3 ) ;
	MNVIC_VidEnablePeripheral ( USART1                                ) ;

	/* Initialize UART */
	MUSART_VidInit();

	/* Setting ESP8266 Mode */
	ESP8266_VidInit();

	/* Connecting To WIFI Network */
	ESP8266_VidConnectToWiFi( (u8 *)"WEB52EB2" , (u8 *)"kb552946" );

	/* Connecting To overtheair.freevar.com to check if there is a new hex file to burn*/
	ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );
	status=ESP8266_u8ReceiveHttpReq( (u8 *)"GET http://overtheair.freevar.com/mc_check.txt\r\n" , (u8 *)"55" );

if(status == 1)
{
FPEC_voidEraseAppArea();
}



	while(1){
	ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );

	ESP8266_u8ReceiveHttpReq( (u8 *)"GET http://overtheair.freevar.com/Reciver.php?Rx=ok\r\n" , (u8 *)"60" );

	ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );

	ESP8266_u8ReceiveHttpReq( (u8 *)"GET http://overtheair.freevar.com/record.txt\r\n" , (u8 *)"53" );

	send_rec(DataCome);

	if( EndFlag == 1 )break;

	}


/*    Jump to the application "Entry Point"      */
func();
}


