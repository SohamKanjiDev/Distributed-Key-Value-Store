#include "kvstore.h"
#include <sqlite3.h>
#include <iostream>

constexpr static char* CREATE_TABLE_SQL = "CREATE TABLE IF NOT EXISTS kvstore (key TEXT PRIMARY KEY, value TEXT);";
constexpr static char* DUMP_KEY_VALUE_SQL = "REPLACE INTO kvstore (key, value) VALUES (?, ?);";
constexpr static char* SELECT_ALL_SQL = "SELECT * FROM kvstore;";

KVStore::KVStore(const std::string& db_name) {
    loadFromDB(db_name);
}

void KVStore::set(const std::string& key, const std::string& value) {
    std::scoped_lock lock(m_mutex);
    m_store[key] = value;
}

std::optional<std::string> KVStore::get(const std::string& key) const {
    std::scoped_lock lock(m_mutex);
    if (m_store.find(key) == m_store.end()) {
        return std::nullopt;
    }
    return m_store.at(key);
}

void KVStore::remove(const std::string& key) {
    std::scoped_lock lock(m_mutex);
    if (m_store.find(key) == m_store.end()) {
        throw std::runtime_error(key + " doesn't exist in the store."); 
    }
    m_store.erase(key);
}

void KVStore::updateDB(const std::string& db_name) const
{
    std::scoped_lock lock(m_mutex);
    sqlite3* db;
    if(sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Failed to open db." << std::endl;
        return;
    }
    char* error_message;
    if(sqlite3_exec(db, CREATE_TABLE_SQL, nullptr, 0, &error_message)) {
        std::cerr << "Failed to create table: " << error_message << std::endl;
        sqlite3_free(error_message);
        return;
    }

    sqlite3_stmt* statement;
    if(sqlite3_prepare_v2(db, DUMP_KEY_VALUE_SQL, -1, &statement, nullptr)) {
        std::cerr << "Failed to prepare statement" << std::endl;
    }

    for(const auto& [key, value]: m_store) {
        sqlite3_bind_text(statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 2, value.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(statement);
        sqlite3_reset(statement);
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);
}

static int load_in_memory(void* data, int number_of_columns, char** column_values, char**)
{
    auto store_instance = static_cast<KVStore*>(data);
    if(store_instance == nullptr || number_of_columns != 2) {
        return 1;
    }
    if(column_values[0] != nullptr && column_values[1] != nullptr) {
        store_instance->set(column_values[0], column_values[1]);
    }
    
    return 0;
}

void KVStore::loadFromDB(const std::string& db_name) {
    sqlite3* db;
    if(sqlite3_open(db_name.c_str(), &db)) {
        std::cerr << "Failed to open db." << std::endl;
        return;
    }
    char* error_message;
    if(sqlite3_exec(db, SELECT_ALL_SQL, load_in_memory, this, &error_message)) {
        std::cerr << "Error while loading data: " << error_message << std::endl;
        sqlite3_free(error_message);
    }
    sqlite3_close(db);
}


