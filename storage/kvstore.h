#pragma once
#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>

class KVStore 
{
    public:

    /**
     * Set the value for the given key. 
     * @param[in] key is the key. 
     * @param[in] value is the value.
    */
    void set(const std::string& key, const std::string& value);

    /**
     * Getter api for value given the key. 
     * @param[in] key is the key we want to find the value for. 
     * @returns the value if present otherwise std::nullopt.
    */
    std::optional<std::string> get(const std::string& key) const;

    /**
     * Remove the given key. 
     * @param[in] key is the key we want to delete. 
     * @throws std::runtime_error if the key doesn't exist.
    */
    void remove(const std::string& key);

    /**
     * @returns the size of our store.
    */
    size_t size() const {
        return m_store.size();
    }

    private:

    std::unordered_map<std::string, std::string> m_store;
    mutable std::mutex m_mutex;
};