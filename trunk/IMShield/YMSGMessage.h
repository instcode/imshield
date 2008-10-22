#ifndef _YMSGMESSAGE_H_
#define _YMSGMESSAGE_H_

class YMSGHeader
{
	byte m_ymsgHeader[YES_PACKET_HEADER_SIZE];

public:
	int BuildHeader(byte* ymsgHeader);
	void SetPayloadLength(int nLength);
	void SetProtocolVersion(int nVersion);
	void SetRequestService(int nCommand);
	void SetCommandStatus(byte *cmd_status);
	void SetSessionID(byte *session_id);

	int GetPayloadLength();
	int GetRequestService();
	byte* GetHeaderData();
};

struct YMSGField
{
	int key;
	char* value;
};

class YMSGHandler;

class YMSGMessage
{
public:
	static const int YMSG_PACKET_PARSING_INVALID_HEADER = -1;
	static const int YMSG_PACKET_PARSING_REQUIRE_BUFFER = -2;
	static const int YMSG_PACKET_PARSING_SUCESSFULLY = 0;

private:
	int	m_nFieldCount;
	int	m_nFieldCapacity;

protected:
	YMSGHeader m_ymsgHeader;
	YMSGField* m_pFields;

private:
	void AllocateFieldStorage();
	void AddField(int nKey, char* szValue);

public:
	YMSGMessage();
	virtual ~YMSGMessage();

public:
	int ParseHeader(byte* pBuffer, int nLength);
	int ParseFields(byte* pBuffer, YMSGHandler* pHandler);
	
	int GetFieldCount();
	YMSGHeader& GetHeader();
	YMSGField& GetField(int nIndex);
};

class YMSGMessageBuilder
{
	byte* m_pBuffer;

public:
	YMSGMessageBuilder();
	void AppendField(YMSGField& ymsgField);
	byte* BuildPacketBuffer(YMSGHeader& ymsgHeader);
};

class YMSGService;

class YMSGHandler
{
	YMSGService *m_pService;

public:
	YMSGHandler(YMSGService *pService)
	{
		m_pService = pService;
	}

	virtual void VisitField(YMSGField& ymsgField) = 0;
};

class YMSGService
{
	YMSGMessage m_ymsgMessage;
	byte*		m_pBuffer;
	int			m_nBufferSize;
	int			m_nBufferCapacity;

private:
	int AllocateBuffer(int nSize);
	byte* BeginPrepareBuffer(byte* lpBuffer, int nSize);
	void EndPrepareBuffer(byte* lpBuffer, int nSize);

public:
	YMSGService();
	~YMSGService();

	void HandleBuffer(byte* lpBuffer, int nLength);
	void ParseMessage(byte* lpBuffer);
	void HandleServerMessage(byte* lpBuffer, int nLength);
	void HandleClientMessage(byte* lpBuffer, int nLength);
};

class YMSGLoginHandler: public YMSGHandler
{
	char m_szLoginUser[32];
public:
	YMSGLoginHandler(YMSGService *pService);

	char* GetLoginUser();
	void VisitField(YMSGField& ymsgField);
};

class YMSGChatHandler: public YMSGHandler
{
	char m_szSender[32];
	char m_szReceiver[32];
public:
	YMSGChatHandler(YMSGService *pService);

	void VisitField(YMSGField& ymsgField);
};

class YMSGMessageUtil
{
public:
	static void GetPlainChatText(const char* lpszChatText, char* lpszPlainText);
};

#endif