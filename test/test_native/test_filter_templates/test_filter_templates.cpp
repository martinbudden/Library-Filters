#include "FilterTemplates.h"
#include <unity.h>
#include <xyz_type.h>


void setUp() {
}

void tearDown() {
}

// NOLINTBEGIN(cppcoreguidelines-init-variables,cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
void test_null_filter_float()
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

void test_moving_average_filter_float()
{
    FilterMovingAverageT<float, 3> filter;
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

void test_moving_average_filter_xyz()
{
    FilterMovingAverageT<xyz_t, 4> filter;
    xyz_t m = filter.filter(xyz_t{1.0F, 0.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(1.0F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-3.0F, m.z);

    m = filter.filter(xyz_t{2.0F, 0.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(1.5F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-3.0F, m.z);

    m = filter.filter(xyz_t{3.0F, 3.0F, 0.0F});
    TEST_ASSERT_EQUAL_FLOAT(2.0F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-2.0F, m.z);

    m = filter.filter(xyz_t{4.0F, 2.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(2.5F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(1.25F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-2.25F, m.z);

    m = filter.filter(xyz_t{5.0F, 2.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(3.5F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(1.75F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-2.25F, m.z);

    m = filter.filter(xyz_t{6.0F, 2.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(4.5F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(2.25F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-2.25F, m.z);

    m = filter.filter(xyz_t{10.0F, 2.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(6.25F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-3.0F, m.z);

    filter.reset();
    m = filter.filter(xyz_t{4.0F, 2.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(4.0F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-3.0F, m.z);

    m = filter.filter(xyz_t{20.0F, 0.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(12.0F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-3.0F, m.z);


    m = filter.filter(xyz_t{-9.0F, 0.0F, -3.0F});
    TEST_ASSERT_EQUAL_FLOAT(5.0F, m.x);
    TEST_ASSERT_EQUAL_FLOAT(2.0F / 3.0F, m.y);
    TEST_ASSERT_EQUAL_FLOAT(-3.0F, m.z);

}

void test_power_transfer_filter1_float()
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

void test_biquad_filter_float()
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

    RUN_TEST(test_null_filter_float);
    RUN_TEST(test_null_filter_xyz);
    RUN_TEST(test_moving_average_filter_float);
    RUN_TEST(test_moving_average_filter_xyz);
    RUN_TEST(test_power_transfer_filter1_float);
    RUN_TEST(test_power_transfer_filter1_xyz);
    RUN_TEST(test_biquad_filter_float);
    RUN_TEST(test_biquad_filter_xyz);

    UNITY_END();
}
