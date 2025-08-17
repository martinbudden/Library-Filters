#include "Filters.h"
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

class IIR_filter_test : public IIR_filter
{
public:
    virtual ~IIR_filter_test() = default;
    // class is not copyable or moveable
    IIR_filter_test(const IIR_filter_test&) = delete;
    IIR_filter_test& operator=(const IIR_filter_test&) = delete;
    IIR_filter_test(IIR_filter_test&&) = delete;
    IIR_filter_test& operator=(IIR_filter_test&&) = delete;

    explicit IIR_filter_test(float tau) : IIR_filter(tau) {}
    IIR_filter_test(float frequencyCutoff, float dT) : IIR_filter(frequencyCutoff, dT) {}

    float getOmega() { return _omega; }
    float calculateAlpha(float dT) { return _omega*dT/(_omega*dT + 1.0F); }
};

void test_IIR_filter()
{
    constexpr float dT = 5.0F / 1000.0F;
    {
    IIR_filter_test filter(0.0);
    TEST_ASSERT_EQUAL_FLOAT(0.0, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.0, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(0.0, filter.filter(10.0, dT));
    }
    {
    IIR_filter_test filter(100.0);
    TEST_ASSERT_EQUAL_FLOAT(628.3185, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.758547, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(7.58547, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.417005, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.859234, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.966012, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.991794, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.998018, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.37878, filter.filter(10.5, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.09146, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.40136, filter.filter(10.5, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.09691, filter.filter(10.0, dT));
    }

    {
    IIR_filter_test filter(100.0, dT);
    TEST_ASSERT_EQUAL_FLOAT(628.3185, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.758547, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(7.58547, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.417005, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.859234, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.966012, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.991794, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.998018, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(10.37878, filter.filter(10.5));
    TEST_ASSERT_EQUAL_FLOAT(10.09146, filter.filter(10.0));
    TEST_ASSERT_EQUAL_FLOAT(10.40136, filter.filter(10.5));
    TEST_ASSERT_EQUAL_FLOAT(10.09691, filter.filter(10.0));
    }
    {
    IIR_filter_test filter(10000.0F);
    TEST_ASSERT_EQUAL_FLOAT(62831.85, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.996827, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(9.96827, filter.filter(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.999899, filter.filter(10.0, dT));
    }
}

void test_IIR_alpha_filter()
{
    IIR_filter filter;
    filter.setAlpha(0.8F);
    float out = filter.filter(10);
    TEST_ASSERT_EQUAL_FLOAT(8.0, out);
    out = filter.filter(10);
    TEST_ASSERT_EQUAL_FLOAT(0.8*10.0 + 0.2*8.0, out);
    out = filter.filter(15);
    TEST_ASSERT_EQUAL_FLOAT(0.8*15.0 +0.2*(0.8*10.0 + 0.2*8.0), out);
}

void test_power_transfer_filter1()
{
    PowerTransferFilter1 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(0.0F, filter.getState());
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
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.2440311F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.735024F, filter.filter(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.2440311F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.735024F, filter.filter(2.0F));

    filter.setToPassthrough();
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
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.1682476F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.562592F, filter.filter(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.1682476, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.562592F, filter.filter(2.0F));

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
}

void test_iir_filter()
{
    IIR_filter filter; // NOLINT(cppcoreguidelines-init-variables)

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

void test_biquad_filter()
{
    BiquadFilter filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.filter(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.filter(4.0F));

    filter.setParameters(2.0F, 3.0F, 5.0F, 7.0F, 11.0F, 13.0F);
    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filter(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filter(2.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.filterWeighted(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.filterWeighted(2.0F));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_null_filter);
    RUN_TEST(test_moving_average_filter);
    RUN_TEST(test_IIR_filter);
    RUN_TEST(test_power_transfer_filter1);
    RUN_TEST(test_power_transfer_filter2);
    RUN_TEST(test_power_transfer_filter3);
    RUN_TEST(test_iir_filter);
    RUN_TEST(test_biquad_filter);

    UNITY_END();
}
