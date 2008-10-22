#include <windows.h>
#include <stdio.h>

#include "System.h"
#include "IMShield.h"
#include "YMSGService.h"
#include "YMSGMessage.h"

////////////////
// YMSGHeader //
////////////////
int YMSGHeader::BuildHeader(byte* ymsgHeader)
{
	if (ymsgHeader == NULL)
		return FALSE;

	const BYTE INDENTIFIERS[] = { 'Y','M','S','G' };
	for (int i = 0; i < 4; ++i) {
		if (ymsgHeader[i] != INDENTIFIERS[i])
			return FALSE;
	}
	//SysTrace("0x%02X ", ymsgHeader[i]);
	memcpy(m_ymsgHeader, ymsgHeader, YES_PACKET_HEADER_SIZE);
	return TRUE;
}

void YMSGHeader::SetProtocolVersion(int nVersion)
{
	GetByteArrayFromShort(nVersion, m_ymsgHeader + 4);
}

void YMSGHeader::SetPayloadLength(int nLength)
{
	GetByteArrayFromShort(nLength, m_ymsgHeader + 8);
}

void YMSGHeader::SetRequestService(int nCommand)
{
	GetByteArrayFromShort(nCommand, m_ymsgHeader + 10);
}

void YMSGHeader::SetCommandStatus(byte *cmd_status)
{
	memcpy(m_ymsgHeader + 12, cmd_status, 4);
}

void YMSGHeader::SetSessionID(byte *session_id)
{
	memcpy(m_ymsgHeader + 16, session_id, 4);
}

int YMSGHeader::GetPayloadLength()
{
	return ntohs(*((short*)(m_ymsgHeader + 8)));
}

int YMSGHeader::GetRequestService()
{
	return ntohs(*((short*)(m_ymsgHeader + 10)));
}

byte* YMSGHeader::GetHeaderData()
{
	return m_ymsgHeader;
}

/////////////////
// YMSGMessage //
/////////////////
YMSGMessage::YMSGMessage()
{
	m_nFieldCount = 0;
	m_nFieldCapacity = 0;
}

YMSGMessage::~YMSGMessage()
{
	if (m_pFields == NULL)
		return;

	for (int i = 0; i < m_nFieldCount; i++) {
		delete [] m_pFields[i].value;
	}
	delete [] m_pFields;
	m_pFields = NULL;
}

void YMSGMessage::AllocateFieldStorage()
{
	if (m_nFieldCapacity == 0) {
		m_nFieldCapacity = 5;
		m_pFields = new YMSGField[m_nFieldCapacity];
	}
	else {
		m_nFieldCapacity *= 2;
		YMSGField *pYMSGFields = new YMSGField[m_nFieldCapacity];
		memcpy(pYMSGFields, m_pFields, m_nFieldCount * sizeof (YMSGField));
		delete [] m_pFields;
		m_pFields = pYMSGFields;
	}
	SysTrace("## Allocated - Usage/Capacity: %d/%d\n", m_nFieldCount, m_nFieldCapacity);
}

YMSGHeader& YMSGMessage::GetHeader()
{
	return m_ymsgHeader;
}

YMSGField& YMSGMessage::GetField(int nIndex)
{
	return m_pFields[nIndex];
}

int YMSGMessage::GetFieldCount()
{
	return m_nFieldCount;
}

int YMSGMessage::ParseHeader(byte* pBuffer, int nLength)
{
	if (pBuffer == NULL || nLength < YES_PACKET_HEADER_SIZE ||
		!m_ymsgHeader.BuildHeader(pBuffer))
	{
		return YMSG_PACKET_PARSING_INVALID_HEADER;
	}

	int nPayloadLength = m_ymsgHeader.GetPayloadLength();
	if (nPayloadLength > nLength - YES_PACKET_HEADER_SIZE)
		return YMSG_PACKET_PARSING_REQUIRE_BUFFER;
	return YMSG_PACKET_PARSING_SUCESSFULLY;
}

int YMSGMessage::ParseFields(byte* pBuffer, YMSGHandler* pHandler)
{
	int nPayloadLength = m_ymsgHeader.GetPayloadLength();
	byte* pBuffer1 = pBuffer + YES_PACKET_HEADER_SIZE;
	byte* pBuffer2 = pBuffer1;
	byte* pBufferEnd = pBuffer + YES_PACKET_HEADER_SIZE + nPayloadLength;
	int nKey = 0;
	BOOL isKeyNext = TRUE;

	// Process YMSGField (key & value) pairs
	while (pBuffer1 < pBufferEnd)
	{
		if (pBuffer1[0] == 0xC0 && pBuffer1[1] == 0x80) {
			int nLength = pBuffer1 - pBuffer2;
			if (nLength > 0) {
				char* szData = new char[nLength + 1];
				memcpy(szData, pBuffer2, nLength);
				szData[nLength] = NULL;

				if (isKeyNext) {
					isKeyNext = FALSE;
					nKey = atoi(szData);
					delete [] szData;
				}
				else {
					isKeyNext = TRUE;
					if (nKey < MAX_YMSG_FIELD_KNOWN_COUNT) {
						SysTrace("#%s# (%d & %s)\n", YMSG_FLD_NAME_MAPPING[nKey], nKey, szData);
					}
					else {
						SysTrace("#Unknown# (%d & %s)\n", nKey, szData);
					}
					AddField(nKey, szData);
					if (pHandler != NULL)
						pHandler->VisitField(m_pFields[m_nFieldCount - 1]);
				}
			}
			++pBuffer1;
			pBuffer2 = pBuffer1 + 1;
		}
		++pBuffer1;
	}
	return 0;
}

void YMSGMessage::AddField(int nKey, char* szValue) {
	if (m_nFieldCount == m_nFieldCapacity)
		AllocateFieldStorage();

	m_pFields[m_nFieldCount].value = szValue;
	m_pFields[m_nFieldCount].key = nKey;
	
	++m_nFieldCount;
}

/////////////////////
// YMSGMessageUtil //
/////////////////////
void YMSGMessageUtil::GetPlainChatText(const char* lpszChatText, char* lpszPlainText)
{
	LPCTSTR lpszTokenBegin = NULL;
	for (; *lpszChatText != NULL; lpszChatText++)
	{
		if ((*lpszChatText == '<') && (
			strnicmp(lpszChatText + 1, "font", 4) == 0 ||
			strnicmp(lpszChatText + 1, "fade", 4) == 0 ||
			strnicmp(lpszChatText + 1, "alt", 3) == 0 ||
			strnicmp(lpszChatText + 1, "/font", 5) == 0 ||
			strnicmp(lpszChatText + 1, "/fade", 5) == 0 ||
			strnicmp(lpszChatText + 1, "/alt", 4) == 0))
		{
			lpszTokenBegin = lpszChatText;
			for (lpszChatText += 4; *lpszChatText != '>'; lpszChatText++)
			{
				if (*lpszChatText == NULL)
				{
					lpszChatText = lpszTokenBegin;
					break;
				}
			}
		}
		else {
			// Escaped characters
			if (*lpszChatText == 0x1B && *(lpszChatText + 1) == 0x5B)
			{
				lpszTokenBegin = lpszChatText;
				for (lpszChatText += 2; *lpszChatText != 0x6D; lpszChatText++)
				{
					if (*lpszChatText == NULL)
					{
						lpszChatText = lpszTokenBegin;
						*(lpszPlainText++) = (*lpszChatText);
						break;
					}
				}
			}
			else { 
				*(lpszPlainText++) = (*lpszChatText);
			}
		}
	}
	// Terminate string
	*lpszPlainText = NULL;
}

/////////////////
// YMSGService //
/////////////////
YMSGService::YMSGService()
{
	m_nBufferSize = 0;
	m_nBufferCapacity = 0;
	m_pBuffer = NULL;
}

YMSGService::~YMSGService()
{
	m_nBufferCapacity = 0;
	if (m_pBuffer != NULL) {
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}

byte* YMSGService::BeginPrepareBuffer(byte* lpBuffer, int dwBytes)
{
	FILE* hFile = fopen("C:\\ymsg_debug_log.txt", "a+");
	fprintf(hFile, "\nBytes received: %d\n", dwBytes);
	fwrite(lpBuffer, sizeof(byte), dwBytes, hFile);
	fprintf(hFile, "\n");
	fclose(hFile);

	if (m_nBufferSize > 0) {
		int nPayloadLength = m_ymsgMessage.GetHeader().GetPayloadLength();
		int nNeeded = YES_PACKET_HEADER_SIZE + nPayloadLength - m_nBufferSize;
		int nAvailable = nNeeded;
		if (nAvailable > dwBytes) {
			nAvailable = dwBytes;
		}
		memcpy(m_pBuffer + m_nBufferSize, lpBuffer, nAvailable);
		m_nBufferSize += nAvailable;
		SysTrace("Allocated packet buffer - usage/available %d/%d...\n", m_nBufferSize, m_nBufferCapacity);
		if (m_nBufferSize == nPayloadLength + YES_PACKET_HEADER_SIZE) {
			// Parse our buffer instead
			return m_pBuffer;
		}
		return NULL;
	}
	else {
		int nError = m_ymsgMessage.ParseHeader(lpBuffer, dwBytes);
		// Check whether it's an invalid YMSG packet
		if (nError == YMSGMessage::YMSG_PACKET_PARSING_INVALID_HEADER) {
			return NULL;
		}
		if (nError == YMSGMessage::YMSG_PACKET_PARSING_REQUIRE_BUFFER) {
			int nPayloadLength = m_ymsgMessage.GetHeader().GetPayloadLength();
			AllocateBuffer(nPayloadLength + YES_PACKET_HEADER_SIZE);
			memcpy(m_pBuffer, lpBuffer, dwBytes);
			m_nBufferSize += dwBytes;
			SysTrace("Allocated packet buffer - usage/available %d/%d...\n", m_nBufferSize, m_nBufferCapacity);
			return NULL;
		}
		// Parsed YMSG packet header successfully!
		return lpBuffer;
	}
}

void YMSGService::EndPrepareBuffer(byte* lpBuffer, int nSize)
{
	// Reset size of buffer used
	m_nBufferSize = 0;
}

int YMSGService::AllocateBuffer(int nSize)
{
	// Only allocate buffer if it's necessary
	if (m_nBufferCapacity >= nSize)
		return m_nBufferCapacity;
	m_nBufferCapacity = nSize;

	// Clean old buffer
	if (m_pBuffer != NULL)
		delete [] m_pBuffer;
	m_pBuffer = new byte [m_nBufferCapacity];
	return m_nBufferCapacity;
}

void YMSGService::HandleServerMessage(byte* lpBuffer, int dwBytes)
{

}

void YMSGService::HandleClientMessage(byte* lpBuffer, int dwBytes)
{

}

void YMSGService::ParseMessage(byte* lpBuffer)
{
	YMSGHeader ymsgHeader = m_ymsgMessage.GetHeader();

	int nService = ymsgHeader.GetRequestService();

	SysTrace("###############################\n");
	SysTrace("Handle service [%d]...", nService);
	if (nService < YES_FEATURE_NOT_SUPPORTED)
	{
		SysTrace("Service Name: [%s]\n", YES_NAME_MAPPING_BASIC[nService]);
	}
	else if (nService >= YES_NAME_MAPPING_ADVANCED_BEGIN && nService <= YES_NAME_MAPPING_ADVANCED_END)
	{
		SysTrace("Service Name: [%s]\n",
			YES_NAME_MAPPING_ADVANCED[nService - YES_NAME_MAPPING_ADVANCED_BEGIN]);
	}
	else if (nService == YES_NEWS_ALERTS)
	{
		SysTrace("Service Name: [YES_NEWS_ALERTS]\n");
	}
	else {
		SysTrace("\n");
	}

	switch (nService)
	{
	case YES_HELO:
		{
			YMSGLoginHandler handler(this);
			m_ymsgMessage.ParseFields(lpBuffer, &handler);
		}
		break;

	case YES_CHAT_MSG:
		{
			YMSGChatHandler handler(this);
			m_ymsgMessage.ParseFields(lpBuffer, &handler);
		}
		break;

	default:
		m_ymsgMessage.ParseFields(lpBuffer, NULL);
	}
	SysTrace("Finish current incoming message.\n");
}

void YMSGService::HandleBuffer(byte* lpBuffer, int dwBytes)
{
	byte* pYMSGBuffer = BeginPrepareBuffer(lpBuffer, dwBytes);
	if (pYMSGBuffer == NULL) {
		return;
	}
	ParseMessage(pYMSGBuffer);
	EndPrepareBuffer(lpBuffer, dwBytes);
}

//////////////////////
// YMSGLoginHandler //
//////////////////////
YMSGLoginHandler::YMSGLoginHandler(YMSGService *pService):
	YMSGHandler(pService)
{
}

char* YMSGLoginHandler::GetLoginUser()
{
	return m_szLoginUser;
}

void YMSGLoginHandler::VisitField(YMSGField& ymsgField)
{
	if (YMSG_FLD_CURRENT_ID == ymsgField.key) {
		strcpy(m_szLoginUser, ymsgField.value);
	}
	else if (YMSG_FLD_CHALLENGE == ymsgField.key) {
		SysTrace("Session: 0x%08X\n", ymsgField.value);
	}
}

/////////////////////
// YMSGChatHandler //
/////////////////////
YMSGChatHandler::YMSGChatHandler(YMSGService *pService):
	YMSGHandler(pService)
{
}

void YMSGChatHandler::VisitField(YMSGField& ymsgField)
{
	if (YMSG_FLD_CURRENT_ID == ymsgField.key) {
	}
	else if (YMSG_FLD_SENDER == ymsgField.key) {
		strcpy(m_szSender, ymsgField.value);
	}
	else if (YMSG_FLD_TARGET_USER == ymsgField.key) {
		strcpy(m_szReceiver, ymsgField.value);
	}
	else if (YMSG_FLD_CHAT_MSG == ymsgField.key) {
		if (stricmp(m_szSender, m_szReceiver) == 0) {
			char pszOut[4096];
			YMSGMessageUtil::GetPlainChatText(ymsgField.value, pszOut);
			SysTrace("Text: %s\n", pszOut);
		}
	}
}
