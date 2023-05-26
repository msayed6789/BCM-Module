
#ifndef UTILS_H_
#define UTILS_H_


#define F_CPU  8000000
#include <util/delay.h>



#define Read_Bit(reg,bit)         (reg=reg>>bit & 1)
#define Set_Bit(reg,bit)          (reg|=1<<bit)
#define CLR_Bit(reg,bit)          (reg&=~(1<<bit))
#define TOG_BIT(reg,bit)          (reg=reg^(1<<bit))
#define WRITE_BIT(reg,bit,val)    (reg=(reg&(~(1<<bit))) | (val<<bit))





#endif /* UTILS_H_ */