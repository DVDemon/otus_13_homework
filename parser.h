#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <map>
#include <functional>
#include <mutex>
#include <vector>
#include "database.h"

namespace homework{
    class Parser{
        public:
            std::string parse(const char*, size_t);
            void parse_input_stream();
            void init();
        private:
            using handler_t = std::function<std::string(std::vector<std::string>&)>;
            std::once_flag inited;
            std::map<std::string,handler_t> handlers;
            Database database;
    };
}

#endif