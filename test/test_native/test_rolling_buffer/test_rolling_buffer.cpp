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
    rb.pushBack(10);
    TEST_ASSERT_EQUAL(1, rb.size());

    rb.pushBack(11);
    TEST_ASSERT_EQUAL(2, rb.size());

    rb.pushBack(12);
    TEST_ASSERT_EQUAL(3, rb.size());

    rb.pushBack(13);
    TEST_ASSERT_EQUAL(4, rb.size());

    // the buffer is full, so size will no longer increase
    rb.pushBack(14);
    TEST_ASSERT_EQUAL(4, rb.size());

    rb.pushBack(15);
    TEST_ASSERT_EQUAL(4, rb.size());
    TEST_ASSERT_EQUAL(4, rb.capacity());
}

void test_rolling_buffer_front_back()
{
    static RollingBuffer<int, 4> rb;

    rb.pushBack(10);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(10, rb.back());

    rb.pushBack(11);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(11, rb.back());

    rb.pushBack(12);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(12, rb.back());

    rb.pushBack(13);
    TEST_ASSERT_EQUAL(10, rb.front());
    TEST_ASSERT_EQUAL(13, rb.back());

    // now items start dropping off the front
    rb.pushBack(14);
    TEST_ASSERT_EQUAL(11, rb.front());
    TEST_ASSERT_EQUAL(14, rb.back());

    rb.pushBack(15);
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

    rb.pushBack(10);
    {
    auto it = rb.begin();
    const auto end = rb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    rb.pushBack(11);
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

    rb.pushBack(12);
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

    rb.pushBack(13);
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

    rb.pushBack(14);
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

    rb.pushBack(15);
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

    rb.pushBack(16);
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

    rb.pushBack(17);
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

    rb.pushBack(18);
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

    rb.pushBack(19);
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

void test_rolling_buffer_copy()
{
    static RollingBuffer<int, 4> rb;
    static std::array<int, 4> buf;

    TEST_ASSERT_EQUAL(0, rb.getBegin());
    TEST_ASSERT_EQUAL(0, rb.getEnd());

    rb.pushBack(10);
    TEST_ASSERT_EQUAL(0, rb.getBegin());
    TEST_ASSERT_EQUAL(1, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);


    rb.pushBack(11);
    TEST_ASSERT_EQUAL(0, rb.getBegin());
    TEST_ASSERT_EQUAL(2, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);

    rb.pushBack(12);
    TEST_ASSERT_EQUAL(0, rb.getBegin());
    TEST_ASSERT_EQUAL(3, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);
    TEST_ASSERT_EQUAL(12, buf[2]);

    rb.pushBack(13);
    TEST_ASSERT_EQUAL(0, rb.getBegin());
    TEST_ASSERT_EQUAL(4, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);
    TEST_ASSERT_EQUAL(12, buf[2]);
    TEST_ASSERT_EQUAL(13, buf[3]);

    rb.pushBack(14);
    TEST_ASSERT_EQUAL(1, rb.getBegin());
    TEST_ASSERT_EQUAL(0, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(11, buf[0]);
    TEST_ASSERT_EQUAL(12, buf[1]);
    TEST_ASSERT_EQUAL(13, buf[2]);
    TEST_ASSERT_EQUAL(14, buf[3]);

    rb.pushBack(15);
    TEST_ASSERT_EQUAL(2, rb.getBegin());
    TEST_ASSERT_EQUAL(1, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(12, buf[0]);
    TEST_ASSERT_EQUAL(13, buf[1]);
    TEST_ASSERT_EQUAL(14, buf[2]);
    TEST_ASSERT_EQUAL(15, buf[3]);

    rb.pushBack(16);
    TEST_ASSERT_EQUAL(3, rb.getBegin());
    TEST_ASSERT_EQUAL(2, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(13, buf[0]);
    TEST_ASSERT_EQUAL(14, buf[1]);
    TEST_ASSERT_EQUAL(15, buf[2]);
    TEST_ASSERT_EQUAL(16, buf[3]);

    rb.pushBack(17);
    TEST_ASSERT_EQUAL(4, rb.getBegin());
    TEST_ASSERT_EQUAL(3, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(14, buf[0]);
    TEST_ASSERT_EQUAL(15, buf[1]);
    TEST_ASSERT_EQUAL(16, buf[2]);
    TEST_ASSERT_EQUAL(17, buf[3]);

    rb.pushBack(18);
    TEST_ASSERT_EQUAL(0, rb.getBegin());
    TEST_ASSERT_EQUAL(4, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(15, buf[0]);
    TEST_ASSERT_EQUAL(16, buf[1]);
    TEST_ASSERT_EQUAL(17, buf[2]);
    TEST_ASSERT_EQUAL(18, buf[3]);

    rb.pushBack(19);
    TEST_ASSERT_EQUAL(1, rb.getBegin());
    TEST_ASSERT_EQUAL(0, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(16, buf[0]);
    TEST_ASSERT_EQUAL(17, buf[1]);
    TEST_ASSERT_EQUAL(18, buf[2]);
    TEST_ASSERT_EQUAL(19, buf[3]);

    rb.pushBack(20);
    TEST_ASSERT_EQUAL(17, rb[0]);
    TEST_ASSERT_EQUAL(18, rb[1]);
    TEST_ASSERT_EQUAL(19, rb[2]);
    TEST_ASSERT_EQUAL(20, rb[3]);
    TEST_ASSERT_EQUAL(2, rb.getBegin());
    TEST_ASSERT_EQUAL(1, rb.getEnd());
    buf.fill(-1);
    rb.copy(buf);
    TEST_ASSERT_EQUAL(17, buf[0]);
    TEST_ASSERT_EQUAL(18, buf[1]);
    TEST_ASSERT_EQUAL(19, buf[2]);
    TEST_ASSERT_EQUAL(20, buf[3]);
}

void test_rolling_buffer_sum()
{
    static RollingBufferWithSum<int, 4> rb;

    rb.pushBack(10);
    TEST_ASSERT_EQUAL(10, rb.sum());
    TEST_ASSERT_EQUAL(10, rb.recalculateSum());
    TEST_ASSERT_EQUAL(10, rb.sum());

    rb.pushBack(11);
    TEST_ASSERT_EQUAL(21, rb.sum());
    TEST_ASSERT_EQUAL(21, rb.recalculateSum());
    TEST_ASSERT_EQUAL(21, rb.sum());

    rb.pushBack(12);
    TEST_ASSERT_EQUAL(33, rb.sum());
    TEST_ASSERT_EQUAL(33, rb.recalculateSum());
    TEST_ASSERT_EQUAL(33, rb.sum());

    rb.pushBack(14);
    TEST_ASSERT_EQUAL(47, rb.sum());
    TEST_ASSERT_EQUAL(47, rb.recalculateSum());
    TEST_ASSERT_EQUAL(47, rb.sum());

    rb.pushBack(15);
    TEST_ASSERT_EQUAL(52, rb.sum());
    TEST_ASSERT_EQUAL(52, rb.recalculateSum());
    TEST_ASSERT_EQUAL(52, rb.sum());

    rb.pushBack(16);
    TEST_ASSERT_EQUAL(57, rb.sum());

    rb.pushBack(17);
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
    RUN_TEST(test_rolling_buffer_copy);
    RUN_TEST(test_rolling_buffer_sum);

    UNITY_END();
}