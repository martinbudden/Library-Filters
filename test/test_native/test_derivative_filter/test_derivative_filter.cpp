#include <derivative_filter_templates.h>
#include <derivative_filters.h>
#include <filters.h>
#include <unity.h>
#include <xy_type.h>


void setUp()
{
    // set stuff up here
}

void tearDown()
{
    // clean stuff up here
}

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
void test_derivative_filter_two_point()
{
    DerivativeFilter2point filter;
    filter.push_back(0.0F, 0.0F);
    filter.push_back(2.0F, 1.0F);
    float dx_dt = filter.derivative();

    TEST_ASSERT_EQUAL_FLOAT(2.0, dx_dt);

    filter.push_back(0.0F, 0.0F);
    filter.push_back(1.0F, 1.0F);
    filter.push_back(4.0F, 2.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(3.0F, dx_dt);

    filter.push_back(9.0F, 3.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(5.0F, dx_dt);

    filter.push_back(16.0F, 4.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(7.0F, dx_dt);
}

void test_derivative_filter_two_point_template()
{
    DerivativeFilter2pointT<float> filter;
    filter.push_back(0.0F, 0.0F);
    filter.push_back(2.0F, 1.0F);
    float dx_dt = filter.derivative();

    TEST_ASSERT_EQUAL_FLOAT(2.0, dx_dt);

    filter.push_back(0.0F, 0.0F);
    filter.push_back(1.0F, 1.0F);
    filter.push_back(4.0F, 2.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(3.0F, dx_dt);

    filter.push_back(9.0F, 3.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(5.0F, dx_dt);

    filter.push_back(16.0F, 4.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(7.0F, dx_dt);
}

void test_quadratic_solve()
{
    // y = x*x + 2*x + 3
    // dy/dx = 2*x + 2;
    auto p0 = xy_t{1.0F, 6.0F};
    auto p1 = xy_t{2.0F, 11.0F};
    auto p2 = xy_t{3.0F, 18.0F};

    const float x1 = p1.x - p0.x;
    const float y1 = p1.y - p0.y;
    const float x2 = p2.x - p0.x;
    const float y2 = p2.y - p0.y;

    const float det = x1*(x1*x2 - x2*x2);
    const float dy_dx2e = (y1*x2*x2 + y2*(x1*x1 - 2.0F*x1*x2))/det;
    TEST_ASSERT_EQUAL_FLOAT(8.0, dy_dx2e);


    const float a = (y1*x2 - y2*x1)/det;
    const float b = (x1*x1*y2 -x2*x2*y1)/det;
    const float globalB = b - 2.0F*a*p0.x;
    const float c = 3.0;

    const float dy_dx2a = (2.0F*(y1*x2 - y2*x1)*(p2.x - p0.x) + (x1*x1*y2 -x2*x2*y1))/det;
    TEST_ASSERT_EQUAL_FLOAT(8.0, dy_dx2a);

    const float dy_dx2b = (2.0F*(y1*x2 - y2*x1)*x2 + (x1*x1*y2 -x2*x2*y1))/det;
    TEST_ASSERT_EQUAL_FLOAT(8.0, dy_dx2b);
    const float dy_dx2c = (2.0F*y1*x2*x2 - 2.0F*y2*x1*x2 + x1*x1*y2 -x2*x2*y1)/det;
    TEST_ASSERT_EQUAL_FLOAT(8.0, dy_dx2c);
    const float dy_dx2d = (y1*x2*x2 - 2.0F*y2*x1*x2 + x1*x1*y2)/det;
    TEST_ASSERT_EQUAL_FLOAT(8.0, dy_dx2d);


    TEST_ASSERT_EQUAL_FLOAT(1.0F, a);
    TEST_ASSERT_EQUAL_FLOAT(2.0F, globalB);

    TEST_ASSERT_EQUAL_FLOAT(p0.y, a*p0.x*p0.x + globalB*p0.x + c);
    TEST_ASSERT_EQUAL_FLOAT(p1.y, a*p1.x*p1.x + globalB*p1.x + c);
    TEST_ASSERT_EQUAL_FLOAT(p2.y, a*p2.x*p2.x + globalB*p2.x + c);

    const float dy_dx0 = 2.0F*a*p0.x + globalB;
    TEST_ASSERT_EQUAL_FLOAT(4.0, dy_dx0);

    const float dy_dx1 = 2.0F*a*p1.x + globalB;
    TEST_ASSERT_EQUAL_FLOAT(6.0, dy_dx1);

    const float dy_dx2 = 2.0F*a*p2.x + globalB;
    TEST_ASSERT_EQUAL_FLOAT(8.0, dy_dx2);
}

void test_derivative_filter_three_point_a()
{
    // y = x*x + 2*x + 3
    // dy/dx = 2*x + 2;
    DerivativeFilter3point filter;
    filter.push_back(6.0F, 1.0F);
    filter.push_back(11.0F, 2.0F);
    filter.push_back(18.0F, 3.0F);
    const float dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(8.0F, dx_dt);
}

void test_derivative_filter_three_point_template_a()
{
    // y = x*x + 2*x + 3
    // dy/dx = 2*x + 2;
    DerivativeFilter3pointT<float> filter;
    filter.push_back(6.0F, 1.0F);
    filter.push_back(11.0F, 2.0F);
    filter.push_back(18.0F, 3.0F);
    float dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(8.0F, dx_dt);
}

void test_derivative_filter_three_point_b()
{
    // y = x*x + 2*x + 3
    // dy/dx = 2*x + 2;
    DerivativeFilter3point filter;
    filter.push_back(6.0F, 1.0F);
    filter.push_back(11.0F, 2.0F);
    const float dx_dt = filter.filter(18.0F, 3.0F);
    TEST_ASSERT_EQUAL_FLOAT(8.0F, dx_dt);
}

void test_derivative_filter_three_point_c()
{
    // y = x*x + 2*x + 3
    // dy/dx = 2*x + 2;
    DerivativeFilter3point filter;
    filter.push_back(6.0F, 1.0F);
    filter.push_back(27.0F, 4.0F);
    const float dx_dt = filter.filter(102.0F, 9.0F);
    TEST_ASSERT_EQUAL_FLOAT(20.0F, dx_dt);
}

void test_derivative_filter_three_point_d()
{
    // y = x*x + 2*x + 3
    // dy/dx = 2*x + 2;
    DerivativeFilter3point filter;
    filter.push_back(0.0F, 1.0F);
    filter.push_back(1.0F, 2.0F);
    filter.push_back(2.0F, 3.0F);
    //filter.fill(1.0F);

    float dx_dt = filter.filter(6.0F, 1.0F);
    TEST_ASSERT_EQUAL_FLOAT(-8.0F, dx_dt);

    dx_dt = filter.filter(27.0F, 4.0F);
    TEST_ASSERT_EQUAL_FLOAT(34.0F, dx_dt);

    dx_dt = filter.filter(102.0F, 9.0F);
    TEST_ASSERT_EQUAL_FLOAT(20.0F, dx_dt);

    dx_dt = filter.filter(123.0F, 10.0F);
    TEST_ASSERT_EQUAL_FLOAT(22.0F, dx_dt);
}

void test_derivative_filter_three_point_e()
{
    // x = t*t
    // dx/dt = 2*t
    DerivativeFilter3point filter;
    filter.push_back(0.0F, 0.0F);
    filter.push_back(2.0F, 1.0F);
    filter.push_back(4.0F, 2.0F);
    const float dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(2.0F, dx_dt);
}

void test_derivative_filter_step()
{
    {DerivativeFilter3point32 filter;
    uint32_t t = 0;
    filter.push_back(0.0F, t++);
    filter.push_back(0.0F, t++);

    float dx_dt = filter.filter(1.0F, t++);
    TEST_ASSERT_EQUAL_FLOAT(1.5F, dx_dt);

    dx_dt = filter.filter(1.0F, t++);
    TEST_ASSERT_EQUAL_FLOAT(-0.5F, dx_dt);

    dx_dt = filter.filter(2.0F, t++);
    TEST_ASSERT_EQUAL_FLOAT(1.5F, dx_dt);

    dx_dt = filter.filter(2.0F, t++);
    TEST_ASSERT_EQUAL_FLOAT(-0.5F, dx_dt);

    dx_dt = filter.filter(3.0F, t++);
    TEST_ASSERT_EQUAL_FLOAT(1.5F, dx_dt);

    dx_dt = filter.filter(3.0F, t++);
    TEST_ASSERT_EQUAL_FLOAT(-0.5F, dx_dt);}

    {DerivativeFilter2point32 filter;
    // exponentially weighted moving average filter
    PowerTransferFilter1 ewma(0.5F);
    uint32_t t = 0;
    filter.push_back(0.0F, t++);

    float dx_dt = filter.filter(1.0F, t++);
    float f = ewma.filter(dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.5F, f);

    dx_dt = filter.filter(1.0F, t++);
    f = ewma.filter(dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.25, f);

    dx_dt = filter.filter(2.0F, t++);
    f = ewma.filter(dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.625, f);

    dx_dt = filter.filter(2.0F, t++);
    f = ewma.filter(dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.3125, f);

    dx_dt = filter.filter(3.0F, t++);
    f = ewma.filter(dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(1.0F, dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.65625, f);

    dx_dt = filter.filter(3.0F, t++);
    f = ewma.filter(dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.0F, dx_dt);
    TEST_ASSERT_EQUAL_FLOAT(0.328125F, f);}
}
#if false
    filter.push_back(0.0F, 0.0F);
    filter.push_back(1.0F, 1.0F);
    filter.push_back(4.0F, 2.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(5.5F, dx_dt);

    // x = t^2
    // dx/dt = 2t
    // t     0 1 2 3  4  5
    // x     0 1 4 9 16 25
    // dx/dt 0 2 4 6  8 10
    filter.push_back(9.0F, 3.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(8.5F, dx_dt);

    filter.push_back(16.0F, 4.0F);
    dx_dt = filter.derivative();
    TEST_ASSERT_EQUAL_FLOAT(11.5F, dx_dt);
}
#endif

void test_derivative_filter_four_point()
{
    DerivativeFilter4point filter;
    filter.push_back(0.0F, 0.0F);
    filter.push_back(1.0F, 1.0F);
    filter.push_back(4.0F, 2.0F);
    filter.push_back(9.0F, 3.0F);
    float dx_dt = filter.derivative();

    TEST_ASSERT_EQUAL_FLOAT(10.0, dx_dt);


    filter.push_back(16.0F, 4.0F);
    dx_dt = filter.derivative();

    TEST_ASSERT_EQUAL_FLOAT(17.0, dx_dt);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_derivative_filter_two_point);
    RUN_TEST(test_derivative_filter_two_point_template);
    RUN_TEST(test_quadratic_solve);
    RUN_TEST(test_derivative_filter_three_point_a);
    RUN_TEST(test_derivative_filter_three_point_template_a);
    RUN_TEST(test_derivative_filter_three_point_b);
    RUN_TEST(test_derivative_filter_three_point_c);
    RUN_TEST(test_derivative_filter_three_point_d);
    RUN_TEST(test_derivative_filter_step);
    RUN_TEST(test_derivative_filter_four_point);

    UNITY_END();
}