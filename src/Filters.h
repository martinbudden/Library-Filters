#pragma once

#include <algorithm>
#include <cassert>
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
    inline float filter(float input) { return input; }
    inline float filter(float input, float dT) { (void)dT; return input; }
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
    inline float filter(float input);
    inline float filter(float input, float dT) { (void)dT; return filter(input); }
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
class FIR_filter {
public:
    explicit FIR_filter(const float* coefficients) : _coefficients(coefficients), _back(0) { memset(_buffer, 0, sizeof(_buffer)); }
    inline float filter(float input);
private:
    enum { ORDER = N };
protected:
    const float* _coefficients;
    float _buffer[ORDER];
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


class ButterWorthFilter {
public:
    ButterWorthFilter(float a1, float a2, float b0, float b1, float b2) : 
        _a1(a1), _a2(a2), 
        _b0(b0), _b1(b1), _b2(b2), 
        _x1(0.0F), _x2(0.0F), 
        _y1(0.0F), _y2(0.0F)
        {}
    ButterWorthFilter() : ButterWorthFilter(0.0F, 0.0F, 1.0F, 0.0F, 0.0F) {}
    //! Copy parameters from another ButterWorthFilter filter
    inline void setParameters(const ButterWorthFilter& other) {
        _a1 = other._a1;
        _a2 = other._a2;
        _b0 = other._b0;
        _b1 = other._b1;
        _b2 = other._b2;
    }

    inline void reset() { _x1 = 0.0F; _x2 = 0.0F; _y1 = 0.0F; _y2 = 0.0F; }
    inline void setToPassthrough() { _b0 = 1.0F; _b1 = 0.0F; _b2 = 0.0F; _a1 = 0.0F; _a2 = 0.0F; reset(); }
    inline float filter(float input) {
        const float output = _b0*input - _a2*_y2 + _b1*_x1 + _b2*_x2 - _a1*_y1;
        _y2 = _y1;
        _y1 = output;
        _x2 = _x1;
        _x1 = input;
        return output;
    }
protected:
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    float _x1;
    float _x2;
    float _y1;
    float _y2;
};


/*!
Infinite Impulse Response (IIR) Filter.
Also known as Exponential Moving Average (EMA) Filter.
See https://en.wikipedia.org/wiki/Low-pass_filter#RC_filter
*/
class IIR_filter {
public:
    explicit IIR_filter(float frequencyCutoff) :
        _alpha(0.0F),
        _omega(2.0F*PI_F*frequencyCutoff),
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
        _omega = 2.0F*PI_F*frequencyCutoff;
        _alpha = _omega*dT/(_omega*dT + 1.0F);
    }
    inline void setCutoffFrequencyAndReset(float frequencyCutoff, float dT) { setCutoffFrequency(frequencyCutoff, dT); reset(); }
    inline float filter(float input, float dT) { // Variable dT IIR_filter filter;
        const float alpha = _omega*dT/(_omega*dT + 1.0F);
        _state += alpha*(input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
        return _state;
    }
    inline float filter(float input) { // Constant dT IIR_filter filter
        _state += _alpha*(input - _state); // optimized form of _state = alpha*input + (1.0F - alpha)*_state
        return _state;
    }
protected:
    static constexpr float PI_F = 3.141592653589793F;
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
    inline float filter(float input) {
        _state += _k*(input - _state);
        return _state;
    }
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); reset(); }
    // Calculates filter gain based on delay (time constant of filter) - time it takes for filter response to reach 63.2% of a step input.
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float cutoffFrequency = 1.0F/PI_F*delay*2.0F;
        return gain(cutoffFrequency, dT);
    }
    static inline float gain(float cutoffFrequency, float dT) {
        const float omega = 2.0F*PI_F*cutoffFrequency*dT;
        return omega/(omega + 1.0F);
    }
protected:
    static constexpr float PI_F = 3.141592653589793F;
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
    inline float filter(float input) {
        _state1 += _k*(input - _state1);
        _state0 += _k*(_state1 - _state0);
        return _state0;
    }
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float cutoffFrequency = 1.0F/PI_F*delay*cutoffCorrection;
        return gain(cutoffFrequency, dT);
    }
    static inline float gain(float cutoffFrequency, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gain(cutoffFrequency*cutoffCorrection, dT);
    }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.553773974F;
    static constexpr float PI_F = 3.141592653589793F;
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
    inline float filter(float input) {
        _state2 += _k*(input - _state2);
        _state1 += _k*(_state2 - _state1);
        _state0 += _k*(_state1 - _state0);
        return _state0;
    }
    inline void setCutoffFrequency(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequency, float dT) { _k = gain(cutoffFrequency, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float cutoffFrequency = 1.0F/(PI_F*delay*cutoffCorrection);
        return gain(cutoffFrequency, dT);
    }
    static inline float gain(float cutoffFrequency, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gain(cutoffFrequency*cutoffCorrection, dT);
    }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.961459177F;
    static constexpr float PI_F = 3.141592653589793F;
    float _k;
    float _state0;
    float _state1;
    float _state2;
};

/*!
Biquad filter, see https://en.wikipedia.org/wiki/Digital_biquad_filter

Has additional `_weight` member data, which allows the filter to combine input and output using `filterWeighted()` function.
`_weight` is ignored when using `filter()` function.
*/
class BiquadFilter {
public:
    BiquadFilter(float a1, float a2, float b0, float b1, float b2) :
        _weight(1.0F),
        _a1(a1), _a2(a2),
        _b0(b0), _b1(b1), _b2(b2)
        {}
    BiquadFilter() : BiquadFilter(0.0F, 0.0F, 1.0F, 0.0F, 0.0F) {}
public:
    void setWeight(float weight) { _weight = weight; }
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

    inline void reset() { _x1 = 0.0F; _x2 = 0.0F; _y1 = 0.0F; _y2 = 0.0F; }
    inline void setToPassthrough() { _b0 = 1.0F; _b1 = 0.0F; _b2 = 0.0F; _a1 = 0.0F; _a2 = 0.0F;  _weight = 1.0F; reset(); }

    inline float filter(float input) {
        const float output = _b0*input + _b1*_x1 + _b2*_x2 - _a1*_y1 - _a2*_y2;
        _x2 = _x1;
        _x1 = input;
        _y2 = _y1;
        _y1 = output;
        return output;
    }

    inline float filterWeighted(float input) {
        const float output = filter(input);
        // weight of 1.0 gives just output, weight of 0.0 gives just input
        return _weight*(output - input) + input;
    }

    inline void initNotch(float frequency, uint32_t loopTimeUs, float Q) {
        assert(Q!=0.0F && "Q cannot be zero");
        setLoopTime(loopTimeUs);
        setQ(Q);
        setNotchFrequency(frequency, 0.0F);
        reset();
    }

    inline float calculateOmega(float frequency) const { return frequency*_2PiLoopTimeSeconds; }
    void setLowPassFrequency(float frequency, float weight);
    inline void setNotchFrequency(float frequency, float weight);
    inline void setNotchFrequency(float sinOmega, float two_cosOmega, float weight);

    static float calculateQ(float centerFrequency, float lowerCutoffFrequency) {
        return centerFrequency*lowerCutoffFrequency/(centerFrequency*centerFrequency - lowerCutoffFrequency*lowerCutoffFrequency);
    }
    void setQ(float centerFrequency, float lowerCutoffFrequency) { _2Q_reciprocal = 1.0F/(2.0F*calculateQ(centerFrequency, lowerCutoffFrequency)); }
    void setQ(float Q) { _2Q_reciprocal = 1.0F /(2.0F*Q); }
    float getQ() const { return (1.0F/_2Q_reciprocal)/2.0F; }
    void setLoopTime(uint32_t loopTimeUs) { _2PiLoopTimeSeconds = 2.0F*PI_F*static_cast<float>(loopTimeUs)*0.000001F; }
protected:
    static constexpr float PI_F = 3.141592653589793F;
    float _weight {1.0F}; //<! weight of 1.0 gives just output, weight of 0.0 gives just input
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    float _x1 {0.0F};
    float _x2 {0.0F};
    float _y1 {0.0F};
    float _y2 {0.0F};

    float _2Q_reciprocal {1.0F}; // store 2*Q, since that is what is used in calculations
    float _2PiLoopTimeSeconds {0.0F}; // store 2*PI*loopTimeSeconds, since that is what is used in calculations
};

inline void BiquadFilter::setLowPassFrequency(float frequency, float weight)
{
    _weight = weight;

    const float omega = frequency*_2PiLoopTimeSeconds;
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
    const float a0reciprocal = 1.0F/(1.0F + alpha);

    _b1 = (1.0F - cosOmega)*a0reciprocal;
    _b0 = _b1*0.5F;
    _b2 = _b0;
    _a1 = -2.0F*cosOmega*a0reciprocal;
    _a2 = (1.0F - alpha)*a0reciprocal;
}

inline void BiquadFilter::setNotchFrequency(float frequency, float weight)
{
    _weight = weight;

    const float omega = frequency*_2PiLoopTimeSeconds;
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
    const float a0reciprocal = 1.0F/(1.0F + alpha);

    _b0 = a0reciprocal;
    _b2 = a0reciprocal;
    _b1 = -2.0F*cosOmega*a0reciprocal;
    _a1 = _b1;
    _a2 = (1.0F - alpha)*a0reciprocal;
}

inline void BiquadFilter::setNotchFrequency(float sinOmega, float two_cosOmega, float weight)
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
