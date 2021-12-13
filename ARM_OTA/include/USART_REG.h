
#ifndef USART_REG_H_
#define USART_REG_H_

typedef struct{

	volatile u32 SR;
	volatile u32 DR;
	volatile u32 BRR;
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 CR3;
	volatile u32 GTPR;

}USART_Type;


#define MUSART1 ( ( volatile USART_Type* ) 0x40013800 )

#define MUSART2 ( ( volatile USART_Type* ) 0x40004400 )

#define MUSART3 ( ( volatile USART_Type* ) 0x40004800 )

#define MUART4 (  ( volatile USART_Type* ) 0x40004C00 )

#define MUART5 (  ( volatile USART_Type* ) 0x40005000 )

#endif /* USART_REG_H_ */
