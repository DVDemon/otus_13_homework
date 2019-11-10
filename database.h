#ifndef DATABASE_H
#define DATABASE_H


#include <string>
#include <mutex>
#include "sqlite3.h"

namespace homework{
    enum class Table{
        A,
        B
    };

    class Database{
        public:
            Database();
            std::string insert(Table t,size_t id,const std::string &value);
            std::string truncate(Table t);
            std::string select(Table t);
            std::string intersection();
            std::string symmetric_difference();
        private:
            sqlite3*       handle;
            std::mutex     mtx;
    };
}

#endif