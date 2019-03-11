/*
 * UARTCommsController.h
 *
 *  Created on: 15 Apr 2017
 *      Author: Vavat
 */

#ifndef UARTCOMMSCONTROLLER_H_
#define UARTCOMMSCONTROLLER_H_

#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "..\Libraries\FIFOBuffer.h"
#include "cmsis_os.h"
#include "CThreadBase.h"

#define MAX_COMMAND_SIZE	200

class UARTCommsController {
public:
	UARTCommsController();
	virtual ~UARTCommsController();

	/* @brief:  initialise instance with correct UART and node identifier
	 * @arg:	pointer to HAL UART instance.
	 * @arg:	boolean marker if this channel can be used for debugging.
	 */
	void Initialise(UART_HandleTypeDef * pHUART, bool isDebugChannel);

	/* @brief:	setup half-duplex control pin.
	 * @arg:	pTxEnablePort GPIO port pointer.
	 * @arg:	pTxEnablePin GPIO pin map pointer.
	 * @ret:	return true if setup is successful.
	 * @Note:	pTxEnablePin is a bitmap and must have only one bit set.
	 */
	bool SetupHalfDuplexControl(GPIO_TypeDef * pTxEnablePort, uint16_t TxEnablePin);

	/* @brief:	main thread runner.
	 * @note: 	this method never returns.
	 */
	void Run();

	/* @brief:	receive new character into buffer.
	 * 			when interrupt fires this function is called to store received
	 * 			character in the buffer for processing at allocated time slot.
	 */
	void RxChar();

	/* @brief:	transmit character from buffer.
	 * 			when interrupt fires this function is called to store received
	 * 			character in the buffer for processing at allocated time slot.
	 */
	void TxChar();

	/* @brief:	parse characters and extract correctly formatted commands.
	 * @arg:	pointer to next character that arrived through comport.
	 * @ret:	returns true when command assembly has been completed.
	 */
	bool ParseCommand(char * pData);

	/* @brief:	send reply via TxBuffer.
	 * @arg:	pointer to string to be sent back
	 * @ret:	number of characters that were sent.
	 * 			return -1 if failed to send all.
	 */
	int SendReply(char * pReply);

	/* @brief:	Register mailbox for sending commands.
	 * 			This is a way of letting Comms controller know where to send
	 * 			commands it receives.
	 * @arg:	Pointer to osMailQId of the inbox that can accept commands.
	 */
	void RegisterController(CThreadBase * pController);

	/* static pointers to this class instances and corresponding UART hardware */
	static UARTCommsController * 	s_pUARTController1;
	static UARTCommsController * 	s_pUARTController2;
	static UARTCommsController * 	s_pUARTController3;
	static UARTCommsController * 	s_pUARTController4;
	static UARTCommsController * 	s_pUARTController5;
	static UARTCommsController * 	s_pUARTController6;
	static UARTCommsController * 	s_pUARTController7;
	static UARTCommsController * 	s_pUARTController8;

private:

	void Reset();
	uint8_t CalculateChecksum();

	UART_HandleTypeDef * 		m_pHUART;
	FIFOBuffer 					m_TxBuffer;
	FIFOBuffer 					m_RxBuffer;

	/* Variable to be fed as a temp storage to interrupt driven Receive function of HAL */
	uint8_t 					m_IncomingRxChar;
	uint8_t						m_OutgoingTxChar;

	uint8_t						m_NewCommand[MAX_COMMAND_SIZE];
	uint8_t *					m_pNewCommandChar;

	enum EParserState{
		EParserState_Idle,
		EParserState_Command,
		EParserState_Checksum1,
		EParserState_Checksum2,
	}m_ParserState;

	bool						m_bDebugChannel;
	char 						m_RxCRCString[3];
	uint8_t						m_RxCRC;

	static const unsigned int   s_MaxControllerCount = 50;
	CThreadBase *				m_pControllers[s_MaxControllerCount];
	int 						m_ControllerCount;
};

#endif /* UARTCOMMSCONTROLLER_H_ */
