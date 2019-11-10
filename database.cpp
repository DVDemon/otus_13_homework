#include "database.h"
#include <exception>
#include <iomanip>
#include "utils.h"
#define UNUSED(value) (void)value

homework::Database::Database()
{
    std::lock_guard<std::mutex> lock(mtx);
    handle = nullptr;

    // open database
    if (sqlite3_open("join_server.sqlite", &handle))
    {
        std::string error{"sqlite3_open failed!"};
        error += sqlite3_errmsg(handle);
        throw std::logic_error(error);
    }

    // create tables
    char *errMsg = nullptr;

    const std::string createQueryA = "CREATE TABLE IF NOT EXISTS A(id int NOT NULL, name varchar(255), PRIMARY KEY(id));";
    if (sqlite3_exec(handle, createQueryA.c_str(), nullptr, nullptr, &errMsg))
    {
        std::string error{"sqlite3_exec with createQueryA failed!"};
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::logic_error(error);
    }

    const std::string createIndexA = "CREATE UNIQUE INDEX IF NOT EXISTS index_a ON A(id);";
    if (sqlite3_exec(handle, createIndexA.c_str(), nullptr, nullptr, &errMsg))
    {
        std::string error{"sqlite3_exec with createIndexA failed!"};
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::logic_error(error);
    }

    const std::string createQueryB = "CREATE TABLE IF NOT EXISTS B(id int NOT NULL, name varchar(255), PRIMARY KEY(id));";
    if (sqlite3_exec(handle, createQueryB.c_str(), nullptr, nullptr, &errMsg))
    {
        std::string error{"sqlite3_exec with createQueryB failed!"};
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::logic_error(error);
    }

    const std::string createIndexB = "CREATE UNIQUE INDEX IF NOT EXISTS index_b ON B(id);";
    if (sqlite3_exec(handle, createIndexB.c_str(), nullptr, nullptr, &errMsg))
    {
        std::string error{"sqlite3_exec with createIndexB failed!"};
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::logic_error(error);
    }
}



std::string homework::Database::insert(Table t, size_t id, const std::string &value)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::string insert_query;
    insert_query = "INSERT INTO ";
    switch (t)
    {
    case Table::A:
        insert_query += "A ";
        break;
    case Table::B:
        insert_query += "B ";
        break;
    }
    insert_query += "(id,name) VALUES (";
    std::string idstr = std::to_string(id);
    insert_query += idstr;
    insert_query += ",'";
    insert_query += replace(value, "'", "''");
    insert_query += "');";

    char *errMsg = nullptr;
    if (sqlite3_exec(handle, insert_query.c_str(), nullptr, nullptr, &errMsg))
    {
        sqlite3_free(errMsg);
        std::string res("ERR duplicate ");
        res += idstr;
        return res;
    }

    return std::string("OK");
}

std::string homework::Database::truncate(Table t)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::string truncate_query{"DELETE FROM "};
    switch (t)
    {
    case Table::A:
        truncate_query += "A;";
        break;
    case Table::B:
        truncate_query += "B;";
        break;
    }

    char *errMsg = nullptr;
    if (sqlite3_exec(handle, truncate_query.c_str(), nullptr, nullptr, &errMsg))
    {
        std::string error{"ERR sqlite3_exec with truncate failed!"};
        error += errMsg;
        sqlite3_free(errMsg);
        return error;
    }
    return std::string("OK");
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
    std::string *result = (std::string *)data;

    for (int i = 0; i < argc; i++)
    {
        *result += argv[i] ? argv[i] : "NULL";
        if (i < argc - 1)
            *result += ",";
    }

    *result += "\n";
    UNUSED(azColName);
    return 0;
}

std::string homework::Database::select(Table t)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::string truncate_query{"SELECT * FROM "};
    std::string result;
    switch (t)
    {
    case Table::A:
        truncate_query += "A;";
        break;
    case Table::B:
        truncate_query += "B;";
        break;
    }

    char *errMsg = nullptr;
    if (sqlite3_exec(handle, truncate_query.c_str(), callback, &result, &errMsg))
    {
        std::string error{"sqlite3_exec with select failed!"};
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::logic_error(error);
    }
    return result;
}

std::string homework::Database::intersection()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::string query{"SELECT A.id, A.name, B.name FROM A INNER JOIN B ON A.id=B.id;"};
    std::string result;

    char *errMsg = nullptr;
    if (sqlite3_exec(handle, query.c_str(), callback, &result, &errMsg))
    {
        std::string error{"ERR sqlite3_exec with select failed! "};
        error += errMsg;
        sqlite3_free(errMsg);
        return error;
    }
    return result;
}

std::string homework::Database::symmetric_difference()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::string query{"SELECT l.id, l.name, '' FROM A l WHERE NOT EXISTS (SELECT id FROM B r WHERE l.id=r.id);"};
    std::string result;

    char *errMsg = nullptr;
    if (sqlite3_exec(handle, query.c_str(), callback, &result, &errMsg))
    {
        std::string error{"ERR sqlite3_exec with select failed! "};
        error += errMsg;
        sqlite3_free(errMsg);
        return error;
    }

    query = "SELECT l.id, '', l.name FROM B l WHERE NOT EXISTS (SELECT id FROM A r WHERE l.id=r.id);";
    if (sqlite3_exec(handle, query.c_str(), callback, &result, &errMsg))
    {
        std::string error{"ERR sqlite3_exec with select failed! "};
        error += errMsg;
        sqlite3_free(errMsg);
        return error;
    }

    return result;
}
