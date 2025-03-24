#include <RollingBuffer.h>
#include <unity.h>

void setUp() {
    // set stuff up here
}

void tearDown() {
    // clean stuff up here
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
void test_rolling_buffer_size() {
    static RollingBuffer<int, 4> rb;
    TEST_ASSERT_TRUE(rb.capacity() == 4);

    TEST_ASSERT_TRUE(rb.size() == 0);
    rb.push_back(10);
    TEST_ASSERT_TRUE(rb.size() == 1);

    rb.push_back(11);
    TEST_ASSERT_TRUE(rb.size() == 2);

    rb.push_back(12);
    TEST_ASSERT_TRUE(rb.size() == 3);

    rb.push_back(13);
    TEST_ASSERT_TRUE(rb.size() == 4);

    // the buffer is full, so size will no longer increase
    rb.push_back(14);
    TEST_ASSERT_TRUE(rb.size() == 4);

    rb.push_back(15);
    TEST_ASSERT_TRUE(rb.size() == 4);
    TEST_ASSERT_TRUE(rb.capacity() == 4);
}

void test_rolling_buffer_front_back() {
    static RollingBuffer<int, 4> rb;

    rb.push_back(10);
    TEST_ASSERT_TRUE(rb.front() == 10);
    TEST_ASSERT_TRUE(rb.back() == 10);

    rb.push_back(11);
    TEST_ASSERT_TRUE(rb.front() == 10);
    TEST_ASSERT_TRUE(rb.back() == 11);

    rb.push_back(12);
    TEST_ASSERT_TRUE(rb.front() == 10);
    TEST_ASSERT_TRUE(rb.back() == 12);

    rb.push_back(13);
    TEST_ASSERT_TRUE(rb.front() == 10);
    TEST_ASSERT_TRUE(rb.back() == 13);

    // now items start dropping off the front
    rb.push_back(14);
    TEST_ASSERT_TRUE(rb.front() == 11);
    TEST_ASSERT_TRUE(rb.back() == 14);

    rb.push_back(15);
    TEST_ASSERT_TRUE(rb.front() == 12);
    TEST_ASSERT_TRUE(rb.back() == 15);
}

void test_rolling_buffer_iteration() {
    static RollingBuffer<int, 4> rb;

    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_FALSE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(10);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(11);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_TRUE(*it == 11);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(12);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_TRUE(*it == 11);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 12);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(13);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_TRUE(*it == 11);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 12);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 13);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(14);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 11);
    ++it;
    TEST_ASSERT_TRUE(*it == 12);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 13);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 14);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(15);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 12);
    ++it;
    TEST_ASSERT_TRUE(*it == 13);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 14);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 15);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(16);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 13);
    ++it;
    TEST_ASSERT_TRUE(*it == 14);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 15);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 16);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(17);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 14);
    ++it;
    TEST_ASSERT_TRUE(*it == 15);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 16);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 17);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(18);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 15);
    ++it;
    TEST_ASSERT_TRUE(*it == 16);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 17);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 18);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.push_back(19);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 16);
    ++it;
    TEST_ASSERT_TRUE(*it == 17);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 18);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 19);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_rolling_buffer_size);
    RUN_TEST(test_rolling_buffer_front_back);
    RUN_TEST(test_rolling_buffer_iteration);

    UNITY_END();
}