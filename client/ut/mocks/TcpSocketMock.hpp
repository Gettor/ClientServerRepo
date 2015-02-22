#ifndef TCP_SOCKET_MOCK_HPP_
#define TCP_SOCKET_MOCK_HPP_

#include <gmock/gmock.h>
#include <memory>
#include <boost/asio.hpp>

#include "common/utilities/ITcpSocket.hpp"

class TcpSocketMock : public ITcpSocket
{
public:
    MOCK_METHOD2(connect, void(const std::string&, const std::string&));
    MOCK_METHOD1(write, void(std::string&));
    MOCK_METHOD1(write, void(const char*));
    MOCK_METHOD0(read, std::shared_ptr<const std::string>());
    MOCK_METHOD0(establishServer, std::shared_ptr<boost::asio::ip::tcp::acceptor>());
};
typedef std::shared_ptr<TcpSocketMock> TcpSocketMockPtr;

#endif  // TCP_SOCKET_MOCK_HPP_
