#pragma once

#include <memory>
#include <thread>
#include <string>

#include "ICommunicationService.hpp"
#include "NetworkMessage.hpp"
#include "ITcpSocket.hpp"
#include "IMessageQueue.hpp"
#include "MessageWriter.hpp"
#include "Console.hpp"

class CommunicationService : public ICommunicationService
{
public:
	CommunicationService(TcpSocketPtr tcpSocket,
		MessageQueuePtr messageQueue, MessageWriterPtr messageWritter) :
			messageQueue_(messageQueue),
			tcpSocket_(std::move(tcpSocket)),
			messageWriter_(messageWritter),
			console_("CommunicationService") {}

	void startService(const std::string& host, const std::string& port);
	void putMessageInQueue(NetworkMessage&& message);
	void putMessageInQueue(const NetworkMessage& message);
	void tearDown();

private:
	MessageQueuePtr messageQueue_;
	TcpSocketPtr tcpSocket_;
	MessageWriterPtr messageWriter_;
	ThreadPtr writerThread_;
	Console console_;
};