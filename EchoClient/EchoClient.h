#pragma once
#include "NetClient.h"
#include "NetServerSerializeBuffer.h"
#include <string>

class EchoClient : public CNetClient
{
public:
	EchoClient() = delete;
	explicit EchoClient(const std::wstring& optionFile);
	virtual ~EchoClient();

public:
	virtual void OnConnectionComplete();
	virtual void OnRecv(CNetServerSerializationBuf* OutReadBuf);
	virtual void OnSend(int sendsize);

	virtual void OnWorkerThreadBegin();
	virtual void OnWorkerThreadEnd();
	virtual void OnError(st_Error* OutError);

private:
	void MakeRandomString(OUT CNetServerSerializationBuf& buffer);

private:
	std::wstring echoString;
	//int beforeSendSize = 0;
};