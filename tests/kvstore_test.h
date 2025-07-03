#include <gtest/gtest.h>
#include <memory>

class KVStore;

class KVStoreTest : public testing::Test
{  
public:

    void SetUp() override;

    void TearDown() override;

    std::unique_ptr<KVStore> m_store;
};