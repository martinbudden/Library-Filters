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
    virtual float filter_virtual(float input) = 0;
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
    inline void set_to_passthrough() {}
    inline void set_cutoff_frequency(float cutoff_frequency_hz, float dt) { (void)cutoff_frequency_hz; (void)dt; }
    inline void set_cutoff_frequency_and_reset(float cutoff_frequency_hz, float dt) { (void)cutoff_frequency_hz; (void)dt; }

    inline float filter(float input) { return input; }
    inline float filter(float input, float dt) { (void)dt; return input; }
    virtual float filter_virtual(float input) override { return filter(input); }
};


/*!
First order power transfer filter
*/
class PowerTransferFilter1 : public FilterBase {
public:
    explicit PowerTransferFilter1(float k) : _k(k) {}
    PowerTransferFilter1() : PowerTransferFilter1(1.0F) {}
    PowerTransferFilter1(float cutoff_frequency_hz, float dt) : PowerTransferFilter1(gain_from_frequency(cutoff_frequency_hz, dt)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state = 0.0F; }
    inline void set_to_passthrough() { _k = 1.0F; reset(); }

    inline float filter(float input) {
        _state += _k*(input - _state); // equivalent to _state = _k*input + (1.0F - _k)*_state;
        return _state;
    }
    virtual float filter_virtual(float input) override { return filter(input); }

    inline void set_cutoff_frequency(float cutoff_frequency_hz, float dt) { _k = gain_from_frequency(cutoff_frequency_hz, dt); }
    inline void set_cutoff_frequency_and_reset(float cutoff_frequency_hz, float dt) { _k = gain_from_frequency(cutoff_frequency_hz, dt); reset(); }
    // Calculates filter gain based on delay (time constant of filter) - time it takes for filter response to reach 63.2% of a step input.
    static inline float gain_from_delay(float delay, float dt) {
        if (delay <= 0) { return 1.0F; } // gain of 1.0F means no filtering
        const float omega = dt/delay;
        return omega/(omega + 1.0F);
    }
    static inline float gain_from_frequency(float cutoff_frequency_hz, float dt) {
        const float omega = 2.0F*PI_F*cutoff_frequency_hz*dt;
        return omega/(omega + 1.0F);
    }
// for testing
    float get_state() const { return _state; }
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
    PowerTransferFilter2(float cutoff_frequency_hz, float dt) : PowerTransferFilter2(gain_from_frequency(cutoff_frequency_hz, dt)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state[0] = 0.0F; _state[1] = 0.0F; }
    inline void set_to_passthrough() { _k = 1.0F; }

    inline float filter(float input) {
        _state[1] += _k*(input - _state[1]);
        _state[0] += _k*(_state[1] - _state[0]);
        return _state[0];
    }
    virtual float filter_virtual(float input) override { return filter(input); }

    inline void set_cutoff_frequency(float cutoff_frequency_hz, float dt) { _k = gain_from_frequency(cutoff_frequency_hz, dt); }
    inline void set_cutoff_frequency_and_reset(float cutoff_frequency_hz, float dt) { _k = gain_from_frequency(cutoff_frequency_hz, dt); reset(); }
    static inline float gain_from_delay(float delay, float dt) {
        return PowerTransferFilter1::gain_from_delay(delay*CUTOFF_CORRECTION, dt);
    }
    static inline float gain_from_frequency(float cutoff_frequency_hz, float dt) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gain_from_frequency(cutoff_frequency_hz*CUTOFF_CORRECTION, dt);
    }
// for testing
    const std::array<float, 2>& get_state() const { return _state; }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float CUTOFF_CORRECTION = 1.553773974F;
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
    PowerTransferFilter3(float cutoff_frequency_hz, float dt) : PowerTransferFilter3(gain_from_frequency(cutoff_frequency_hz, dt)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state[0] = 0.0F; _state[1] = 0.0F; _state[2] = 0.0F; }
    inline void set_to_passthrough() { _k = 1.0F; reset(); }

    inline float filter(float input) {
        _state[2] += _k*(input - _state[2]);
        _state[1] += _k*(_state[2] - _state[1]);
        _state[0] += _k*(_state[1] - _state[0]);
        return _state[0];
    }
    virtual float filter_virtual(float input) override { return filter(input); }

    inline void set_cutoff_frequency(float cutoff_frequency_hz, float dt) { _k = gain_from_frequency(cutoff_frequency_hz, dt); }
    inline void set_cutoff_frequency_and_reset(float cutoff_frequency_hz, float dt) { _k = gain_from_frequency(cutoff_frequency_hz, dt); reset(); }
    static inline float gain_from_delay(float delay, float dt) {
        return PowerTransferFilter1::gain_from_delay(delay*CUTOFF_CORRECTION, dt);
    }
    static inline float gain_from_frequency(float cutoff_frequency_hz, float dt) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1::gain_from_frequency(cutoff_frequency_hz*CUTOFF_CORRECTION, dt);
    }
// for testing
    const std::array<float, 3>& get_state() const { return _state; }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float CUTOFF_CORRECTION = 1.961459177F;
    float _k;
    std::array<float, 3> _state {};
};


/*!
Biquad filter, see https://en.wikipedia.org/wiki/Digital_biquad_filter

Has additional `_weight` member data, which allows the filter to combine input and output using `filter_weighted()` function.
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
    void set_weight(float weight) { _weight = weight; }
    float get_weight() const { return _weight; }
    void set_parameters(float a1, float a2, float b0, float b1, float b2, float weight) {
        _weight = weight;
        _a1 = a1;
        _a2 = a2;
        _b0 = b0;
        _b1 = b1;
        _b2 = b2;
    }
    void set_parameters(float a1, float a2, float b0, float b1, float b2) {
        set_parameters(a1, a2, b0, b1, b2, 1.0F);
    }
    //! Copy parameters from another Biquad filter
    inline void set_parameters(const BiquadFilter& other) {
        _weight = other._weight;
        _a1 = other._a1;
        _a2 = other._a2;
        _b0 = other._b0;
        _b1 = other._b1;
        _b2 = other._b2;
    }

    inline void reset() { _state.x1 = 0.0F; _state.x2 = 0.0F; _state.y1 = 0.0F; _state.y2 = 0.0F; }
    inline void set_to_passthrough() { _b0 = 1.0F; _b1 = 0.0F; _b2 = 0.0F; _a1 = 0.0F; _a2 = 0.0F;  _weight = 1.0F; reset(); }

    inline float filter(float input) {
        const float output = _b0*input + _b1*_state.x1 + _b2*_state.x2 - _a1*_state.y1 - _a2*_state.y2;
        _state.x2 = _state.x1;
        _state.x1 = input;
        _state.y2 = _state.y1;
        _state.y1 = output;
        return output;
    }
    virtual float filter_virtual(float input) override { return filter(input); }

    inline float filter_weighted(float input) {
        const float output = filter(input);
        // weight of 1.0 gives just output, weight of 0.0 gives just input
        return _weight*(output - input) + input;
    }

    inline void init_lowpass(float frequency_hz, float looptime_seconds, float Q) {
        assert(Q != 0.0F && "Q cannot be zero");
        set_looptime(looptime_seconds);
        setQ(Q);
        set_low_pass_frequency(frequency_hz);
        reset();
    }
    inline void init_notch(float frequency_hz, float looptime_seconds, float Q) {
        assert(Q != 0.0F && "Q cannot be zero");
        set_looptime(looptime_seconds);
        setQ(Q);
        set_notch_frequency(frequency_hz);
        reset();
    }

    inline float calculate_omega(float frequency) const { return frequency*_2_pi_looptime_seconds; }

    void set_low_pass_frequencyWeighted(float frequency_hz, float weight);
    void set_low_pass_frequency(float frequency_hz) { set_low_pass_frequencyWeighted(frequency_hz, 1.0F); }

    inline void set_notch_frequency_weighted(float frequency_hz, float weight); // assumes Q already set
    inline void set_notch_frequency(float frequency_hz) {set_notch_frequency_weighted(frequency_hz, 1.0F); } // assumes Q already set
    inline void set_notch_frequency_weighted(float sin_omega, float two_cos_omega, float weight);
    inline void set_notch_frequency(float center_frequency_hz, float lower_cutoff_frequency_hz) {
        setQ(calculateQ(center_frequency_hz, lower_cutoff_frequency_hz));
        set_notch_frequency(center_frequency_hz);
    }
    inline void set_notch_frequency(uint16_t center_frequency_hz, uint16_t lower_cutoff_frequency_hz) {
        set_notch_frequency(static_cast<float>(center_frequency_hz), static_cast<float>(lower_cutoff_frequency_hz));
    }

    static float calculateQ(float center_frequency_hz, float lower_cutoff_frequency_hz) {
        return center_frequency_hz*lower_cutoff_frequency_hz/(center_frequency_hz*center_frequency_hz - lower_cutoff_frequency_hz*lower_cutoff_frequency_hz);
    }
    void setQ(float centerFrequency, float lower_cutoff_frequency) { _2Q_reciprocal = 1.0F/(2.0F*calculateQ(centerFrequency, lower_cutoff_frequency)); }
    void setQ(float Q) { _2Q_reciprocal = 1.0F /(2.0F*Q); }
    float getQ() const { return (1.0F/_2Q_reciprocal)/2.0F; }

    void set_looptime(float looptime_seconds) { _2_pi_looptime_seconds = 2.0F*PI_F*looptime_seconds; }
// for testing
    const state_t& get_state() const { return _state; }
protected:
    float _weight {1.0F}; //<! weight of 1.0 gives just output, weight of 0.0 gives just input
    float _a1;
    float _a2;
    float _b0;
    float _b1;
    float _b2;

    state_t _state {};

    float _2Q_reciprocal {1.0F}; // store 1/(2*Q), since that is what is used in set_notch_frequency calculations
    float _2_pi_looptime_seconds {0.0F}; // store 2*PI*looptime_seconds, since that is what is used in calculations
protected:
    static constexpr float PI_F = 3.14159265358979323846F;
};

/*!
Note: weight must be in range [0, 1].
*/
inline void BiquadFilter::set_low_pass_frequencyWeighted(float frequency_hz, float weight)
{
    _weight = weight;

    const float omega = frequency_hz*_2_pi_looptime_seconds;
#if defined(LIBRARY_FILTERS_USE_SINCOS)
    float sin_omega {};
    float cosOmega {};
    sincosf(omega, &sin_omega, &cosOmega);
    const float alpha = sin_omega*_2Q_reciprocal;
#else
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
#endif
    const float a0_reciprocal = 1.0F/(1.0F + alpha);

    _b1 = (1.0F - cosOmega)*a0_reciprocal;
    _b0 = _b1*0.5F;
    _b2 = _b0;
    _a1 = -2.0F*cosOmega*a0_reciprocal;
    _a2 = (1.0F - alpha)*a0_reciprocal;
}

/*!
Note: weight must be in range [0, 1].
*/
inline void BiquadFilter::set_notch_frequency_weighted(float frequency_hz, float weight)
{
    _weight = weight;

    const float omega = frequency_hz*_2_pi_looptime_seconds;
#if defined(LIBRARY_FILTERS_USE_SINCOS)
    float sin_omega {};
    float cosOmega {};
    sincosf(omega, &sin_omega, &cosOmega);
    const float alpha = sin_omega*_2Q_reciprocal;
#else
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
#endif
    const float a0_reciprocal = 1.0F/(1.0F + alpha);

    _b0 = a0_reciprocal;
    _b2 = a0_reciprocal;
    _b1 = -2.0F*cosOmega*a0_reciprocal;
    _a1 = _b1;
    _a2 = (1.0F - alpha)*a0_reciprocal;
}

/*!
Note: weight must be in range [0, 1].
*/
inline void BiquadFilter::set_notch_frequency_weighted(float sin_omega, float two_cos_omega, float weight)
{
    _weight = weight;

    const float alpha = sin_omega*_2Q_reciprocal;
    const float a0_reciprocal = 1.0F/(1.0F + alpha);

    _b0 = a0_reciprocal;
    _b2 = a0_reciprocal;
    _b1 = -two_cos_omega*a0_reciprocal;
    _a1 = _b1;
    _a2 = (1.0F - alpha)*a0_reciprocal;
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
    inline float filter(float input, float dt) { (void)dt; return filter(input); }
    virtual float filter_virtual(float input) override { return filter(input); }
protected:
    size_t _count {0};
    size_t _index {0};
    float _sum {0};
    std::array<float, N> _samples;
};

template <size_t N>
inline float FilterMovingAverage<N>::filter(float input)
{
    _sum += input;
    if (_count < N) {
        _samples[_index] = input;
        ++_index;
        ++_count;
        return _sum/static_cast<float>(_count);
    } else {
        if (_index == N) {
            _index = 0;
        }
        _sum -= _samples[_index];
        _samples[_index] = input;
        ++_index;
    }
    constexpr float n_reciprocal = 1.0F/static_cast<float>(N);
    return _sum*n_reciprocal;
}
