#include <stdio.h>

#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/pushbuttons.h"

int main() {
	int_setup(2, (int []){73, 199});

	// enable the interrupts for pushbuttons
	enable_PB_INT_ASM(PB0 | PB2 | PB3);
	
	// initialize time parameters
	HPS_TIM_config_t hps_tim;

	hps_tim.tim = TIM0;
	hps_tim.timeout = 1000;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;

	HPS_TIM_config_ASM(&hps_tim);

	HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);

	int ms = 0, sec = 0, min = 0, timer_on = 1;
	while(1) {
		if(hps_tim0_int_flag) {
			hps_tim0_int_flag = 0;
			if (HPS_TIM_read_INT_ASM(TIM0) && timer_on) {
				HPS_TIM_clear_INT_ASM(TIM0);
				// count in increments of 10 ms
				ms += 10;
				// 1000ms = 1s so we increment seconds and reset ms
				if (ms >= 1000){
					ms -= 1000;
					sec++;
				}
				// 1s = 1min so we increment minutes and reset sec
				if (sec >= 60){
					sec -= 60;
					min++;
				}
				// reset minutes if it goes above 60
				if (min >= 60){
					min -= 60;
				}

				// find the appropriate values for the hex displays
				HEX_write_ASM(HEX0, (ms % 100) / 10);
				HEX_write_ASM(HEX1, ms / 100);
				HEX_write_ASM(HEX2, sec % 10);
				HEX_write_ASM(HEX3, sec / 10);
				HEX_write_ASM(HEX4, min % 10);
				HEX_write_ASM(HEX5, min / 10);
			}
		}
		if(fpga_pb_int_flag != 4) {

			// if the interrupt is pb 0 then start timer
			if(fpga_pb_int_flag == 0){
				timer_on = 1;
			}
			// if the interrupt is pb 1 then stop timer
			if(fpga_pb_int_flag == 1){
				timer_on = 0;
			}
			// if the interrupt is pb 1 then stop timer
			if(fpga_pb_int_flag == 2){
				// reset all values and set timer_on to false
				timer_on = 0;
				ms = 0;
				sec = 0;
				min = 0;

				// reset all the displays to show 0
				HEX_write_ASM(HEX0 | HEX1 | HEX2 | HEX3 | HEX4 | HEX5, 0);
			}
			fpga_pb_int_flag = 4;

		}
	}
	return 0;
}
