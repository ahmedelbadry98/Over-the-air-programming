
#include"TYPES.h"
#include"MACROS.h"

#include"NVIC_INTERFACE.h"
#include"NVIC_REG.h"


void MNVIC_VidEnablePeripheral ( u8 Copy_u8INTID ){

	MNVIC->ISER[ Copy_u8INTID / 32 ] = 1 << ( Copy_u8INTID % 32 ) ;

}

void MNVIC_VidDisablePeripheral ( u8 Copy_u8INTID ){

	MNVIC->ICER[ Copy_u8INTID / 32 ] = 1 << ( Copy_u8INTID % 32 ) ;

}


void MNVIC_VidSetPending ( u8 Copy_u8INTID ){

	MNVIC->ISPR[ Copy_u8INTID / 32 ] = 1 << ( Copy_u8INTID % 32 ) ;


}

void MNVIC_VidClearPending ( u8 Copy_u8INTID ){

	MNVIC->ICPR[ Copy_u8INTID / 32 ] = 1 << ( Copy_u8INTID % 32 ) ;

}


u8   MNVIC_u8GetActive( u8 Copy_u8INTID ){

	u8 LOC_u8Active = GET_BIT( (MNVIC->IAPR[ Copy_u8INTID/32 ]) , ( Copy_u8INTID % 32 ) );
	return LOC_u8Active ;

}


void MNVIC_VidSetPriority( s8 Copy_u8INTID , u8 Copy_u8GroupPriority , u8 Copy_u8SubPriority , u32 Copy_u8Group ){

	u8 Priority =  Copy_u8SubPriority | Copy_u8GroupPriority << ( (Copy_u8Group - 0x05FA0300) / 256 )  ;
	//Core Peripheral
	if( Copy_u8INTID < 0  ){

		if( Copy_u8INTID == MEMORY_MANAGE || Copy_u8INTID == BUS_FAULT || Copy_u8INTID == USAGE_FAULT ){

			Copy_u8INTID += 3;
			SCB -> SHPR1 = ( Priority ) << ( (8 * Copy_u8INTID) + 4 );

		}
		else if ( Copy_u8INTID == SV_CALL ){

			Copy_u8INTID += 7;
			SCB -> SHPR2 = ( Priority ) << ( (8 * Copy_u8INTID) + 4 );

		}
		else if( Copy_u8INTID == PEND_SV || Copy_u8INTID == SYSTICK ){

			Copy_u8INTID += 8;

			SCB -> SHPR3 = ( Priority ) << ( (8 * Copy_u8INTID) + 4 );

		}

	}
    //External Peripheral
	else if( Copy_u8INTID >= 0 ){

		MNVIC->IPR[ Copy_u8INTID ] = Priority << 4 ;

	}

	SCB->AIRCR = Copy_u8Group ;

}

