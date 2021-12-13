

#ifndef RCC_REG_H_
#define RCC_REG_H_

typedef struct{
	volatile u32 CR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 APB2RSTR;
	volatile u32 APB1RSTR;
	volatile u32 AHBENR;
	volatile u32 APB2ENR;
	volatile u32 APB1ENR;
	volatile u32 BDCR;
	volatile u32 CSR;
}RCC;

// M is Refer To MCal

#define MRCC ((volatile RCC *)0x40021000)

#endif /* RCC_REG_H_ */
