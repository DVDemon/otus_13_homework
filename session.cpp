#include "session.h"
#include <iostream>
#include <memory>
#include <string>

homework::Session::Session(tcp::socket socket) : 
socket_(std::move(socket)){
  //std::cout << "session created ... " << std::endl;
}

void homework::Session::start(){
  //std::cout << "start read ... " << std::endl;
    do_read();
}

void homework::Session::do_read(){
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length) {

                                if (!ec)
                                {
                                    //std::cout << "start parse ... " << std::endl;
                                    std::string result = parser.parse(data_,length);
                                    do_write(result);
                                }
                            });
}

void homework::Session::do_write(std::string str)
  {
    //std::cout << "start write ... " << std::endl;
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(str.c_str(), str.length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            //std::cout << "start read ... " << std::endl;
            do_read();
          }
        });
  }

