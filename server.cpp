#include "server.h"
#include "session.h"

#include <iostream>

homework::Server::Server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)){
    do_accept();
}

void homework::Server::do_accept(){
    //std::cout << "start accepting ..." << std::endl;
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec)
            {
                //std::cout << "start create session ..." << std::endl;
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
}