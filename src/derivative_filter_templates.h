#include "rolling_buffer.h"
#include <cstdint>

template <typename T>
class DerivativeFilter2pointT {
    struct xt_t {
        T x;
        float t;
    };
public:
    void push_back(T x, float t) { _rb.push_back(xt_t{x, t}); }
    void fill(float dt) { _rb.push_back(xt_t{{}, 0.0F}); _rb.push_back(xt_t{{}, dt}); }
    float derivative() {
        const xt_t v0 = _rb.front();
        const xt_t v1 = _rb.back();
        return (v1.x - v0.x) / (v1.t - v0.t);
    }
    T filter(T x, float t) {
        const xt_t v1 = xt_t{x, t};
        _rb.push_back(v1);
        const xt_t v0 = _rb.front();
        return (v1.x - v0.x) / (v1.t - v0.t);
    }
private:
    RollingBuffer<xt_t, 2> _rb;
};

template <typename T>
class DerivativeFilter2point32T {
private:
    struct xt_t {
        T x;
        uint32_t t;
    };
public:
    void push_back(T x, uint32_t t) { _rb.push_back(xt_t{x, t}); }
    float derivative() {
        const xt_t v0 = _rb.front();
        const xt_t v1 = _rb.back();
        return (v1.x - v0.x) / static_cast<float>(v1.t - v0.t);
    }
    T filter(T x, uint32_t t) {
        const xt_t v1 = xt_t{x, t};
        _rb.push_back(v1);
        const xt_t v0 = _rb.front();
        return (v1.x - v0.x) / static_cast<float>(v1.t - v0.t);
    }
private:
    RollingBuffer<xt_t, 2> _rb;
};

template <typename T>
class DerivativeFilter3pointT {
private:
    struct xt_t {
        T x;
        float t;
    };
public:
    void push_back(T x, float t) { _rb.push_back(xt_t{x, t}); }
    T derivative() {
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];
        const xt_t p2 = _rb.back();

        const T x1 = p1.x - p0.x;
        const float t1 = p1.t - p0.t;
        const T x2 = p2.x - p0.x;
        const float t2 = p2.t - p0.t;

        const float det = t1*t2*(t1 - t2);
        const T dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
    T filter(float x, float t) {
        const T p2 = xt_t{x, t};
        _rb.push_back(p2);
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];

        const T x1 = p1.x - p0.x;
        const float t1 = p1.t - p0.t;
        const T x2 = p2.x - p0.x;
        const float t2 = p2.t - p0.t;

        const float det = t1*t2*(t1 - t2);
        const T dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
private:
    RollingBuffer<xt_t, 3> _rb;
};

template <typename T>
class DerivativeFilter3point32T {
private:
    struct xt_t {
        T x;
        uint32_t t;
    };
public:
    void push_back(T x, uint32_t t) { _rb.push_back(xt_t{x, t}); }
    void fill(T x, uint32_t dt) { _rb.push_back(xt_t{{}, 0}); _rb.push_back(xt_t{x, dt}); _rb.push_back(xt_t{x*2.0F, 2*dt}); }
    float derivative() {
        const xt_t p0 = _rb.front();
        const xt_t p1 = _rb[1];
        const xt_t p2 = _rb.back();
        const T x1 = p1.x - p0.x;
        const float t1 = static_cast<float>(p1.t - p0.t);
        const T x2 = p2.x - p0.x;
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
        const T x1 = p1.x - p0.x;
        const float t1 = static_cast<float>(p1.t - p0.t);
        const T x2 = p2.x - p0.x;
        const float t2 = static_cast<float>(p2.t - p0.t);

        const float det = t1*t2*(t1 - t2);
        const float dx_dt = (x1*t2*t2 + x2*t1*(t1 - 2.0F*t2))/det;
        return dx_dt;
    }
private:
    RollingBuffer<xt_t, 3> _rb;
};
