/*
 * FIFOBuffer.cpp
 *
 *  Created on: 15 Apr 2017
 *      Author: Vavat
 */

#include "FIFOBuffer.h"
#include "stdlib.h"	/* calloc() */

#define MAX_SIZE	1024
#define MIN_SIZE	16

FIFOBuffer::FIFOBuffer(unsigned int size) {
	/* range checking */
	if(size < MIN_SIZE) { size = MIN_SIZE; };
	if(size > MAX_SIZE) { size = MAX_SIZE; };
	/* allocate memory for a buffer */
	m_pOrigin = (char *)calloc(size, sizeof(char));
	if(m_pOrigin == (char *) NULL)	{
		for(;;); /*Fail here*/
	}
	else {
		m_pHead = m_pOrigin;
		m_pTail = m_pOrigin;
		m_Size = size;
		m_Count = 0;
	}
}

FIFOBuffer::~FIFOBuffer() {
	free(m_pOrigin);
	m_pOrigin = NULL;
	m_pHead = NULL;
	m_pTail = NULL;
}

bool FIFOBuffer::Set(const char Data){
	bool bSpaceAvailable;
	if(IsFull()){
		bSpaceAvailable = false;
	}
	else{
		*m_pHead = Data;
		m_pHead++;
		m_Count++;
		/* wrapping? */
		CheckWrapping();
		bSpaceAvailable = !IsFull();
	}
	return bSpaceAvailable;
}

bool FIFOBuffer::Get(char * pData){
	bool bMoreAvailable;
	if(IsEmpty()){
		pData = (char*)NULL;
		bMoreAvailable = false;
	}
	else{
		if(pData){
			*pData = *m_pTail;
		}
		m_pTail++;
		m_Count--;
		/* wrapping? */
		CheckWrapping();
		bMoreAvailable = !IsEmpty();
	}
	return bMoreAvailable;
}

bool FIFOBuffer::IsFull() const{
	bool bFull;
	bFull = ((m_pHead == m_pTail) && (m_Count == m_Size));
	return bFull;
}

bool FIFOBuffer::IsEmpty() const{
	bool bEmpty;
	bEmpty = ((m_pHead == m_pTail) && (m_Count == 0));
	return bEmpty;
}

void FIFOBuffer::CheckWrapping(){
	if(m_pTail >= (m_pOrigin + m_Size))
	{
		m_pTail = m_pOrigin;
	}
	if(m_pHead >= (m_pOrigin + m_Size))
	{
		m_pHead = m_pOrigin;
	}
}
