#include "PreCompile.h"
#include "EchoClient.h"
#include <random>

#define MAX_SEND_SIZE 15

std::string gen_random(const int len) 
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

EchoClient::EchoClient(const std::wstring& optionFile)
{
    Start(optionFile.c_str());
}

EchoClient::~EchoClient()
{
    Stop();
}

void EchoClient::OnConnectionComplete()
{
    std::cout << "connected" << std::endl;

    CNetServerSerializationBuf& buffer = *CNetServerSerializationBuf::Alloc();
    int inputId = 1;
    buffer << inputId;
    MakeRandomString(buffer);

    SendPacket(&buffer);
}

void EchoClient::OnRecv(CNetServerSerializationBuf* OutReadBuf)
{
    int outputId;
    *OutReadBuf >> outputId;
    if (outputId != 1)
    {
        g_Dump.Crash();
    }

    char recvStringBuffer[30];
    ZeroMemory(recvStringBuffer, sizeof(recvStringBuffer));
    OutReadBuf->ReadBuffer(recvStringBuffer, beforeSendSize);

    std::cout << "send : " << echoString << " / " << "recv : " << recvStringBuffer << std::endl;

    if (strcmp(recvStringBuffer, echoString.c_str()) != 0)
    {
        g_Dump.Crash();
    }

    CNetServerSerializationBuf sendBuffer;
    UINT inputId = 1;
    sendBuffer << inputId;
    MakeRandomString(sendBuffer);
     
    SendPacket(&sendBuffer);
}

void EchoClient::OnSend(int sendsize)
{

}

void EchoClient::OnWorkerThreadBegin()
{

}

void EchoClient::OnWorkerThreadEnd()
{

}

void EchoClient::OnError(st_Error* OutError)
{
    std::cout << OutError->ServerErr << " / " << OutError->GetLastErr << std::endl;
}

void EchoClient::MakeRandomString(OUT CNetServerSerializationBuf& buffer)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(10, MAX_SEND_SIZE);
    beforeSendSize = distr(gen);
    echoString = gen_random(beforeSendSize);

    buffer.WriteBuffer(const_cast<char*>(echoString.c_str()), beforeSendSize);
}