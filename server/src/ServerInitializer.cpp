#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <stdexcept>

#include "ServerInitializer.hpp"

using boost::asio::ip::tcp;

ServerInitializer::ServerInitializer(boost::asio::io_service& ioService) :
    console_("ServerInitializer")
{
    acceptor_ = std::make_shared<tcp::acceptor>(ioService,
        tcp::endpoint(tcp::v4(), ServerInitializer::portNumber));
    session_ = std::make_shared<ServerSession>(acceptor_->get_io_service());
    acceptor_->async_accept(session_->socket(),
        boost::bind(&ServerInitializer::handleAccept, this, boost::asio::placeholders::error));
    ioService.run();
}

void ServerInitializer::handleAccept(const boost::system::error_code& error)
{
    if (!error)
    {
        session_->start();
        sessionArray_.push_back(session_);
        session_ = std::make_shared<ServerSession>(acceptor_->get_io_service());
        acceptor_->async_accept(session_->socket(),
            boost::bind(&ServerInitializer::handleAccept, this, boost::asio::placeholders::error));
    }
}