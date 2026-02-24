#include "filters.h"
#include <unity.h>

void setUp() {
}

void tearDown() {
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
void test_null_filter()
{
    FilterNull filter; // NOLINT(cppcoreguidelines-init-variables)
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));
}

void test_moving_average_filter()
{
    FilterMovingAverage<3> filter;
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.5F, filter.filter(2.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(3.0F));
    TEST_ASSERT_EQUAL_FLOAT(3.0F, filter.filter(4.0F));
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(5.0F));
    TEST_ASSERT_EQUAL_FLOAT(5.0F, filter.filter(6.0F));
    TEST_ASSERT_EQUAL_FLOAT(7.0F, filter.filter(10.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));
    TEST_ASSERT_EQUAL_FLOAT(12.0F, filter.filter(20.0F));
    TEST_ASSERT_EQUAL_FLOAT(5.0F, filter.filter(-9.0F));
}

void test_power_transfer_filter1()
{
    PowerTransferFilter1 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(0.0F, filter.get_state());
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.reset();
    filter.set_cutoff_frequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.filter(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));

    filter.set_cutoff_frequency_and_reset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.filter(2.0F));

    filter.set_to_passthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
}

void test_power_transfer_filter2()
{
    PowerTransferFilter2 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.reset();
    filter.set_cutoff_frequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.2440311F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.735024F, filter.filter(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));

    filter.set_cutoff_frequency_and_reset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.2440311F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.735024F, filter.filter(2.0F));

    filter.set_to_passthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
}

void test_power_transfer_filter3()
{
    PowerTransferFilter3 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.reset();
    filter.set_cutoff_frequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.1682476F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.562592F, filter.filter(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));

    filter.set_cutoff_frequency_and_reset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.1682476, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.562592F, filter.filter(2.0F));

    filter.set_to_passthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
}

void test_biquad_filter()
{
    BiquadFilter filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.set_parameters(2.0F, 3.0F, 5.0F, 7.0F, 11.0F, 13.0F);
    filter.set_to_passthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter_weighted(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter_weighted(2.0F));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_null_filter);
    RUN_TEST(test_moving_average_filter);
    RUN_TEST(test_power_transfer_filter1);
    RUN_TEST(test_power_transfer_filter2);
    RUN_TEST(test_power_transfer_filter3);
    RUN_TEST(test_biquad_filter);

    UNITY_END();
}
