#include <RollingBuffer.h>
#include <unity.h>

void setUp()
{
    // set stuff up here
}

void tearDown()
{
    // clean stuff up here
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
void test_rolling_buffer_size()
{
    static RollingBuffer<int, 4> rb;
    TEST_ASSERT_EQUAL(4, rb.capacity());

    TEST_ASSERT_EQUAL(0, rb.size());
    rb.push_back(10);
    TEST_ASSERT_EQUAL(1, rb.size());

    rb.push_back(11);
    TEST_ASSERT_EQUAL(2, rb.size());

    rb.push_back(12);
    TEST_ASSERT_EQUAL(3, rb.size());

    rb.push_back(13);
    TEST_ASSERT_EQUAL(4, rb.size());

    // the buffer is full, so size will no longer increase
    rb.push_back(14);
    TEST_ASSERT_EQUAL(4, rb.size());

    rb.push_back(15);
    TEST_ASSERT_EQUAL(4, rb.size());
    TEST_ASSERT_EQUAL(4, rb.capacity());
}

void test_rolling_buffer_front_back()
{
    static RollingBuffer<int, 4> rb;

    rb.push_back(10);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(10, rb.back());

    rb.push_back(11);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(11, rb.back());

    rb.push_back(12);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(12, rb.back());

    rb.push_back(13);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(13, rb.back());

    // now items start dropping off the front
    rb.push_back(14);
    TEST_ASSERT_EQUAL(11, rb.front());
    TEST_ASSERT_EQUAL(14, rb.back());

    rb.push_back(15);
    TEST_ASSERT_EQUAL(12, rb.front());
    TEST_ASSERT_EQUAL(15, rb.back());
}

void test_rolling_buffer_iteration()
{
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

void test_rolling_buffer_sum()
{
    static RollingBufferWithSum<int, 4> rb;

    rb.push_back(10);
    TEST_ASSERT_EQUAL(10, rb.sum());
    TEST_ASSERT_EQUAL(10, rb.recalculateSum());
    TEST_ASSERT_EQUAL(10, rb.sum());

    rb.push_back(11);
    TEST_ASSERT_EQUAL(21, rb.sum());
    TEST_ASSERT_EQUAL(21, rb.recalculateSum());
    TEST_ASSERT_EQUAL(21, rb.sum());

    rb.push_back(12);
    TEST_ASSERT_EQUAL(33, rb.sum());
    TEST_ASSERT_EQUAL(33, rb.recalculateSum());
    TEST_ASSERT_EQUAL(33, rb.sum());

    rb.push_back(14);
    TEST_ASSERT_EQUAL(47, rb.sum());
    TEST_ASSERT_EQUAL(47, rb.recalculateSum());
    TEST_ASSERT_EQUAL(47, rb.sum());

    rb.push_back(15);
    TEST_ASSERT_EQUAL(52, rb.sum());
    TEST_ASSERT_EQUAL(52, rb.recalculateSum());
    TEST_ASSERT_EQUAL(52, rb.sum());

    rb.push_back(16);
    TEST_ASSERT_EQUAL(57, rb.sum());

    rb.push_back(17);
    TEST_ASSERT_EQUAL(62, rb.sum());
    TEST_ASSERT_EQUAL(62, rb.recalculateSum());
    TEST_ASSERT_EQUAL(62, rb.sum());
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_rolling_buffer_size);
    RUN_TEST(test_rolling_buffer_front_back);
    RUN_TEST(test_rolling_buffer_iteration);
    RUN_TEST(test_rolling_buffer_sum);

    UNITY_END();
}