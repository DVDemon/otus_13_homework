#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include "parser.h"
using boost::asio::ip::tcp;

namespace homework
{
/**
   * Сессия для работы с конкретным клиентом
   */
class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(tcp::socket socket);
  /**
   * Старт взаимодействия
   */
  void start();

private:
  /**
   * Считывание данных из потока
   */
  void do_read();

  void do_write(std::string str);
  /**
   * Сокет соединения с клиентом
   */

  tcp::socket socket_;
  Parser parser; 

  enum
  {
    max_length = 1024
  };
  char data_[max_length];
};
} // namespace homework

#endif