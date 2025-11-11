/*
 * uart.c
 *
 *  Created on: Nov 10, 2025
 *      Author: HANH
 */

#include "uart.h"
#define ACK_TIMER		1
#define ACK_DUR			3000
uint8_t in_byte = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t str[32]; 					// guarantee string capacity
uint8_t str_len = 0;

uint32_t ADC_value = 0;

command command_data = NONE;
uint8_t command_flag = 0;

communication_state current_com_state = INIT;
parser_state current_parse_state = CMD_START;


uint8_t buffer_is_flag()		//buffer flag wrapper function
{
	return buffer_flag;
}

void reset_flag()				//buffer flag wrapper function
{
	buffer_flag = 0;
}

void get_rx_msg(uint8_t rx_byte)
{
	in_byte = rx_byte;
	buffer[index_buffer++] = rx_byte;
	if (index_buffer == MAX_BUFFER_SIZE) index_buffer = 0;
	buffer_flag = 1;
}

void command_parser_fsm()
{
	switch (current_parse_state) {
		case CMD_START:
			if(1){		//Default init transition
				current_parse_state = CMD_WAIT;
			}
			break;

		case CMD_WAIT:
			if (in_byte == '!') 	// Start point of the command
			{
				index_buffer = 0;
				current_parse_state = CMD_GET;
			}
			break;
		case CMD_GET:
			if (in_byte == '#') 	// End point of the command
			{
				if (buffer[0] == 'R' && buffer[1] == 'S'  && buffer[2] == 'T' && buffer[3] == '#')
				{
					command_data = RST;
					command_flag = 1;
				}
				else if (buffer[0] == 'O' && buffer[1] == 'K'  && buffer[2] == '#')
				{
					command_data = OK;
					command_flag = 1;
				}
				current_parse_state = CMD_WAIT;
			}
			break;
		default:
			break;
	}
}

void uart_communication_fsm()
{
	switch (current_com_state) {
		case INIT:
			if (1) 		// Default init transition
			{
				current_com_state = WAITING;
			}
			break;

		case WAITING:
			if (command_flag == 1 && command_data == RST) 	//There is a complete command and it is RST
			{
				current_com_state = SENDING;
				command_flag = 0;				//Done command;
			}
			break;

		case SENDING:
			ADC_value = HAL_ADC_GetValue(&hadc1);		//Read ADC
			str_len = sprintf((char *)str, "!ADC=%lu#\r\n", ADC_value);	//Transform to string
			HAL_UART_Transmit(&huart2, str, str_len , HAL_MAX_DELAY);	//Transmit to virtual terminal

			if (1) 		// Always change to WAIT_ACK right after send
			{
				timerSet(ACK_TIMER, ACK_DUR);
				current_com_state = WAIT_ACK;
			}
			break;

		case WAIT_ACK:
			if (command_flag == 1 && command_data == OK) //There is a complete command and it is RST
			{
				current_com_state = WAITING;
				command_flag = 0;				//Done command;
			}
			else if (timerFlag(ACK_TIMER)) // Change to SENDING after 3s (resend)
			{
				current_com_state = SENDING;
			}
			break;

		default:
			break;
	}
}
