#include "kvstore.h"

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
