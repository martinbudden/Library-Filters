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
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.update(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(4.0F));
}

void test_moving_average_filter()
{
    FilterMovingAverage<3> filter;
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.5F, filter.update(2.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(3.0F));
    TEST_ASSERT_EQUAL_FLOAT(3.0F, filter.update(4.0F));
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(5.0F));
    TEST_ASSERT_EQUAL_FLOAT(5.0F, filter.update(6.0F));
    TEST_ASSERT_EQUAL_FLOAT(7.0F, filter.update(10.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(4.0F));
    TEST_ASSERT_EQUAL_FLOAT(12.0F, filter.update(20.0F));
    TEST_ASSERT_EQUAL_FLOAT(5.0F, filter.update(-9.0F));
}

class IIR_filter_test : public IIR_filter
{
public:
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
    TEST_ASSERT_EQUAL_FLOAT(0.0, filter.update(10.0, dT));
    }
    {
    IIR_filter_test filter(100.0);
    TEST_ASSERT_EQUAL_FLOAT(628.3185, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.758547, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(7.58547, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.417005, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.859234, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.966012, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.991794, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.998018, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.37878, filter.update(10.5, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.09146, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.40136, filter.update(10.5, dT));
    TEST_ASSERT_EQUAL_FLOAT(10.09691, filter.update(10.0, dT));
    }

    {
    IIR_filter_test filter(100.0, dT);
    TEST_ASSERT_EQUAL_FLOAT(628.3185, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.758547, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(7.58547, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.417005, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.859234, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.966012, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.991794, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(9.998018, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(10.37878, filter.update(10.5));
    TEST_ASSERT_EQUAL_FLOAT(10.09146, filter.update(10.0));
    TEST_ASSERT_EQUAL_FLOAT(10.40136, filter.update(10.5));
    TEST_ASSERT_EQUAL_FLOAT(10.09691, filter.update(10.0));
    }
    {
    IIR_filter_test filter(10000.0F);
    TEST_ASSERT_EQUAL_FLOAT(62831.85, filter.getOmega());
    TEST_ASSERT_EQUAL_FLOAT(0.996827, filter.calculateAlpha(dT));
    TEST_ASSERT_EQUAL_FLOAT(9.96827, filter.update(10.0, dT));
    TEST_ASSERT_EQUAL_FLOAT(9.999899, filter.update(10.0, dT));
    }
}

void test_IIR_alpha_filter()
{
    IIR_filter filter;
    filter.setAlpha(0.8F);
    float out = filter.update(10);
    TEST_ASSERT_EQUAL_FLOAT(8.0, out);
    out = filter.update(10);
    TEST_ASSERT_EQUAL_FLOAT(0.8*10.0 + 0.2*8.0, out);
    out = filter.update(15);
    TEST_ASSERT_EQUAL_FLOAT(0.8*15.0 +0.2*(0.8*10.0 + 0.2*8.0), out);
}

void test_power_transfer_filter1()
{
    PowerTransferFilter1 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.update(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(4.0F));

    filter.reset();
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.update(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.update(2.0F));

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));
}

void test_power_transfer_filter2()
{
    PowerTransferFilter2 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.update(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(4.0F));

    filter.reset();
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.2440311F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.735024F, filter.update(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.2440311F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.735024F, filter.update(2.0F));

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));
}

void test_power_transfer_filter3()
{
    PowerTransferFilter3 filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.update(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(4.0F));

    filter.reset();
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.1682476F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.562592F, filter.update(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.1682476, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(0.562592F, filter.update(2.0F));

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));
}

void test_iir_filter()
{
    IIR_filter filter; // NOLINT(cppcoreguidelines-init-variables)

    // test that filter with default settings performs no filtering
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(-1.0F, filter.update(-1.0F));

    filter.reset();
    TEST_ASSERT_EQUAL_FLOAT(4.0F, filter.update(4.0F));

    filter.reset();
    filter.setCutoffFrequency(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.update(2.0F));

    filter.init(1.0F);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));

    filter.setCutoffFrequencyAndReset(100.0F, 0.001F);
    TEST_ASSERT_EQUAL_FLOAT(0.3858696F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(1.008713F, filter.update(2.0F));

    filter.setToPassthrough();
    TEST_ASSERT_EQUAL_FLOAT(1.0F, filter.update(1.0F));
    TEST_ASSERT_EQUAL_FLOAT(2.0F, filter.update(2.0F));
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

    UNITY_END();
}
