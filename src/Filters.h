#pragma once

#include <algorithm>
#include <cmath>

/*!
Null filter.
*/
class FilterNull {
public:
    FilterNull()  {}
    inline void reset() {}
    inline float update(float input) { return input; }
    inline float update(float input, [[maybe_unused]] float dt) { return input; }
};


/*!
Simple moving average filter.
See [Moving Average Filter - Theory and Software Implementation - Phil's Lab #21](https://www.youtube.com/watch?v=rttn46_Y3c8).
*/
template <size_t N>
class FilterMovingAverage {
public:
    inline FilterMovingAverage()  {} // cppcheck-suppress uninitMemberVar
public:
    inline void reset() { _sum = 0.0F; _count = 0; _index = 0;}
    inline float update(float input);
    inline float update(float input, float dt) { (void)dt; return update(input); }
private:
    size_t _count {0};
    size_t _index {0};
    float _sum {0};
    float _samples[N];
};

template <size_t N>
inline float FilterMovingAverage<N>::update(float input)
{
    _sum += input;
    if (_count < N) {
        _samples[_index++] = input;
        ++_count;
        return _sum / static_cast<float>(_count);
    } else {
        if (_index == N) {
            _index = 0;
        }
        _sum -= _samples[_index];
        _samples[_index++] = input;
    }
    constexpr float nReciprocal = 1.0F / N;
    return _sum * nReciprocal;
}


/*!
Infinite Impulse Response (IIR) Filter.
Also known as Exponential Moving Average (EMA) Filter.
See https://en.wikipedia.org/wiki/Low-pass_filter#RC_filter
*/
class IIR_filter {
public:
    explicit IIR_filter(float frequencyCutoff) : _state(0.0F) {
        _omega = 2.0F * static_cast<float>(M_PI) * frequencyCutoff;
    }
    IIR_filter(float frequencyCutoff, float dT) : _state(0.0F) {
        setCutoffFrequency(frequencyCutoff, dT);
    }
    IIR_filter() {}
    inline void setCutoffFrequency(float frequencyCutoff, float dT) {
        _omega = 2.0F * static_cast<float>(M_PI) * frequencyCutoff;
        _alpha = _omega*dT/(_omega*dT + 1.0F);
    }
    inline void setAlpha(float alpha) { _alpha = alpha; }
    inline void reset() { _state = 0.0F; }
    inline float update(float input, float dT);
    inline float update(float input);
protected:
    float _alpha {0.0};
    float _omega {0.0};
    float _state {0.0};
};

/*!
Variable dT IIR_filter update;
*/
inline float IIR_filter::update(float input, float dT) {
    const float alpha = _omega*dT/(_omega*dT + 1.0F);
    _state += alpha * (input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
    return _state;
}
/*!
Constant dT IIR_filter update
*/
inline float IIR_filter::update(float input) {
    _state += _alpha * (input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
    return _state;
}


/*!
Finite Impulse Response (FIR) Filter
*/
template <size_t N>
class FIR_filter {
public:
    explicit FIR_filter(const float* coefficients) : _coefficients(coefficients), _back(0) { memset(_buffer, 0, sizeof(_buffer)); }
    inline float update(float input);
private:
    enum { ORDER = N };
private:
    const float* _coefficients;
    float _buffer[ORDER];
    size_t _back;  //!< The virtual end of the circular buffer (one behind the last element).
};

template <size_t N>
inline float FIR_filter<N>::update(float input) {
    auto index = _back;

    // Add the input value to the back of the circular buffer
    _buffer[_back] = input;
    ++_back;
    if (_back == ORDER) {
        _back = 0;
    }

    float output = 0.0F;
    for (auto ii = 0; ii < ORDER; ++ii) {
        output += _coefficients[ii] * _buffer[index];
        if (index == 0) {
            index = ORDER;
        }
        --index;
    }

    return output;
}


class ButterWorthFilter {
public:
    ButterWorthFilter(float a1, float a2, float b0, float b1, float b2) : _a1(a1), _a2(a2), _b0(b0), _b1(b1), _b2(b2), _x0(0.0F), _x1(0.0F), _y0(0.0F), _y1(0.0F) {}
    inline float update(float input);
    inline void reset() { _x0 = 0.0F; _x1 = 0.0F; _y0 = 0.0F; _y1 = 0.0F; }
private:
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    float _x0;
    float _x1;
    float _y0;
    float _y1;

};

inline float ButterWorthFilter::update(float input) {
    const float output = _b0 * input - _a2 * _y1 + _b1 * _x0 + _b2 * _x1 - _a1 * _y0;
    _y1 = _y0;
    _y0 = output;
    _x1 = _x0;
    _x0 = input;
    return output;
}
