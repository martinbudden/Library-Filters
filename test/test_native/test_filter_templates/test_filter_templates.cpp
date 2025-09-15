#include "FilterTemplates.h"
#include <unity.h>
#include <xyz_type.h>


void setUp() {
}

void tearDown() {
}

// NOLINTBEGIN(cppcoreguidelines-init-variables,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
void test_null_filter()
{
    FilterNullT<float> filter;
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));
}

void test_null_filter_xyz()
{
    FilterNullT<xyz_t> filter;
    xyz_t input {};
    xyz_t output {};

    input = {1.0, -1.0, 2.0 };
    output = filter.filter(input);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, output.x);
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, output.y);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, output.z);

    filter.reset();
    output = filter.filter({4.0F, 5.0F, 6.0F});
    TEST_ASSERT_EQUAL_FLOAT(4.0F, output.x);
    TEST_ASSERT_EQUAL_FLOAT(5.0F, output.y);
    TEST_ASSERT_EQUAL_FLOAT(6.0F, output.z);
}

void test_power_transfer_filter1()
{
    PowerTransferFilter1T<float> filter;

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.reset();
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.filter(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.filter(2.0F));

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
}

void test_power_transfer_filter1_xyz()
{
    PowerTransferFilter1T<xyz_t> filter;
    xyz_t output {};
    xyz_t state {};

    // test that filter with default settings performs no filtering
    output  = filter.filter({ 2.0F, 3.0F, 5.0F});
    TEST_ASSERT_EQUAL_FLOAT(2.0F, output.x);
    TEST_ASSERT_EQUAL_FLOAT(3.0F, output.y);
    TEST_ASSERT_EQUAL_FLOAT(5.0F, output.z);
    state  = filter.getState();
    TEST_ASSERT_EQUAL_FLOAT(2.0F, state.x);
    TEST_ASSERT_EQUAL_FLOAT(3.0F, state.y);
    TEST_ASSERT_EQUAL_FLOAT(5.0F, state.z);

    filter.reset();
    state  = filter.getState();
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.y);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.z);

    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.filter({1.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.filter({2.0F, 0.0F, 0.0F}).x);

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter({1.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter({2.0F, 0.0F, 0.0F}).x);

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.filter({1.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.filter({2.0F, 0.0F, 0.0F}).x);

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter({1.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter({2.0F, 0.0F, 0.0F}).x);
}

void test_biquad_filter()
{
    BiquadFilterT<float> filter;
    BiquadFilterT<float>::state_t state {};

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));
    state = filter.getState();
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, state.x1);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, state.x2);
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, state.y1);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, state.y2);

    filter.reset();
    state = filter.getState();
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.x1);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.x2);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.y1);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.y2);
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.setParameters(2.0F, 3.0F, 5.0F, 7.0F, 11.0F, 13.0F);
    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filterWeighted(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filterWeighted(2.0F));
}

void test_biquad_filter_xyz()
{
    BiquadFilterT<xyz_t> filter;
    xyz_t output {};
    BiquadFilterT<xyz_t>::state_t state {};

    // test that filter with default settings performs no filtering
    output  = filter.filter({ 2.0F, 3.0F, 5.0F});
    TEST_ASSERT_EQUAL_FLOAT(2.0F, output.x);
    TEST_ASSERT_EQUAL_FLOAT(3.0F, output.y);
    TEST_ASSERT_EQUAL_FLOAT(5.0F, output.z);
    state = filter.getState();
    TEST_ASSERT_EQUAL_FLOAT(2.0F, state.x1.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.x2.x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, state.y1.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.y2.x);

    filter.reset();
    state = filter.getState();
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.x1.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.x2.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.y1.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, state.y2.x);
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter({4.0F, 0.0F, 0.0F}).x);

    filter.setParameters(2.0F, 3.0F, 5.0F, 7.0F, 11.0F, 13.0F);
    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter({1.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter({2.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filterWeighted({1.0F, 0.0F, 0.0F}).x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filterWeighted({2.0F, 0.0F, 0.0F}).x);
}

// NOLINTEND(cppcoreguidelines-init-variables,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_null_filter);
    RUN_TEST(test_null_filter_xyz);
    RUN_TEST(test_power_transfer_filter1);
    RUN_TEST(test_power_transfer_filter1_xyz);
    RUN_TEST(test_biquad_filter);
    RUN_TEST(test_biquad_filter_xyz);

    UNITY_END();
}
