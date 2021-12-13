
#ifndef SYSTICK_REG_H_
#define SYSTICK_REG_H_

typedef struct{

	volatile u32 CTRL ;
	volatile u32 LOAD ;
	volatile u32 VAL  ;
	volatile u32 CALIB;

}SYSTICK;


#define MSTK ((volatile SYSTICK*)0xE000E010)

#endif /* SYSTICK_REG_H_ */
