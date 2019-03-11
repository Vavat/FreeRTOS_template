/*
 * UARTCommsController.cpp
 *
 *  Created on: 15 Apr 2017
 *      Author: Vavat
 */

#include "UARTCommsController.h"
#include "stm32l476xx.h"
#include "cmsis_os.h"
#include <locale>
#include <string.h>
#include "CThreadBase.h"
#include "stm32l4xx_hal.h"

#define RESPONSE_MARKER		'<'
#define COMMAND_MARKER		'>'
#define isCommandMarker(x)	(x == COMMAND_MARKER)
#define isEndOfCommand(x) 	(x == 10 || x == 13)		// LF (10) or CR (13)
#define isCheckSum(x)		(x == ';')
#define isService(x)		(x == '(' || x == ')')
#define isValid(x)			(isEndOfCommand(x) || isprint(x))

/* cunning way to convert character into a digit. This is possible due to ASCII table digit mapping. */
#define toDigit(x)			(x - 48)

UARTCommsController* UARTCommsController::s_pUARTController1 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController2 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController3 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController4 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController5 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController6 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController7 = NULL;
UARTCommsController* UARTCommsController::s_pUARTController8 = NULL;

/* TxBuffer is large to accommodate possible large transmissions
 * RxBuffer is smaller since it only needs to handle incoming commands
 * @115200 baud it'll take 1/115200 * (1 + 8 + 1) * 64 = 5.5 ms
 * to overflow. Since this controller runs faster than that, it should
 * never happen. */
UARTCommsController::UARTCommsController() :
m_pHUART(NULL),
m_TxBuffer(200),
m_RxBuffer(64),
m_ControllerCount(0)
{
	m_pControllers[m_ControllerCount] = NULL;
	m_IncomingRxChar = '\0';
	m_NewCommand[49] = '\0';
	m_RxCRCString[2] = '\0';
	Reset();
}

void UARTCommsController::Initialise(UART_HandleTypeDef * pHUART, bool debugChannel){
	m_pHUART = pHUART;
	m_bDebugChannel = debugChannel;

	#ifdef USART1
		if(pHUART->Instance == USART1){
			s_pUARTController1 = this;
		}
	#endif
	#ifdef USART2
		if(pHUART->Instance == USART2){
			s_pUARTController2 = this;
		}
	#endif
	#ifdef USART3
		if(pHUART->Instance == USART3){
			s_pUARTController3 = this;
		}
	#endif
	#ifdef USART4
		if(pHUART->Instance == USART4){
			s_pUARTController4 = this;
		}
	#endif
	#ifdef USART5
		if(pHUART->Instance == USART5){
			s_pUARTController5 = this;
		}
	#endif
	#ifdef USART6
		if(pHUART->Instance == USART6){
			s_pUARTController6 = this;
		}
	#endif
	#ifdef USART7
		if(pHUART->Instance == USART7){
			s_pUARTController7 = this;
		}
	#endif
	#ifdef USART8
		if(pHUART->Instance == USART8){
			s_pUARTController8 = this;
		}
	#endif
}

UARTCommsController::~UARTCommsController() {
}

void UARTCommsController::Run(){
	if(m_bDebugChannel){
		if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET){
			SendReply("Independent watchdog reset.");
		}
		if(__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)){
			SendReply("Pin reset.");
		}
		if(__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) != RESET){
			SendReply("Brown-out reset.");
		}
		if(__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)){
			SendReply("Software reset.");
		}
		if(__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET){
			SendReply("Window watchdog reset.");
		}
		if(__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) != RESET){
			SendReply("Low power reset.");
		}
		SendReply("Ready!");
	}
	/* reset flag */
	__HAL_RCC_CLEAR_RESET_FLAGS();

	HAL_UART_Receive_IT(m_pHUART, &m_IncomingRxChar, 1);
	for(;;){
		/* Check for incoming data */
		char incoming;
		bool bMoreData = false;
		bool bCommandReady = false;
		if(!m_RxBuffer.IsEmpty()){
			do{
			    taskENTER_CRITICAL();
				bMoreData = m_RxBuffer.Get(&incoming);
				taskEXIT_CRITICAL();
				bCommandReady = ParseCommand(&incoming);
			}while(bMoreData & !bCommandReady);
		}
		/* If new command is ready, send to all controllers */
		if(bCommandReady){
			bool bCommandRecognised = false;
			char reply[200];
			unsigned int controller = 0;
			do{
				*reply = '\0';
				bCommandRecognised = (m_pControllers[controller])->NewCommand((char*)m_NewCommand, reply);
				if(bCommandRecognised){
					SendReply(reply);
				}
			}while(m_pControllers[++controller] != NULL && !bCommandRecognised);
			/* if command not recognised send generic reply. */
			if(!bCommandRecognised){
				SendReply("Command not recognised.");
			}
		}

		/* Check for outgoing data */
		TxChar();
		osDelay(10);
	}
}

//TODO: refactor method to stop using local char[] variable. Too much memory use.
int UARTCommsController::SendReply(char * pReply){
	unsigned int CheckSum = 0;
	unsigned int CharCounter = 0;
	taskENTER_CRITICAL();
	/* add reply marker */
	m_TxBuffer.Set('<');
	CharCounter++;
	/* add actual reply */
	while(*pReply != '\0'){
		m_TxBuffer.Set(*pReply);
		CheckSum += *pReply;
		pReply++;
		CharCounter++;
	}
	/* add checksum */
	m_TxBuffer.Set(';');
	CheckSum &= 0xF;
	if(CheckSum < 10){
		m_TxBuffer.Set(CheckSum+48); //offset 0 to 48, which is ascii value for "0".
	}
	else{
		m_TxBuffer.Set(CheckSum+55); //offset 10 to 65, which is ascii value for "A".
	}
	m_TxBuffer.Set('\r');
	taskEXIT_CRITICAL();
	return CharCounter;
}

void UARTCommsController::RxChar(){
	m_RxBuffer.Set((const char)m_IncomingRxChar);
	HAL_UART_Receive_IT(m_pHUART, &m_IncomingRxChar, 1);
	return;
}

void UARTCommsController::TxChar(){
	if(!m_TxBuffer.IsEmpty()){
		//TODO Need to make this more efficient and transmit the entire buffer contents.
		if(m_pHUART->gState == HAL_UART_STATE_READY){
			m_TxBuffer.Get((char*)&m_OutgoingTxChar);
			HAL_UART_Transmit_IT(m_pHUART, &m_OutgoingTxChar, 1);
		}
	}else{
		CLEAR_BIT(m_pHUART->Instance->CR1, USART_CR1_TXEIE);
		CLEAR_BIT(m_pHUART->Instance->CR1, USART_CR1_TCIE);
	}
	return;
}

void UARTCommsController::Reset(){
	m_ParserState = EParserState_Idle;
	m_pNewCommandChar = &m_NewCommand[0];
	return;
}

uint8_t UARTCommsController::CalculateChecksum(){
	uint8_t * pRunner = m_NewCommand;
	uint8_t checkSum;
	while(*pRunner != '\0'){
		checkSum += *pRunner++;
	}
	return checkSum;
}

/* command can be in following formats:
 * >command 		- simplest command. No nodeID, no CRC. Only for debug purposes.
 * >command;XX 	    - CRC checked command. This is what all clients must be sending.
 * 					CRC covers everything between starting marker and semicolon.
 */
bool UARTCommsController::ParseCommand(char * pData){
	bool bCommandComplete = false;
	/* sanitise input. */
	if(!isValid(*pData)){
		Reset();
		return bCommandComplete;
	}

	/* any time command marker is encountered, start command parsing from scratch. */
	if(isCommandMarker(*pData)){
		Reset();
	}

	switch(m_ParserState){
	case EParserState_Command:
		if(isEndOfCommand(*pData)){							/* CR or LF is received. end of command*/
			*m_pNewCommandChar = '\0';
			m_ParserState = EParserState_Idle;
			bCommandComplete = true;
		}else if(isCheckSum(*pData)){ 						/* ';' received. end of command string, start of checksum */
			*m_pNewCommandChar = '\0';
			m_ParserState = EParserState_Checksum1;
		}else{												/* everything else goes into command accumulator. */
			*m_pNewCommandChar = tolower(*pData);
			m_pNewCommandChar++;
		}
		break;
	case EParserState_Checksum1:
		if(isxdigit(*pData)){								/* hex digit expected. */
			m_RxCRCString[0] = *pData;
			m_ParserState = EParserState_Checksum2;
		}
		else {
			Reset();
		}
		break;
	case EParserState_Checksum2:
		if(isxdigit(*pData)){								/* hex digit expected. if checksum fits, command is deemed accepted.*/
			m_RxCRCString[1] = *pData;
			unsigned int csScan;
			csScan = sscanf(m_RxCRCString, "%x", (unsigned int *)&m_RxCRC);
			if(csScan == 1 && m_RxCRC == CalculateChecksum()){
				m_ParserState = EParserState_Idle;
				bCommandComplete = true;
			}
			else{
				Reset();
				char reply[50];
				sprintf(reply, "Incorrect checksum: %02X. Expected: %02X", m_RxCRC, CalculateChecksum());
				SendReply(reply);
			}
		}
		else{
			Reset();
		}
		break;
	case EParserState_Idle:
		if(isCommandMarker(*pData)){
			Reset();
			m_ParserState = EParserState_Command;
		}
		break;
	default:
		Reset();
	}
	return bCommandComplete;
}

void UARTCommsController::RegisterController(CThreadBase * pController){
	if(pController){
		/* store new pointer and set next one to null. */
		m_pControllers[m_ControllerCount++] = pController;
		m_pControllers[m_ControllerCount] = NULL;
		//TODO: keep track of number of controllers.
	}
}

/* UART IRQ handler callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
	if(huart->Instance == USART1){
		UARTCommsController::s_pUARTController1->RxChar();
	}
	if(huart->Instance == USART2){
		UARTCommsController::s_pUARTController2->RxChar();
	}
	if(huart->Instance == USART3){
		UARTCommsController::s_pUARTController3->RxChar();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart){
	if(huart->Instance == USART1){
		UARTCommsController::s_pUARTController1->TxChar();
	}
	if(huart->Instance == USART2){
		UARTCommsController::s_pUARTController2->TxChar();
	}
	if(huart->Instance == USART3){
		UARTCommsController::s_pUARTController3->TxChar();
	}
}
