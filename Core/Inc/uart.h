/*
 * uart.h
 *
 *  Created on: Nov 10, 2025
 *      Author: HANH
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

typedef enum
{
	NONE,
	RST,
	OK
} command;

typedef enum
{
	INIT,
	WAITING,
	SENDING,
	WAIT_ACK
} communication_state;

typedef enum
{
	CMD_START,
	CMD_WAIT,
	CMD_GET
} parser_state;

#define MAX_BUFFER_SIZE 30

uint8_t buffer_is_flag();
void reset_flag();
void get_rx_msg(uint8_t in_byte);
void command_parser_fsm();
void uart_communication_fsm();

#endif /* INC_UART_H_ */
