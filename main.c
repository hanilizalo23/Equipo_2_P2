/**
	\file
	\brief P2, Electronic message board
	\author Nelida Paulina Hernández Moya y Mauricio Peralta Osorio
	\date	14/04/2022
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "Menus.h"
#include "fsl_uart.h"

int main(void) {
	ConfigureAll();
	flow_flags_t flag_flow;

    while(1) {
    	//Constantly read one character in both terminals
    	flag_flow = ReadTerminals();

    	//Do something according to the stage of the program
    	if(flag_flow.t1_flag)
    	{
    		ChooseStage(PC);
    	}
    	if(flag_flow.t2_flag)
    	{
    		ChooseStage(HC05);
    	}
    }
    return 0 ;
}
