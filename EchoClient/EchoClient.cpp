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
    UINT id = 1;
    buffer << id;
    MakeRandomString(buffer);

    SendPacket(&buffer);
}

void EchoClient::OnRecv(CNetServerSerializationBuf* OutReadBuf)
{
    std::string recvBuffer;
    OutReadBuf->ReadBuffer(const_cast<char*>(recvBuffer.c_str()), 14);
    //OutReadBuf->ReadBuffer(const_cast<char*>(recvBuffer.c_str()), beforeSendSize);

    std::cout << "send : " << echoString << std::endl << "recv : " << recvBuffer << std::endl;

    if (echoString != "testString")
    {
        g_Dump.Crash();
    }
    //if (recvBuffer != echoString)
    //{
    //    g_Dump.Crash();
    //}

    CNetServerSerializationBuf buffer;
    MakeRandomString(buffer);

    SendPacket(&buffer);
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

    //buffer.WriteBuffer(const_cast<char*>(echoString.c_str()), beforeSendSize);
    std::string t;
    t = "testString";
    buffer.WriteBuffer((char*)t.c_str(), t.size());
}