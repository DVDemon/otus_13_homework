#include <iostream>
#include <cstdlib>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "server.h"
#include "parser.h"
#include "database.h"
using boost::asio::ip::tcp;

bool isPositiveInteger(const std::string&& s){
    return !s.empty() && 
           (std::count_if(s.begin(), s.end(), [](auto a){return (a>='0'&&a<='9');}) == (long)s.size());
}

auto main(int argc,char * argv[]) -> int{
  try
  {

    if(argc>1)
    if(isPositiveInteger(std::string(argv[1])))
    {
        //homework::Parser p;
        //p.parse_input_stream();
        int    port_number = atoi(argv[1]);
        boost::asio::io_context io_context;
        homework::Server server(io_context,port_number);
        io_context.run();
        
        return 1;
    }

    std::cerr << "Usage: join_server <port>\n";
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << "\n";
  }
  return 0;
}