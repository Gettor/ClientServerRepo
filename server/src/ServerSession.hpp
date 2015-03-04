#ifndef SERVER_SESSION_HPP_
#define SERVER_SESSION_HPP_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <atomic>
#include <thread>

#include "common/utilities/Console.hpp"
#include "common/socketServices/MessageQueue.hpp"
#include "common/socketServices/MessageReader.hpp"
#include "common/socketServices/MessageWriter.hpp"
#include "common/messages/MessageUtilities.hpp"
#include "ServerSocket.hpp"

using boost::asio::ip::tcp;

class ServerSession : public std::enable_shared_from_this<ServerSession>
{
public:
  ServerSession(boost::asio::io_service& io_service):
    socket_(std::make_shared<ServerSocket>(std::make_shared<tcp::socket>(io_service))),
    readerQueue_(std::make_shared<MessageQueue>()),
    writerQueue_(std::make_shared<MessageQueue>()),
    reader_(std::make_shared<MessageReader>(socket_, readerQueue_)),
    writer_(std::make_shared<MessageWriter>(socket_, writerQueue_)),
    console_("ServerSession")
  { }

  tcp::socket& getSocket();
  std::shared_ptr<std::thread> start();
  void startThreadsAndRun(std::shared_ptr<ServerSession> self);
  void stop();

private:
  void runSession();
  int getMessage();
  void sendResponse();


  std::shared_ptr<ServerSocket> socket_;
  std::multimap<common::messagetype::MessageType, std::shared_ptr<std::string>> receivedMessages_;
  std::atomic<bool> stop_;

  MessageQueuePtr readerQueue_;
  MessageQueuePtr writerQueue_;

  MessageCommanderPtr reader_;
  MessageCommanderPtr writer_;

  ThreadPtr readerThread_;
  ThreadPtr writerThread_;
  
  Console console_;

  static const int noMessage_ = -1;

};

#endif