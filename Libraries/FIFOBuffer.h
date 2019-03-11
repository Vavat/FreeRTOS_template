/*
 * FIFOBuffer.h
 *
 *  Created on: 15 Apr 2017
 *      Author: Vavat
 */

#ifndef FIFOBUFFER_H_
#define FIFOBUFFER_H_

class FIFOBuffer {
public:
	/* @brief: 	Create a fifo buffer of characters.
	 * @arg:	number of slots requested.
	 * @note:	requested number of slots will be range forced between min and max.
	 */
	FIFOBuffer(unsigned int size);
	virtual ~FIFOBuffer();

	/* @brief:	Request next value from the buffer.
	 * @arg:	pointer to variable to be filled with next value from FIFO
	 * @retval:	true if there are more values available
	 */
	bool Get(char * pData = 0);

	/* @brief:	Store value into the buffer.
	 * @arg:	variable to be stored in the FIFO
	 * @retval:	true if there are more values available
	 */
	bool Set(const char Data);

	/* @brief:	Test if buffer is full
	 * @retval:	true if buffer is full
	 */
	bool IsFull() const;

	/* @brief:	Test if buffer is empty
	 * @retval:	true if buffer is empty
	 */
	bool IsEmpty() const;

	/* @brief:	Request total size of the buffer
	 * @retval:	total size of buffer in storage units
	 */
	unsigned int GetSize() const { return m_Size; };

	/* @brief:	Request number of filled slots
	 * @retval:	total number of filled slots
	 */
	unsigned int GetCount() const { return m_Count; };

	/* @brief:	Request number of available slots
	 * @retval:	total number of empty slots
	 */
	unsigned int GetFreeSpace() const {return (m_Size - m_Count); };

private:

	void CheckWrapping();

	char * m_pHead;
	char * m_pTail;
	char * m_pOrigin;

	unsigned int m_Count;
	unsigned int m_Size;
};

#endif /* FIFOBUFFER_H_ */
