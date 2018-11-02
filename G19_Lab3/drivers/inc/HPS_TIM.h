#ifndef __HPS_TIM
#define __HPS_TIM

	typedef enum {
		TIM0 = 0x00000001,
		TIM1 = 0x00000002,
		TIM2 = 0x00000004,
		TIM3 = 0x00000008
	}	HPS_TIM_t;

	typedef struct {
		HPS_TIM_t tim;
		int timeout; // in usec
		int LD_en;
		int INT_en;
		int enable;
	}	HPS_TIM_config_t;
	
	extern void HPS_TIM_config_ASM(HPS_TIM_config_t *param); //takes a struct pointer as an argument
						//extra arguments go onto stack
						//individual elements can be easily accessed at offset from base address
	extern int HPS_TIM_read_INT_ASM(HPS_TIM_t tim); //multiply timer instances passed in argument
						//return: integer of 4-bit one hot encoded # 
						// 0 = S-bit (interrupt status register) of TIM0
	extern void HPS_TIM_clear_INT_ASM(HPS_TIM_t tim);

#endif
