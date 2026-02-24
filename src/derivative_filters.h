#include "rolling_buffer.h"
#include <cstdint>


class DerivativeFilter2point {
    struct xt_t {
        float x;
        float t;
    };
public:
    void push_back(float x, float t) { _rb.push_back(xt_t{x, t}); }
    void fill(float dt) { _rb.push_back(xt_t{0.0F, 0.0F}); _rb.push_back(xt_t{0.0F, dt}); }
    float derivative() {
        const xt_t v0 = _rb.front();
        const xt_t v1 = _rb.back();
        return (v1.x - v0.x) / (v1.t - v0.t);
    }
    float filter(float x, float t) {
        const xt_t v1 = xt_t{x, t};
        _rb.push_back(v1);
        const xt_t v0 = _rb.front();
        return (v1.x - v0.x) / (v1.t - v0.t);
    }
private:
    RollingBuffer<xt_t, 2> _rb;
};

class DerivativeFilter2point32 {
private:
    struct xt_t {
        float x;
        uint32_t t;
    };
public:
    void push_back(float x, uint32_t t) { _rb.push_back(xt_t{x, t}); }
    float derivative() {
        const xt_t v0 = _rb.front();
        const xt_t v1 = _rb.back();
        return (v1.x - v0.x) / static_cast<float>(v1.t - v0.t);
    }
    float filter(float x, uint32_t t) {
        const xt_t v1 = xt_t{x, t};
        _rb.push_back(v1);
        const xt_t v0 = _rb.front();
        return (v1.x - v0.x) / static_cast<float>(v1.t - v0.t);
    }
private:
    RollingBuffer<xt_t, 2> _rb;
};

class DerivativeFilter3point {
private:
    struct xt_t {
        float x;
        float t;
    };
public:
    void push_back(float x, float t) { _rb.push_back(xt_t{x, t}); }
    void fill(float dt) { _rb.push_back(xt_t{0.0F, 0.0F}); _rb.push_back(xt_t{0.0F, dt}); _rb.push_back(xt_t{0.0F, 2.0F*dt}); }
    float derivative() {
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];
        const xt_t p2 = _rb.back();
        const float x1 = p1.x - p0.x;
        const float t1 = p1.t - p0.t;
        const float x2 = p2.x - p0.x;
        const float t2 = p2.t - p0.t;

        const float det = t1*t2*(t1 - t2);
        const float dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
    float filter(float x, float t) {
        const xt_t p2 = xt_t{x, t};
        _rb.push_back(p2);
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];
        const float x1 = p1.x - p0.x;
        const float t1 = p1.t - p0.t;
        const float x2 = p2.x - p0.x;
        const float t2 = p2.t - p0.t;

        const float det = t1*t2*(t1 - t2);
        const float dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
private:
    RollingBuffer<xt_t, 3> _rb;
};

class DerivativeFilter3point32 {
private:
    struct xt_t {
        float x;
        uint32_t t;
    };
public:
    void push_back(float x, uint32_t t) { _rb.push_back(xt_t{x, t}); }
    void fill(uint32_t dt) { _rb.push_back(xt_t{0.0F, 0}); _rb.push_back(xt_t{0.1F, dt}); _rb.push_back(xt_t{0.2F, 2*dt}); }
    float derivative() {
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];
        const xt_t p2 = _rb.back();
        const float x1 = p1.x - p0.x;
        const float t1 = static_cast<float>(p1.t - p0.t);
        const float x2 = p2.x - p0.x;
        const float t2 = static_cast<float>(p2.t - p0.t);

        const float det = t1*t2*(t1 - t2);
        const float dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
    float filter(float x, uint32_t t) {
        const xt_t p2 = xt_t{x, t};
        _rb.push_back(p2);
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];
        const float x1 = p1.x - p0.x;
        const float t1 = static_cast<float>(p1.t - p0.t);
        const float x2 = p2.x - p0.x;
        const float t2 = static_cast<float>(p2.t - p0.t);

        const float det = t1*t2*(t1 - t2);
        const float dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
private:
    RollingBuffer<xt_t, 3> _rb;
};

class DerivativeFilter3point32_X {
private:
    struct xt_t {
        float x;
        uint32_t t;
    };
public:
    void push_back(float x, uint32_t t) { _rb.push_back(xt_t{x, t}); }
    float derivative() {
        const xt_t v0 = _rb.front();
        const xt_t v1 = _rb[1];
        const xt_t v2 = _rb.back();
        const float h1 = static_cast<float>(v2.t - v1.t);
        const float h2 = static_cast<float>(v1.t - v0.t);
        const float dx_dt = (h1 + h2)/(h1*h1) * (v2.x - v1.x) - h2/(h1*(h1+h2)) * (v1.x - v0.x);
        return dx_dt;
    }
private:
    RollingBuffer<xt_t, 3> _rb;
};

class DerivativeFilter4point {
private:
    struct xt_t {
        float x;
        float t;
    };
public:
    void push_back(float x, float t) { _rb.push_back(xt_t{x, t}); }
    float derivative() {
        const xt_t v0 = _rb.front();
        const xt_t v1 = _rb[1];
        const xt_t v2 = _rb[2];
        const xt_t v3 = _rb.back();

        const float t0 = v0.t;
        const float t1 = v1.t;
        const float t2 = v2.t;
        const float t3 = v3.t;

        // Uniform or approximate step sizes
        //const float h1 = t1 - t0;
        //const float h2 = t2 - t1;
        //const float h3 = t3 - t2;

        // Use simple backward difference weights (non-uniform)
        // Formula: f'(t3) ~ a*x0 + b*x1 + c*x2 + d*x3
        // Derived by interpolating cubic polynomial over last 4 points and differentiating
        const float d0 = (t0-t1)*(t0-t2)*(t0-t3);
        const float d1 = (t1-t0)*(t1-t2)*(t1-t3);
        const float d2 = (t2-t0)*(t2-t1)*(t2-t3);
        //const float d3 = (t3-t0)*(t3-t1)*(t3-t2);

        /*const float denom0 = h1*(t0-t2)*(t3-t0);
        const float denom1 = h1*h2*(t3-t1);
        const float denom2 = (t0-t2)*h2*h3;
        const float denom3 = (t3-t0)*(t3-t1)*h3;*/

        const float a = (2*t3 - t1 - t2) / d0;
        const float b = (2*t3 - t0 - t2) / d1;
        const float c = (2*t3 - t0 - t1) / d2;
        const float d = 1.0F - (a + b + c); // approximate closure to sum weights to 1

        const float dx_dt = a*v0.x + b*v1.x + c*v2.x + d*v3.x;
        return dx_dt;
    }
private:
    RollingBuffer<xt_t, 4> _rb;
};
