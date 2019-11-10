#include "parser.h"
#include <iostream>
#include "utils.h"
#define UNUSED(value) (void)value

void homework::Parser::parse_input_stream(){
    const size_t buffer_size = 5;
    char buffer[buffer_size];
    size_t pos{};
    int   c{};

    while( (c= std::cin.get())>0){
        buffer[pos] = c; ++pos;
        if(pos>=buffer_size){
            pos=0;
            parse(buffer,buffer_size);
        }
    }
    parse(buffer,pos);
}

std::vector<std::string> get_commands(std::string input){
    size_t i = 0;
    std::vector<std::string> res;

    while(i<input.length()){
        size_t space = input.find(' ',i);
        if(space!=std::string::npos) {
            if(space>i)
                res.push_back(input.substr(i,space-i));
            i=space+1;
            
        } else {
            res.push_back(input.substr(i,input.length()-i));
            break;
        }
    }
    return res;
}


void homework::Parser::init(){
    handlers["INSERT"] = [this](std::vector<std::string>& val) ->std::string {
        
        if(val.size()==3){
                Table t;
                if(val[0]==std::string("A"))  t = Table::A;
                 else
                if(val[0]==std::string("B"))  t = Table::B;
                 else return std::string("ERR Wrong syntax - unknown table name");

                if(!is_integer(val[1])) return std::string("ERR Wrong syntax - id is not possitive integer");
                return database.insert(t,atoi(val[1].c_str()),val[2]);
            }

        return std::string("ERR");
        };

    handlers["TRUNCATE"] = [this](std::vector<std::string>& val) ->std::string {
           if(val.size()>0){
                if(val[0]==std::string("A")) return database.truncate(Table::A);
                if(val[0]==std::string("B")) return database.truncate(Table::B);
            }
            return std::string("ERR Unknown table");
        };

    handlers["INTERSECTION"] = [this](std::vector<std::string>& val) ->std::string {
        UNUSED(val);
        return database.intersection();
        };

    handlers["SYMMETRIC_DIFFERENCE"] = [this](std::vector<std::string>& val) ->std::string {
        UNUSED(val);
        return database.symmetric_difference();
        };

    handlers["SELECT"] = [this](std::vector<std::string>& val) ->std::string {
            if(val.size()>0){
                if(val[0]==std::string("A")) return database.select(Table::A);
                if(val[0]==std::string("B")) return database.select(Table::B);
            }
            return std::string("ERR Unknown table");
         };
}

std::string homework::Parser::parse(const char* str, size_t str_size){
    static std::string buffer {};
    std::string result{};
    std::call_once(inited,[this](){this->init();});
   
    buffer+= std::string(str,str_size);
    buffer=replace(buffer,std::string("\r"),std::string(" "));

    size_t length{};
    if((length = buffer.find('\n'))!=std::string::npos){
        std::string cmd = buffer.substr(0,length);
        buffer = buffer.substr(length+1,buffer.size()-length);
        for(auto c : handlers){
            if(cmd.find(c.first)!=std::string::npos){
                std::string parameters {};
                if(cmd.length()>(c.first.length()+1))
                    parameters = cmd.substr(c.first.length()+1,cmd.length()-c.first.length()-1);
                
                std::vector<std::string> param_vector(get_commands(parameters));
                result += c.second(param_vector);
                result += "\n";
            }
        }
    }
    return result;

}