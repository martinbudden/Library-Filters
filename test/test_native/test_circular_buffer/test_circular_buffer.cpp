#include <CircularBuffer.h>
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
void test_circular_buffer_size()
{
    static CircularBuffer<int, 4> cb;
    TEST_ASSERT_EQUAL(4, cb.capacity());

    TEST_ASSERT_EQUAL(0, cb.size());
    bool pushed = cb.pushBack(10);
    TEST_ASSERT_TRUE(pushed);
    TEST_ASSERT_EQUAL(1, cb.size());

    pushed = cb.pushBack(11);
    TEST_ASSERT_TRUE(pushed);
    TEST_ASSERT_EQUAL(2, cb.size());

    pushed = cb.pushBack(12);
    TEST_ASSERT_TRUE(pushed);
    TEST_ASSERT_EQUAL(3, cb.size());

    pushed = cb.pushBack(13);
    TEST_ASSERT_TRUE(pushed);
    TEST_ASSERT_EQUAL(4, cb.size());

    // the buffer is full, so push will fail
    pushed = cb.pushBack(14);
    TEST_ASSERT_FALSE(pushed);
    TEST_ASSERT_EQUAL(4, cb.size());

    pushed = cb.pushBack(15);
    TEST_ASSERT_FALSE(pushed);
    TEST_ASSERT_EQUAL(4, cb.size());
    TEST_ASSERT_EQUAL(4, cb.capacity());
}

void test_circular_buffer_front_back()
{
    static CircularBuffer<int, 4> cb;

    cb.pushBack(10);
    TEST_ASSERT_EQUAL(10, cb.front());
    TEST_ASSERT_EQUAL(10, cb.back());

    cb.pushBack(11);
    TEST_ASSERT_EQUAL(10, cb.front());
    TEST_ASSERT_EQUAL(11, cb.back());

    cb.pushBack(12);
    TEST_ASSERT_EQUAL(10, cb.front());
    TEST_ASSERT_EQUAL(12, cb.back());

    cb.pushBack(13);
    TEST_ASSERT_EQUAL(10, cb.front());
    TEST_ASSERT_EQUAL(13, cb.back());

    // now buffer is full
    bool pushed = cb.pushBack(14);
    TEST_ASSERT_FALSE(pushed);
    TEST_ASSERT_EQUAL(10, cb.front());
    TEST_ASSERT_EQUAL(13, cb.back());

    pushed = cb.pushBack(15);
    TEST_ASSERT_FALSE(pushed);
    TEST_ASSERT_EQUAL(10, cb.front());
    TEST_ASSERT_EQUAL(13, cb.back());
}

void test_circular_buffer_iteration()
{
    static CircularBuffer<int, 4> cb;

    {
    auto it = cb.begin();
    const auto end = cb.end();
    TEST_ASSERT_FALSE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    cb.pushBack(10);
    {
    auto it = cb.begin();
    const auto end = cb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    cb.pushBack(11);
    {
    auto it = cb.begin();
    const auto end = cb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 10);
    ++it;
    TEST_ASSERT_TRUE(*it == 11);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    cb.pushBack(12);
    {
    auto it = cb.begin();
    const auto end = cb.end();
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

    cb.pushBack(13);
    {
    auto it = cb.begin();
    const auto end = cb.end();
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

    cb.pushBack(14);
    {
    auto it = cb.begin();
    const auto end = cb.end();
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

    bool pushed = cb.pushBack(15);
    TEST_ASSERT_FALSE(pushed);
    {
    auto it = cb.begin();
    const auto end = cb.end();
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

    pushed = cb.pushBack(16);
    TEST_ASSERT_FALSE(pushed);
    {
    auto it = cb.begin();
    const auto end = cb.end();
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

    int front {};
    bool popped = cb.popFront(front);
    TEST_ASSERT_TRUE(popped);
    TEST_ASSERT_EQUAL(10, front);

    cb.pushBack(17);
    {
    auto it = cb.begin();
    const auto end = cb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 11);
    ++it;
    TEST_ASSERT_TRUE(*it == 12);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 13);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_TRUE(*it == 17);
    TEST_ASSERT_TRUE(it != end);
    ++it;
    TEST_ASSERT_FALSE(it != end);
    }

    popped = cb.popFront(front);
    TEST_ASSERT_TRUE(popped);
    TEST_ASSERT_EQUAL(11, front);
    cb.pushBack(18);
    {
    auto it = cb.begin();
    const auto end = cb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 12);
    ++it;
    TEST_ASSERT_TRUE(*it == 13);
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

    popped = cb.popFront(front);
    TEST_ASSERT_TRUE(popped);
    TEST_ASSERT_EQUAL(12, front);
    cb.pushBack(19);
    {
    auto it = cb.begin();
    const auto end = cb.end();
    TEST_ASSERT_TRUE(it != end);
    TEST_ASSERT_TRUE(*it == 13);
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

void test_circular_buffer_copy()
{
    static CircularBuffer<int, 4> cb;
    static std::array<int, 4> buf;

    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(0, cb.getEnd());

    cb.pushBack(10);
    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(1, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);


    cb.pushBack(11);
    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(2, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);

    cb.pushBack(12);
    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(3, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);
    TEST_ASSERT_EQUAL(12, buf[2]);

    cb.pushBack(13);
    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(4, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);
    TEST_ASSERT_EQUAL(12, buf[2]);
    TEST_ASSERT_EQUAL(13, buf[3]);

    bool pushed = cb.pushBack(14);
    TEST_ASSERT_FALSE(pushed);
    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(4, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(10, buf[0]);
    TEST_ASSERT_EQUAL(11, buf[1]);
    TEST_ASSERT_EQUAL(12, buf[2]);
    TEST_ASSERT_EQUAL(13, buf[3]);

    int popped {};
    cb.popFront(popped);
    TEST_ASSERT_EQUAL(10, popped);
    cb.pushBack(15);
    TEST_ASSERT_EQUAL(1, cb.getBegin());
    TEST_ASSERT_EQUAL(0, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(11, buf[0]);
    TEST_ASSERT_EQUAL(12, buf[1]);
    TEST_ASSERT_EQUAL(13, buf[2]);
    TEST_ASSERT_EQUAL(15, buf[3]);

    cb.popFront(popped);
    TEST_ASSERT_EQUAL(11, popped);
    cb.pushBack(16);
    TEST_ASSERT_EQUAL(2, cb.getBegin());
    TEST_ASSERT_EQUAL(1, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(12, buf[0]);
    TEST_ASSERT_EQUAL(13, buf[1]);
    TEST_ASSERT_EQUAL(15, buf[2]);
    TEST_ASSERT_EQUAL(16, buf[3]);

    cb.popFront(popped);
    TEST_ASSERT_EQUAL(12, popped);
    cb.pushBack(17);
    TEST_ASSERT_EQUAL(3, cb.getBegin());
    TEST_ASSERT_EQUAL(2, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(13, buf[0]);
    TEST_ASSERT_EQUAL(15, buf[1]);
    TEST_ASSERT_EQUAL(16, buf[2]);
    TEST_ASSERT_EQUAL(17, buf[3]);

    cb.popFront(popped);
    TEST_ASSERT_EQUAL(13, popped);
    cb.pushBack(18);
    TEST_ASSERT_EQUAL(4, cb.getBegin());
    TEST_ASSERT_EQUAL(3, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(15, buf[0]);
    TEST_ASSERT_EQUAL(16, buf[1]);
    TEST_ASSERT_EQUAL(17, buf[2]);
    TEST_ASSERT_EQUAL(18, buf[3]);

    cb.popFront(popped);
    TEST_ASSERT_EQUAL(15, popped);
    cb.pushBack(19);
    TEST_ASSERT_EQUAL(0, cb.getBegin());
    TEST_ASSERT_EQUAL(4, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(16, buf[0]);
    TEST_ASSERT_EQUAL(17, buf[1]);
    TEST_ASSERT_EQUAL(18, buf[2]);
    TEST_ASSERT_EQUAL(19, buf[3]);

    cb.popFront(popped);
    TEST_ASSERT_EQUAL(16, popped);
    cb.pushBack(20);
    TEST_ASSERT_EQUAL(17, cb[0]);
    TEST_ASSERT_EQUAL(18, cb[1]);
    TEST_ASSERT_EQUAL(19, cb[2]);
    TEST_ASSERT_EQUAL(20, cb[3]);
    TEST_ASSERT_EQUAL(1, cb.getBegin());
    TEST_ASSERT_EQUAL(0, cb.getEnd());
    buf.fill(-1);
    cb.copy(buf);
    TEST_ASSERT_EQUAL(17, buf[0]);
    TEST_ASSERT_EQUAL(18, buf[1]);
    TEST_ASSERT_EQUAL(19, buf[2]);
    TEST_ASSERT_EQUAL(20, buf[3]);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_circular_buffer_size);
    RUN_TEST(test_circular_buffer_front_back);
    RUN_TEST(test_circular_buffer_iteration);
    RUN_TEST(test_circular_buffer_copy);

    UNITY_END();
}