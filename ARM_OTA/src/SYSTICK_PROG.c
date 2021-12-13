
#include"TYPES.h"
#include"MACROS.h"

#include"SYSTICK_INTERFACE.h"
#include"SYSTICK_REG.h"

void ( * MSTK_CallBack ) ( void );

void SysTick_Handler(void){

	MSTK_CallBack();

}

void MSTK_VidSetCallBack( void (*ptr)(void) ){

	MSTK_CallBack = ptr;
}

void MSTK_VidInit( void ){

	// Enable Systick Interrupt  -  Clock = AHB / 8 - Stop Systic
	MSTK->CTRL = 0x00000002;

}


void MSTK_VidStart( u32 Copy_PreloadValue ){

	// AHB = EXT = 8MHZ & CLOCK OF MSTK = AHB / 8 = 8MHZ / 8 = 1MHZ = 1 Micro For Each Count
	//Load Reload Value
	MSTK->LOAD = Copy_PreloadValue - 1;
	//Clear Val Register
	MSTK->VAL  = 0 ;
	//Enable Systick
	SET_BIT( MSTK->CTRL , 0 );

}

void MSTK_VidINTStatus( u8 Copy_u8Status ){

	 MSTK->CTRL &= ~( 1 << 1 );
	 MSTK->CTRL |=  ( Copy_u8Status << 1 );

}

u8 MSTK_u8ReadFlag( void ){

	return ( GET_BIT( MSTK->CTRL , 16 ) );

}

u32 MSTK_u32GetCounterValue( void )
{
	return MSTK->VAL ;
}

void MSTK_VidStop( void )
{
	/* Load value into Load register */
	MSTK->LOAD = 0;


	/* Clear Value register */
	MSTK->VAL = 0;

}
