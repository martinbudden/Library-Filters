#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

/*!
Null filter.
*/
class FilterNull {
public:
    FilterNull() {}
public:
    inline void init(float k) { (void)k; }
    inline void reset() {}
    inline void setToPassthrough() {}
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { (void)cutoffFrequency; (void)dT; }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { (void)cutoffFrequency; (void)dT; }
    inline float update(float input) { return input; }
    inline float update(float input, float dT) { (void)dT; return input; }
};


/*!
Simple moving average filter.
See [Moving Average Filter - Theory and Software Implementation - Phil's Lab #21](https://www.youtube.com/watch?v=rttn46_Y3c8).
*/
template <size_t N>
class FilterMovingAverage {
public:
    FilterMovingAverage() {} // cppcheck-suppress uninitMemberVar
public:
    inline void reset() { _sum = 0.0F; _count = 0; _index = 0;}
    inline float update(float input);
    inline float update(float input, float dT) { (void)dT; return update(input); }
protected:
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
Finite Impulse Response (FIR) Filter
*/
template <size_t N>
class FIR_filter {
public:
    explicit FIR_filter(const float* coefficients) : _coefficients(coefficients), _back(0) { memset(_buffer, 0, sizeof(_buffer)); }
    inline float update(float input);
private:
    enum { ORDER = N };
protected:
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
protected:
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


/*!
Infinite Impulse Response (IIR) Filter.
Also known as Exponential Moving Average (EMA) Filter.
See https://en.wikipedia.org/wiki/Low-pass_filter#RC_filter
*/
class IIR_filter {
public:
    explicit IIR_filter(float frequencyCutoff) :
        _alpha(0.0F),
        _omega(2.0F * static_cast<float>(M_PI) * frequencyCutoff),
        _state(0.0F)
        {}
    IIR_filter(float frequencyCutoff, float dT): _state(0.0F) {
        setCutoffFrequency(frequencyCutoff, dT);
    }
    IIR_filter() : _alpha(1.0F), _omega(0.0F), _state(0.0F) {}
public:
    inline void init(float alpha) { _alpha = alpha; _state = 0.0F; }
    inline void reset() { _state = 0.0F; }
    inline void setToPassthrough() { _alpha = 1.0F; reset(); }
    inline void setAlpha(float alpha) { _alpha = alpha; }
    inline void setCutoffFrequency(float frequencyCutoff, float dT) {
        _omega = 2.0F * static_cast<float>(M_PI) * frequencyCutoff;
        _alpha = _omega*dT/(_omega*dT + 1.0F);
    }
    inline void setCutoffFrequencyAndReset(float frequencyCutoff, float dT) { setCutoffFrequency(frequencyCutoff, dT); reset(); }
    inline float update(float input, float dT) { // Variable dT IIR_filter update;
        const float alpha = _omega*dT/(_omega*dT + 1.0F);
        _state += alpha * (input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
        return _state;
    }
    inline float update(float input) { // Constant dT IIR_filter update
        _state += _alpha * (input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
        return _state;
    }
protected:
    float _alpha;
    float _omega;
    float _state;
};


/*!
First order power transfer filter
*/
class PowerTransferFilter1 {
public:
    explicit PowerTransferFilter1(float k) : _k(k), _state(0.0F) {}
    PowerTransferFilter1() : PowerTransferFilter1(1.0F) {}
    PowerTransferFilter1(float cutoffFrequency, float dT) : PowerTransferFilter1(gain(cutoffFrequency, dT)) {}
public:
    inline void init(float k) { _k = k; _state = 0.0F; }
    inline void reset() { _state = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }
    inline float update(float input) {
        _state += _k * (input - _state);
        return _state;
    }
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); reset(); }
    // Calculates filter gain based on delay (time constant of filter) - time it takes for filter response to reach 63.2% of a step input.
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float cutoffFrequency = 1.0F / (static_cast<float>(M_PI) * delay * 2.0F);
        return gain(cutoffFrequency, dT);
    }
    static inline float gain(float cutoffFrequency, float dT) {
        const float omega = 2.0F * static_cast<float>(M_PI) * cutoffFrequency * dT;
        return omega / (omega + 1.0F);
    }
protected:
    float _k;
    float _state;
};


/*!
Second order power transfer filter
*/
class PowerTransferFilter2 {
public:
    explicit PowerTransferFilter2(float k) : _k(k), _state0(0.0F), _state1(0.0F) {}
    PowerTransferFilter2() : PowerTransferFilter2(1.0F) {}
    PowerTransferFilter2(float cutoffFrequency, float dT) : PowerTransferFilter2(gain(cutoffFrequency, dT)) {}
public:
    inline void init(float k) { _state0 = 0.0F; _state1 = 0.0F; _k = k; }
    inline void reset() { _state0 = 0.0F; _state1 = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }
    inline float update(float input) {
        _state1 += _k * (input - _state1);
        _state0 += _k * (_state1 - _state0);
        return _state0;
    }
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float cutoffFrequency = 1.0F / (static_cast<float>(M_PI) * delay * cutoffCorrection);
        return gain(cutoffFrequency, dT);
    }
    static inline float gain(float cutoffFrequency, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gain(cutoffFrequency * cutoffCorrection, dT);
    }
protected:
    // PowerTransferFilter<n> cutoff correction = 1 / sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.553773974F;
    float _k;
    float _state0;
    float _state1;
};


/*!
Third order power transfer filter
*/
class PowerTransferFilter3 {
public:
    explicit PowerTransferFilter3(float k) : _k(k), _state0(0.0F), _state1(0.0F), _state2(0.0F) {}
    PowerTransferFilter3() : PowerTransferFilter3(1.0F) {}
    PowerTransferFilter3(float cutoffFrequency, float dT) : PowerTransferFilter3(gain(cutoffFrequency, dT)) {}
public:
    inline void init(float k) { _state0 = 0.0F; _state1 = 0.0F; _state2 = 0.0F; _k = k; }
    inline void reset() { _state0 = 0.0F; _state1 = 0.0F; _state2 = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }
    inline float update(float input) {
        _state2 += _k * (input - _state2);
        _state1 += _k * (_state2 - _state1);
        _state0 += _k * (_state1 - _state0);
        return _state0;
    }
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float cutoffFrequency = 1.0F / (static_cast<float>(M_PI) * delay * cutoffCorrection);
        return gain(cutoffFrequency, dT);
    }
    static inline float gain(float cutoffFrequency, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gain(cutoffFrequency * cutoffCorrection, dT);
    }
protected:
    // PowerTransferFilter<n> cutoff correction = 1 / sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.961459177F;
    float _k;
    float _state0;
    float _state1;
    float _state2;
};

/*!
Biquad filter, see https://en.wikipedia.org/wiki/Digital_biquad_filter
*/
class BiquadFilter {
public:
    BiquadFilter(float a1, float a2, float b0, float b1, float b2) :
        _a1(a1), _a2(a2),
        _b0(b0), _b1(b1), _b2(b2)
        {}
    BiquadFilter() : BiquadFilter(0.0F, 0.0F, 0.0F, 0.0F, 0.0F) {}
public:
    void setParameters(const BiquadFilter& other) {
        _a1 = other._a1;
        _a2 = other._a2;
        _b0 = other._b0;
        _b1 = other._b1;
        _b2 = other._b2;
        _weight = other._weight;
    }

    inline void reset() { _x1 = 0.0F; _x2 = 0.0F; _y1 = 0.0F; _y2 = 0.0F; }

    inline float update(float input) {
        const float output = _b0*input + _b1*_x1 + _b2*_x2 - _a1*_y1 - _a2*_y2;
        _x2 = _x1;
        _x1 = input;
        _y2 = _y1;
        _y1 = output;
        return output;
    }

    inline float updateWeighted(float input) {
        const float output = update(input);
        return _weight*(output - input) + input;
    }

    inline void initNotch(float frequency, uint32_t loopTimeUs, float Q) {
        _loopTimeUs = loopTimeUs;
        _Q = Q;
        setNotchFrequency(frequency, 0.0F);
        reset();
    }

    inline float calculateOmega(float frequency) const { return 2.0F * static_cast<float>(M_PI) * frequency * _loopTimeUs * 0.000001f; }
    void setNotchFrequency(float frequency, float weight);
    void setNotchFrequency(float sinOmega, float cosOmega, float weight);

    static float calculateQ(float centerFrequency, float lowerCutoffFrequency) {
        return centerFrequency*lowerCutoffFrequency / (centerFrequency*centerFrequency - lowerCutoffFrequency*lowerCutoffFrequency);
    }
    void setQ(float centerFrequency, float lowerCutoffFrequency) { _Q = calculateQ(centerFrequency, lowerCutoffFrequency); }
    void setQ(float Q) { _Q = Q; }
    void setLoopTime(uint32_t loopTimeUs) { _loopTimeUs = loopTimeUs; }
protected:
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    float _x1 {0.0F};
    float _x2 {0.0F};
    float _y1 {0.0F};
    float _y2 {0.0F};

    float _Q {0.0F};
    float _weight {0.0F};
    uint32_t _loopTimeUs {0};
};

inline void BiquadFilter::setNotchFrequency(float sinOmega, float cosOmega, float weight)
{
    _weight = weight;

    const float alpha = sinOmega / (2.0F * _Q);
    const float a0reciprocal = 1.0F / (1.0F + alpha);
    _b0 = a0reciprocal;
    _b2 = a0reciprocal;
    _b1 = -2.0F*cosOmega*a0reciprocal;
    _a1 = _b1;
    _a2 = (1.0F - alpha)*a0reciprocal;
}

inline void BiquadFilter::setNotchFrequency(float frequency, float weight)
{
    _weight = weight;

    const float omega = 2.0F * static_cast<float>(M_PI) * frequency * _loopTimeUs * 0.000001f;
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega) / (2.0F * _Q);
    const float a0reciprocal = 1.0F / (1.0F + alpha);
    _b0 = a0reciprocal;
    _b1 = -2.0F*cosOmega*a0reciprocal;
    _b2 = a0reciprocal;
    _a1 = _b1*a0reciprocal;
    _a2 = (1.0F - alpha)*a0reciprocal;
}
