#ifndef SERVER_SESSION_HPP_
#define SERVER_SESSION_HPP_

#include "IServerSession.hpp"

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <atomic>
#include <thread>
#include <ctime>
#include <chrono>
#include <ratio>

#include "common/utilities/Console.hpp"
#include "common/messages/MessageUtilities.hpp"
#include "IBoostWrapper.hpp"
#include "ISocketServicesWrapper.hpp"

#include "modules/database/IDatabaseWrapper.hpp"

using namespace std::chrono;
using boost::asio::ip::tcp;
using messagePair = std::pair<common::messagetype::MessageType, std::shared_ptr<std::string> >;
using messagePairVec = std::vector<messagePair>;
using messageCounter = std::map<common::messagetype::MessageType, unsigned>;

class ServerSession : public std::enable_shared_from_this<ServerSession>, public IServerSession
{
public:
  ServerSession(std::shared_ptr<IBoostWrapper> wrapper, SocketServicePtr readerWrapper,
    SocketServicePtr writerWrapper, int socketNumber,
    std::shared_ptr<IDatabaseWrapper> databaseConnector, std::shared_ptr<Users> userArray,
    std::shared_ptr<std::mutex> serverMutex):
    wrapper_(wrapper),
    socketNumber_(socketNumber),
    userArray_(userArray),
    serverMutex_(serverMutex),
    stop_(false),
    readerWrapper_(readerWrapper),
    writerWrapper_(writerWrapper),
    databaseConnector_(databaseConnector),
    console_("ServerSession"),
    outputFile_("serverLogs/"+std::to_string(socketNumber-1)+".log")
  {
    console_.info << "New server session with socket number: " + std::to_string(socketNumber_);
    messageCounter_[common::messagetype::Incorrect] = 0;
    messageCounter_[common::messagetype::UpdateEnvironment] = 0;
    messageCounter_[common::messagetype::UpdatePlayer] = 0;
    messageCounter_[common::messagetype::OkResponse] = 0;
    messageCounter_[common::messagetype::Login] = 0;
    messageCounter_[common::messagetype::CurrentPlayerPosition] = 0;
    messageCounter_[common::messagetype::Logout] = 0;
    amountOfMessagesSent_ = 0;
    updateEnvironmentCounter_ = 0;
    bytesCounter_ = 0;
    totalTimeBetweenMessageReceiveAndSend_ = duration_cast<duration<double>>(high_resolution_clock::now() - high_resolution_clock::now());

    cellVertexes_.push_back(std::make_pair(0, 0));
  }

  std::shared_ptr<std::thread> start();
  void startThreadsAndRun(std::shared_ptr<IServerSession> self);
  void stop();
  void tearDown();
  bool wasClientLoggedInCorrectly();
  messageCounter getMessageCounter();
  messagePairVec getMessagePairVector();
  void cyclicPushReceivedMessages(common::messagetype::MessageType receivedMessageType,
    std::shared_ptr<std::string> messageString);

private:
  void runSession();
  int getMessage();
  void sendOtherPlayersUpdate();
  void sendOkResponse(bool serverAllows);
  void sendPlayerPosition(int x, int y, int z);
  void printMessageCounter();
  bool loginService();
  bool updatePlayerPositionByJson(std::string json);

  std::shared_ptr<IBoostWrapper> wrapper_;
  int socketNumber_;
  messagePairVec receivedMessages_;
  messageCounter messageCounter_;
  unsigned long long bytesCounter_;
  unsigned updateEnvironmentCounter_;
  unsigned amountOfMessagesSent_; 
  duration<double> totalTimeBetweenMessageReceiveAndSend_;
  high_resolution_clock::time_point timeBetweenMessageReceiveAndSend_;
  User userForSession_;
  std::shared_ptr<Users> userArray_;
  std::shared_ptr<std::mutex> serverMutex_;
  std::vector<std::pair<unsigned, unsigned> > cellVertexes_;


  std::atomic<bool> stop_;

  SocketServicePtr readerWrapper_;
  SocketServicePtr writerWrapper_;

  ThreadPtr readerThread_;
  ThreadPtr writerThread_;

  std::shared_ptr<IDatabaseWrapper> databaseConnector_;
  
  Console console_;
  std::ofstream outputFile_;

  static const int noMessage_ = -1;

};

#endif