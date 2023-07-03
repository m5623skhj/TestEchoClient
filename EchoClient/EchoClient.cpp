#include "PreCompile.h"
#include "EchoClient.h"
#include <random>
#include "../../RIOServerTest/RIO_Test/Protocol.h"
#include "../../RIOServerTest/RIO_Test/EnumType.h"

#define MAX_SEND_SIZE 15

std::wstring gen_random(const int len) 
{
    static const WCHAR alphanum[] =
        L"0123456789"
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        L"abcdefghijklmnopqrstuvwxyz";
    std::wstring tmp_s;
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
    CNetServerSerializationBuf& buffer = *CNetServerSerializationBuf::Alloc();
    UINT inputId = static_cast<UINT>(PACKET_ID::CALL_TEST_PROCEDURE_PACKET);
    int id3 = 6;
    buffer << inputId << id3;
    MakeRandomString(buffer);

    SendPacket(&buffer);
}

void EchoClient::OnRecv(CNetServerSerializationBuf* OutReadBuf)
{
    UINT outputId;
    *OutReadBuf >> outputId;

    CNetServerSerializationBuf& sendBuffer = *CNetServerSerializationBuf::Alloc();

    switch (static_cast<PACKET_ID>(outputId))
    {
    case PACKET_ID::CALL_TEST_PROCEDURE_PACKET_REPLY:
    {
        UINT inputId = static_cast<UINT>(PACKET_ID::CALL_SELECT_TEST_2_PROCEDURE_PACKET);
        CallSelectTest2ProcedurePacket selectPacket;
        selectPacket.id = 6;

        sendBuffer << inputId << selectPacket.id;
    }
    case PACKET_ID::CALL_SELECT_TEST_2_PROCEDURE_PACKET_REPLY:
    {
        UINT inputId = static_cast<UINT>(PACKET_ID::CALL_TEST_PROCEDURE_PACKET);
     
        sendBuffer << inputId;
        MakeRandomString(sendBuffer);
    }
    default:
        g_Dump.Crash();
        break;
    }
    
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
    //beforeSendSize = distr(gen);
    echoString = gen_random(distr(gen));

    buffer.WriteBuffer((char*)(echoString.c_str()), sizeof(WCHAR[30]));
}