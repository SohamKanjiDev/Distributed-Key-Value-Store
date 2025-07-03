#include "kvstore_test.h"
#include "kvstore.h"
#include <string>
#include <filesystem>

static constexpr char* TEST_DB = "test_db";

void KVStoreTest::SetUp() 
{
  m_store = std::make_unique<KVStore>(TEST_DB);
}

void KVStoreTest::TearDown() 
{
  m_store.reset();
  std::filesystem::remove(TEST_DB);
}

TEST_F(KVStoreTest, setAndGetKeyTest)
{
  const std::string key = "key";
  const std::string value = "value";
  m_store->set(key, value);
  auto result = m_store->get(key);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value(), value);
}

TEST_F(KVStoreTest, getUnknownKeyTest)
{
  const std::string key = "key";
  auto result = m_store->get(key);
  ASSERT_TRUE(!result.has_value());
}

TEST_F(KVStoreTest, deleteKeyTest)
{
  const std::string key = "key";
  const std::string value = "value";
  m_store->set(key, value);
  m_store->remove(key);
  auto result = m_store->get(key);
  ASSERT_TRUE(!result.has_value());
  EXPECT_THROW(m_store->remove(key), std::runtime_error);
}

TEST_F(KVStoreTest, persistenceTest)
{
  const std::string key = "key";
  const std::string value = "value";
  m_store->set(key, value);
  m_store->updateDB(TEST_DB);
  m_store = std::make_unique<KVStore>(TEST_DB);
  auto result = m_store->get(key);
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value(), value);
}

