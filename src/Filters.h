#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>

/*!
Filter abstract base class.
*/
class FilterBase {
public:
    virtual ~FilterBase() = default;
    virtual float filterVirtual(float input) = 0;
};


/*!
Null filter.
*/
class FilterNull : public FilterBase {
public:
    FilterNull() = default;
public:
    inline void init(float k) { (void)k; }
    inline void reset() {}
    inline void setToPassthrough() {}
    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { (void)cutoffFrequencyHz; (void)dT; }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { (void)cutoffFrequencyHz; (void)dT; }

    inline float filter(float input) { return input; }
    inline float filter(float input, float dT) { (void)dT; return input; }
    virtual float filterVirtual(float input) override { return filter(input); }
};


/*!
First order power transfer filter
*/
class PowerTransferFilter1 : public FilterBase {
public:
    explicit PowerTransferFilter1(float k) : _k(k) {}
    PowerTransferFilter1() : PowerTransferFilter1(1.0F) {}
    PowerTransferFilter1(float cutoffFrequencyHz, float dT) : PowerTransferFilter1(gainFromFrequency(cutoffFrequencyHz, dT)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }

    inline float filter(float input) {
        _state += _k*(input - _state); // equivalent to _state = _k*input + (1.0F - _k)*_state;
        return _state;
    }
    virtual float filterVirtual(float input) override { return filter(input); }

    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); reset(); }
    // Calculates filter gain based on delay (time constant of filter) - time it takes for filter response to reach 63.2% of a step input.
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float omega = dT/delay;
        return omega/(omega + 1.0F);
    }
    static inline float gainFromFrequency(float cutoffFrequencyHz, float dT) {
        const float omega = 2.0F*PI_F*cutoffFrequencyHz*dT;
        return omega/(omega + 1.0F);
    }
// for testing
    float getState() const { return _state; }
protected:
    float _k;
    float _state {};
protected:
    static constexpr float PI_F = 3.14159265358979323846F;
};


/*!
Second order power transfer filter
*/
class PowerTransferFilter2 : public FilterBase {
public:
    explicit PowerTransferFilter2(float k) : _k(k) {}
    PowerTransferFilter2() : PowerTransferFilter2(1.0F) {}
    PowerTransferFilter2(float cutoffFrequencyHz, float dT) : PowerTransferFilter2(gainFromFrequency(cutoffFrequencyHz, dT)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state[0] = 0.0F; _state[1] = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; }

    inline float filter(float input) {
        _state[1] += _k*(input - _state[1]);
        _state[0] += _k*(_state[1] - _state[0]);
        return _state[0];
    }
    virtual float filterVirtual(float input) override { return filter(input); }

    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        return PowerTransferFilter1::gainFromDelay(delay*cutoffCorrection, dT);
    }
    static inline float gainFromFrequency(float cutoffFrequencyHz, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gainFromFrequency(cutoffFrequencyHz*cutoffCorrection, dT);
    }
// for testing
    const std::array<float, 2>& getState() const { return _state; }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.553773974F;
    float _k;
    std::array<float, 2> _state {};
};


/*!
Third order power transfer filter
*/
class PowerTransferFilter3 : public FilterBase {
public:
    explicit PowerTransferFilter3(float k) : _k(k) {}
    PowerTransferFilter3() : PowerTransferFilter3(1.0F) {}
    PowerTransferFilter3(float cutoffFrequencyHz, float dT) : PowerTransferFilter3(gainFromFrequency(cutoffFrequencyHz, dT)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state[0] = 0.0F; _state[1] = 0.0F; _state[2] = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }

    inline float filter(float input) {
        _state[2] += _k*(input - _state[2]);
        _state[1] += _k*(_state[2] - _state[1]);
        _state[0] += _k*(_state[1] - _state[0]);
        return _state[0];
    }
    virtual float filterVirtual(float input) override { return filter(input); }

    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        return PowerTransferFilter1::gainFromDelay(delay*cutoffCorrection, dT);
    }
    static inline float gainFromFrequency(float cutoffFrequencyHz, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gainFromFrequency(cutoffFrequencyHz*cutoffCorrection, dT);
    }
// for testing
    const std::array<float, 3>& getState() const { return _state; }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.961459177F;
    float _k;
    std::array<float, 3> _state {};
};


/*!
Biquad filter, see https://en.wikipedia.org/wiki/Digital_biquad_filter

Has additional `_weight` member data, which allows the filter to combine input and output using `filterWeighted()` function.
`_weight` is ignored when using `filter()` function.
*/
class BiquadFilter : public FilterBase {
public:
    BiquadFilter(float a1, float a2, float b0, float b1, float b2) :
        _weight(1.0F),
        _a1(a1), _a2(a2),
        _b0(b0), _b1(b1), _b2(b2)
        {}
    BiquadFilter() : BiquadFilter(0.0F, 0.0F, 1.0F, 0.0F, 0.0F) {}
    struct state_t {
        float x1;
        float x2;
        float y1;
        float y2;
    };
public:
    void setWeight(float weight) { _weight = weight; }
    float getWeight() const { return _weight; }
    void setParameters(float a1, float a2, float b0, float b1, float b2, float weight) {
        _weight = weight;
        _a1 = a1;
        _a2 = a2;
        _b0 = b0;
        _b1 = b1;
        _b2 = b2;
    }
    void setParameters(float a1, float a2, float b0, float b1, float b2) {
        setParameters(a1, a2, b0, b1, b2, 1.0F);
    }
    //! Copy parameters from another Biquad filter
    inline void setParameters(const BiquadFilter& other) {
        _weight = other._weight;
        _a1 = other._a1;
        _a2 = other._a2;
        _b0 = other._b0;
        _b1 = other._b1;
        _b2 = other._b2;
    }

    inline void reset() { _state.x1 = 0.0F; _state.x2 = 0.0F; _state.y1 = 0.0F; _state.y2 = 0.0F; }
    inline void setToPassthrough() { _b0 = 1.0F; _b1 = 0.0F; _b2 = 0.0F; _a1 = 0.0F; _a2 = 0.0F;  _weight = 1.0F; reset(); }

    inline float filter(float input) {
        const float output = _b0*input + _b1*_state.x1 + _b2*_state.x2 - _a1*_state.y1 - _a2*_state.y2;
        _state.x2 = _state.x1;
        _state.x1 = input;
        _state.y2 = _state.y1;
        _state.y1 = output;
        return output;
    }
    virtual float filterVirtual(float input) override { return filter(input); }

    inline float filterWeighted(float input) {
        const float output = filter(input);
        // weight of 1.0 gives just output, weight of 0.0 gives just input
        return _weight*(output - input) + input;
    }

    inline void initLowPass(float frequencyHz, float loopTimeSeconds, float Q) {
        assert(Q != 0.0F && "Q cannot be zero");
        setLoopTime(loopTimeSeconds);
        setQ(Q);
        setLowPassFrequency(frequencyHz);
        reset();
    }
    inline void initNotch(float frequencyHz, float loopTimeSeconds, float Q) {
        assert(Q != 0.0F && "Q cannot be zero");
        setLoopTime(loopTimeSeconds);
        setQ(Q);
        setNotchFrequency(frequencyHz);
        reset();
    }

    inline float calculateOmega(float frequency) const { return frequency*_2PiLoopTimeSeconds; }

    void setLowPassFrequencyWeighted(float frequencyHz, float weight);
    void setLowPassFrequency(float frequencyHz) { setLowPassFrequencyWeighted(frequencyHz, 1.0F); }

    inline void setNotchFrequencyWeighted(float frequencyHz, float weight); // assumes Q already set
    inline void setNotchFrequency(float frequencyHz) {setNotchFrequencyWeighted(frequencyHz, 1.0F); } // assumes Q already set
    inline void setNotchFrequencyWeighted(float sinOmega, float two_cosOmega, float weight);
    inline void setNotchFrequency(float centerFrequencyHz, float lowerCutoffFrequencyHz) {
        setQ(calculateQ(centerFrequencyHz, lowerCutoffFrequencyHz));
        setNotchFrequency(centerFrequencyHz);
    }
    inline void setNotchFrequency(uint16_t centerFrequencyHz, uint16_t lowerCutoffFrequencyHz) {
        setNotchFrequency(static_cast<float>(centerFrequencyHz), static_cast<float>(lowerCutoffFrequencyHz));
    }

    static float calculateQ(float centerFrequencyHz, float lowerCutoffFrequencyHz) {
        return centerFrequencyHz*lowerCutoffFrequencyHz/(centerFrequencyHz*centerFrequencyHz - lowerCutoffFrequencyHz*lowerCutoffFrequencyHz);
    }
    void setQ(float centerFrequency, float lowerCutoffFrequency) { _2Q_reciprocal = 1.0F/(2.0F*calculateQ(centerFrequency, lowerCutoffFrequency)); }
    void setQ(float Q) { _2Q_reciprocal = 1.0F /(2.0F*Q); }
    float getQ() const { return (1.0F/_2Q_reciprocal)/2.0F; }

    void setLoopTime(float loopTimeSeconds) { _2PiLoopTimeSeconds = 2.0F*PI_F*loopTimeSeconds; }
// for testing
    const state_t& getState() const { return _state; }
protected:
    float _weight {1.0F}; //<! weight of 1.0 gives just output, weight of 0.0 gives just input
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    state_t _state {};

    float _2Q_reciprocal {1.0F}; // store 1/(2*Q), since that is what is used in setNotchFrequency calculations
    float _2PiLoopTimeSeconds {0.0F}; // store 2*PI*loopTimeSeconds, since that is what is used in calculations
protected:
    static constexpr float PI_F = 3.14159265358979323846F;
};

/*!
Note: weight must be in range [0, 1].
*/
inline void BiquadFilter::setLowPassFrequencyWeighted(float frequencyHz, float weight)
{
    _weight = weight;

    const float omega = frequencyHz*_2PiLoopTimeSeconds;
#if defined(LIBRARY_FILTERS_USE_SINCOS)
    float sinOmega {};
    float cosOmega {};
    sincosf(omega, &sinOmega, &cosOmega);
    const float alpha = sinOmega*_2Q_reciprocal;
#else
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
#endif
    const float a0reciprocal = 1.0F/(1.0F + alpha);

    _b1 = (1.0F - cosOmega)*a0reciprocal;
    _b0 = _b1*0.5F;
    _b2 = _b0;
    _a1 = -2.0F*cosOmega*a0reciprocal;
    _a2 = (1.0F - alpha)*a0reciprocal;
}

/*!
Note: weight must be in range [0, 1].
*/
inline void BiquadFilter::setNotchFrequencyWeighted(float frequencyHz, float weight)
{
    _weight = weight;

    const float omega = frequencyHz*_2PiLoopTimeSeconds;
#if defined(LIBRARY_FILTERS_USE_SINCOS)
    float sinOmega {};
    float cosOmega {};
    sincosf(omega, &sinOmega, &cosOmega);
    const float alpha = sinOmega*_2Q_reciprocal;
#else
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
#endif
    const float a0reciprocal = 1.0F/(1.0F + alpha);

    _b0 = a0reciprocal;
    _b2 = a0reciprocal;
    _b1 = -2.0F*cosOmega*a0reciprocal;
    _a1 = _b1;
    _a2 = (1.0F - alpha)*a0reciprocal;
}

/*!
Note: weight must be in range [0, 1].
*/
inline void BiquadFilter::setNotchFrequencyWeighted(float sinOmega, float two_cosOmega, float weight)
{
    _weight = weight;

    const float alpha = sinOmega*_2Q_reciprocal;
    const float a0reciprocal = 1.0F/(1.0F + alpha);

    _b0 = a0reciprocal;
    _b2 = a0reciprocal;
    _b1 = -two_cosOmega*a0reciprocal;
    _a1 = _b1;
    _a2 = (1.0F - alpha)*a0reciprocal;
}


/*!
Simple moving average filter.
See [Moving Average Filter - Theory and Software Implementation - Phil's Lab #21](https://www.youtube.com/watch?v=rttn46_Y3c8).
*/
template <size_t N>
class FilterMovingAverage : public FilterBase {
public:
    FilterMovingAverage() {} // cppcheck-suppress uninitMemberVar
public:
    inline void reset() { _sum = 0.0F; _count = 0; _index = 0;}

    inline float filter(float input);
    inline float filter(float input, float dT) { (void)dT; return filter(input); }
    virtual float filterVirtual(float input) override { return filter(input); }
protected:
    size_t _count {0};
    size_t _index {0};
    float _sum {0};
    float _samples[N];
};

template <size_t N>
inline float FilterMovingAverage<N>::filter(float input)
{
    _sum += input;
    if (_count < N) {
        _samples[_index++] = input;
        ++_count;
        return _sum/static_cast<float>(_count);
    } else {
        if (_index == N) {
            _index = 0;
        }
        _sum -= _samples[_index];
        _samples[_index++] = input;
    }
    constexpr float nReciprocal = 1.0F/N;
    return _sum*nReciprocal;
}


/*!
Finite Impulse Response (FIR) Filter
*/
template <size_t N>
class FIR_filter : public FilterBase {
public:
    explicit FIR_filter(const float* coefficients) : _coefficients(coefficients), _back(0) { memset(_buffer, 0, sizeof(_buffer)); }

    inline float filter(float input);
    virtual float filterVirtual(float input) override { return filter(input); }
private:
    enum { ORDER = N };
protected:
    const float* _coefficients;
    std::array<float, ORDER> _buffer;
    size_t _back;  //!< The virtual end of the circular buffer (one behind the last element).
};

template <size_t N>
inline float FIR_filter<N>::filter(float input) {
    auto index = _back;

    // Add the input value to the back of the circular buffer
    _buffer[_back] = input;
    ++_back;
    if (_back == ORDER) {
        _back = 0;
    }

    float output = 0.0F;
    for (auto ii = 0; ii < ORDER; ++ii) {
        output += _coefficients[ii]*_buffer[index];
        if (index == 0) {
            index = ORDER;
        }
        --index;
    }

    return output;
}


class ButterWorthFilter : public FilterBase {
public:
    ButterWorthFilter(float a1, float a2, float b0, float b1, float b2) : 
        _a1(a1), _a2(a2), 
        _b0(b0), _b1(b1), _b2(b2), 
        _state { .x1 = 0.0F, .x2 = 0.0F, .y1 = 0.0F, .y2 = 0.0F }
        {}
    ButterWorthFilter() : ButterWorthFilter(0.0F, 0.0F, 1.0F, 0.0F, 0.0F) {}
    struct state_t {
        float x1;
        float x2;
        float y1;
        float y2;
    };
    //! Copy parameters from another ButterWorthFilter filter
    inline void setParameters(const ButterWorthFilter& other) {
        _a1 = other._a1;
        _a2 = other._a2;
        _b0 = other._b0;
        _b1 = other._b1;
        _b2 = other._b2;
    }

    inline void reset() { _state = { 0.0F, 0.0F, 0.0F, 0.0F }; }
    inline void setToPassthrough() { _b0 = 1.0F; _b1 = 0.0F; _b2 = 0.0F; _a1 = 0.0F; _a2 = 0.0F; reset(); }

    inline float filter(float input) {
        const float output = _b0*input - _a2*_state.y2 + _b1*_state.x1 + _b2*_state.x2 - _a1*_state.y1;
        _state.y2 = _state.y1;
        _state.y1 = output;
        _state.x2 = _state.x1;
        _state.x1 = input;
        return output;
    }
    virtual float filterVirtual(float input) override { return filter(input); }
protected:
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    state_t _state;
};


/*!
Infinite Impulse Response (IIR) Filter.
Also known as Exponential Moving Average (EMA) Filter.
See https://en.wikipedia.org/wiki/Low-pass_filter#RC_filter
*/
class IIR_filter : public FilterBase {
public:
    explicit IIR_filter(float cutoffFrequencyHz) :
        _alpha(0.0F),
        _omega(2.0F*PI_F*cutoffFrequencyHz),
        _state(0.0F)
        {}
    IIR_filter(float cutoffFrequencyHz, float dT): _state(0.0F) {
        setCutoffFrequency(cutoffFrequencyHz, dT);
    }
    IIR_filter() : _alpha(1.0F), _omega(0.0F), _state(0.0F) {}
public:
    inline void init(float alpha) { _alpha = alpha; _state = 0.0F; }
    inline void reset() { _state = 0.0F; }
    inline void setToPassthrough() { _alpha = 1.0F; reset(); }
    inline void setAlpha(float alpha) { _alpha = alpha; }
    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) {
        _omega = 2.0F*PI_F*cutoffFrequencyHz;
        _alpha = _omega*dT/(_omega*dT + 1.0F);
    }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { setCutoffFrequency(cutoffFrequencyHz, dT); reset(); }

    inline float filter(float input, float dT) { // Variable dT IIR_filter filter;
        const float alpha = _omega*dT/(_omega*dT + 1.0F);
        _state += alpha*(input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
        return _state;
    }
    inline float filter(float input) { // Constant dT IIR_filter filter
        _state += _alpha*(input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
        return _state;
    }
    virtual float filterVirtual(float input) override { return filter(input); }
protected:
    float _alpha;
    float _omega;
    float _state;
protected:
    static constexpr float PI_F = 3.14159265358979323846F;
};
