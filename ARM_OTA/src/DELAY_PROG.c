

#include"TYPES.h"

#include"SYSTICK_INTERFACE.h"

#include"DELAY_INTERFACE.h"


void _delay_ms( u32 Copy_u32Time ){

	MSTK_VidInit();
	/* Disable INT */
	MSTK_VidINTStatus( MSTK_INT_DIS );
	MSTK_VidStart( Copy_u32Time * 1000 );
	/* Wiat Flag Polling */
	while( MSTK_u8ReadFlag() == 0 );

}


/* Suppose That The Clock System (AHB) = 8MHZ EXT & The Systick Clock Is = AHB/8 */
void _delay_us( u32 Copy_u32Time ){

	MSTK_VidInit();
	/* Disable INT */
	MSTK_VidINTStatus( MSTK_INT_DIS );
	MSTK_VidStart( Copy_u32Time );
	/* Wiat Flag Polling */
	while( MSTK_u8ReadFlag() == 0 );

}
