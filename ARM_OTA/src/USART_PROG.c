
#include "TYPES.h"
#include "MACROS.h"

#include "USART_INTERFACE.h"
#include "USART_CONFIG.h"
#include "USART_REG.h"

#define THRESHOLD_VALUE  9000000UL

void (*MUSART1_CallBack)(void);

void MUSART_VidInit ( void ){

	u32 LOC_u64Mantissa = ( F_CPU ) / ( 16 * USART1_BAUD_RATE ) ;

	u32 LOC_u64Fraction = ( ( ( F_CPU * 100 ) / ( 16 * USART1_BAUD_RATE )  ) % 100 ) * 16 ;

	if( LOC_u64Fraction > 1500 ){ LOC_u64Mantissa += 1 ; LOC_u64Fraction  = 0 ; }

	#if USART1_STATUS == USART1_ENABLE

		MUSART1 -> SR = 0;

		#if   USART1_WORD_LENGTH == _8BIT_WORD_LENGTH

					CLR_BIT( MUSART1 -> CR1 , 12 );

		#elif USART1_WORD_LENGTH == _9BIT_WORD_LENGTH

					SET_BIT( MUSART1 -> CR1 , 12 );

		#endif

		#if   USART1_PARITY == PARITY_DISABLE

					CLR_BIT( MUSART1 -> CR1 , 10 );

		#elif USART1_PARITY == EVEN_PARITY

					SET_BIT( MUSART1 -> CR1 , 10 );
					CLR_BIT( MUSART1 -> CR1 , 11 );

		#elif USART1_PARITY == ODD_PARITY

					SET_BIT( MUSART1 -> CR1 , 10 );
					SET_BIT( MUSART1 -> CR1 , 11 );

		#endif

		#if  USART1_INTERRUPT == INT_DISABLE

					CLR_BIT( MUSART1 -> CR1 , 5 );
					CLR_BIT( MUSART1 -> CR1 , 6 );
					CLR_BIT( MUSART1 -> CR1 , 7 );

		#elif USART1_INTERRUPT == TXE_INT_ENABLE

					SET_BIT( MUSART1 -> CR1 , 7 );

		#elif USART1_INTERRUPT == TCE_INT_ENABLE

					SET_BIT( MUSART1 -> CR1 , 6 );

		#elif USART1_INTERRUPT == RXNE_INT_ENABLE

					SET_BIT( MUSART1 -> CR1 , 5 );

		#endif

		#if   USART1_TRANSMITTER == TRANSMITTER_ENABLE

					SET_BIT( MUSART1 -> CR1 , 3 );

		#elif USART1_TRANSMITTER == TRANSMITTER_DISABLE

					CLR_BIT( MUSART1 -> CR1 , 3 );

		#endif

		#if   USART1_RECEIVER == RECEIVER_ENABLE

					SET_BIT( MUSART1 -> CR1 , 2 );

		#elif USART1_RECEIVER == RECEIVER_DISBLE

					CLR_BIT( MUSART1 -> CR1 , 2 );

		#endif

		#if   USART1_STOP_BITS == ONE_STOP_BIT

					CLR_BIT( MUSART1 -> CR2 , 12 ); CLR_BIT( MUSART1 -> CR2 , 13 );

		#elif USART1_STOP_BITS == HALF_STOP_BIT

					SET_BIT( MUSART1 -> CR2 , 12 ); CLR_BIT( MUSART1 -> CR2 , 13 );

		#elif USART1_STOP_BITS == TWO_STOP_BIT

					CLR_BIT( MUSART1 -> CR2 , 12 ); SET_BIT( MUSART1 -> CR2 , 13 );

		#elif USART1_STOP_BITS == ONE_AND_HALF_STOP_BIT

					SET_BIT( MUSART1 -> CR2 , 12 ); SET_BIT( MUSART1 -> CR2 , 13 );


		#endif

		MUSART1 -> BRR = ( LOC_u64Mantissa << 4  ) | ( LOC_u64Fraction / 100 ) ;
		SET_BIT( MUSART1 -> CR1 , 13 );

	#elif USART1_STATUS == USART1_DISABLE
		CLR_BIT( MUSART1 -> CR1 , 0 );

	#endif

}


void MUSART1_VidSendCharSync ( u8 Copy_u8Char ){

	MUSART1 -> DR = Copy_u8Char ;

	while( GET_BIT( MUSART1 -> SR , 6 ) == 0 );

	CLR_BIT( MUSART1 -> SR , 6 );

}

void MUSART1_VidSendStringSynch ( u8 * Copy_ptrString ){

	u8 LOC_u8Iterator = 0 ;

	while ( Copy_ptrString[ LOC_u8Iterator ] != '\0' ){

		MUSART1_VidSendCharSync( Copy_ptrString[ LOC_u8Iterator ] );
		LOC_u8Iterator++ ;

	}

}

u8 MUSART1_u8RecCharSynch ( void ){

	u8  LOC_u8Data  = 0 ;
	u32 LOC_TimeOut = 0 ;

	CLR_BIT ( MUSART1 -> SR, 5);

	while( ( GET_BIT ( MUSART1 -> SR, 5) == 0 ) && ( LOC_TimeOut < THRESHOLD_VALUE ) )
	{
		LOC_TimeOut++;
	}

	if( LOC_TimeOut == THRESHOLD_VALUE )
	{
		LOC_u8Data = 255;
	}
	else
	{

		LOC_u8Data = MUSART1 -> DR;
	}

	return LOC_u8Data;

}

u8 String[20];
u8 * MUSART1_PtrReadStringSynch ( void ){

	u8 LOC_Iterator = 0 , LOC_u8DataCome ;

	while( ( LOC_u8DataCome = MUSART1_u8RecCharSynch() ) != 13 ){

		String[ LOC_Iterator ] = LOC_u8DataCome ;
		LOC_Iterator++;
	}

	String[LOC_Iterator] = '\0';

	return ( String );

}

void MUSART1_VidClearFlags( void ){

	/* Clear all flags */
	MUSART1 -> SR = 0;

}

u8 MUSART1_u8ReadDataRegister ( void ){

	return ( MUSART1 -> DR );

}


void MUSART1_VidSetCallBack( void (*ptr) (void) ){

	MUSART1_CallBack = ptr ;

}

void USART1_IRQHandler(void){

	MUSART1_CallBack();

}
