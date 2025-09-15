#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>

/*!
Templated variants of selected filters.
*/

/*!
Filter abstract base class.
*/
template <typename T>
class FilterBaseT {
public:
    virtual T filterVirtual(const T& input) = 0;
};


/*!
Null filter.
*/
template <typename T>
class FilterNullT : public FilterBaseT<T> {
public:
    FilterNullT() = default;
public:
    inline void init(float k) { (void)k; }
    inline void reset() {}
    inline void setToPassthrough() {}
    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { (void)cutoffFrequencyHz; (void)dT; }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { (void)cutoffFrequencyHz; (void)dT; }

    inline T filter(const T& input) { return input; }
    inline T filter(const T& input, float dT) { (void)dT; return input; }
    virtual T filterVirtual(const T& input) override { return filter(input); }
};


/*!
First order power transfer filter
*/
template <typename T>
class PowerTransferFilter1T : public FilterBaseT<T> {
public:
    explicit PowerTransferFilter1T(float k) : _k(k) {}
    PowerTransferFilter1T() : PowerTransferFilter1T(1.0F) {}
    PowerTransferFilter1T(float cutoffFrequencyHz, float dT) : PowerTransferFilter1T(gainFromFrequency(cutoffFrequencyHz, dT)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }

    inline T filter(const T& input) {
        _state += _k*(input - _state); // equivalent to _state = _k*input + (1.0F - _k)*_state;
        return _state;
    }
    virtual T filterVirtual(const T& input) override { return filter(input); }

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
    const T& getState() const { return _state; }
protected:
    float _k;
    T _state {};
protected:
    static constexpr float PI_F = static_cast<float>(M_PI);
};


/*!
Second order power transfer filter
*/
template <typename T>
class PowerTransferFilter2T : public FilterBaseT<T> {
public:
    explicit PowerTransferFilter2T(float k) : _k(k) {}
    PowerTransferFilter2T() : PowerTransferFilter2T(1.0F) {}
    PowerTransferFilter2T(float cutoffFrequencyHz, float dT) : PowerTransferFilter2T(gainFromFrequency(cutoffFrequencyHz, dT)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state[0] = 0.0F; _state[1] = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; }

    inline T filter(const T& input) {
        _state[1] += _k*(input - _state[1]);
        _state[0] += _k*(_state[1] - _state[0]);
        return _state[0];
    }
    virtual T filterVirtual(const T& input) override { return filter(input); }

    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        return PowerTransferFilter1T<T>::gainFromDelay(delay*cutoffCorrection, dT);
    }
    static inline float gainFromFrequency(float cutoffFrequencyHz, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1T<T>::gainFromFrequency(cutoffFrequencyHz*cutoffCorrection, dT);
    }
// for testing
    const std::array<T, 2>& getState() const { return _state; }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.553773974F;
    float _k;
    std::array<T, 2> _state {};
};


/*!
Third order power transfer filter
*/
template <typename T>
class PowerTransferFilter3T : public FilterBaseT<T> {
public:
    explicit PowerTransferFilter3T(float k) : _k(k) {}
    PowerTransferFilter3T() : PowerTransferFilter3T(1.0F) {}
    PowerTransferFilter3T(float cutoffFrequencyHz, float dT) : PowerTransferFilter3T(gainFromFrequency(cutoffFrequencyHz, dT)) {}
public:
    inline void init(float k) { _k = k; reset(); }
    inline void reset() { _state[0] = 0.0F; _state[1] = 0.0F; _state[2] = 0.0F; }
    inline void setToPassthrough() { _k = 1.0F; reset(); }

    inline T filter(const T& input) {
        _state[2] += _k*(input - _state[2]);
        _state[1] += _k*(_state[2] - _state[1]);
        _state[0] += _k*(_state[1] - _state[0]);
        return _state[0];
    }
    virtual T filterVirtual(const T& input) override { return filter(input); }

    inline void setCutoffFrequency(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); }
    inline void setCutoffFrequencyAndReset(float cutoffFrequencyHz, float dT) { _k = gainFromFrequency(cutoffFrequencyHz, dT); reset(); }
    static inline float gainFromDelay(float delay, float dT) {
        return PowerTransferFilter1T<T>::gainFromDelay(delay*cutoffCorrection, dT);
    }
    static inline float gainFromFrequency(float cutoffFrequencyHz, float dT) {
        // shift cutoffFrequency to satisfy -3dB cutoff condition
        return PowerTransferFilter1T<T>::gainFromFrequency(cutoffFrequencyHz*cutoffCorrection, dT);
    }
// for testing
    const std::array<T, 3>& getState() const { return _state; }
protected:
    // PowerTransferFilter<n> cutoff correction = 1/sqrt(2^(1/n) - 1)
    static constexpr float cutoffCorrection = 1.961459177F;
    float _k;
    std::array<T, 3> _state {};
};


/*!
Biquad filter, see https://en.wikipedia.org/wiki/Digital_biquad_filter

Has additional `_weight` member data, which allows the filter to combine input and output using `filterWeighted()` function.
`_weight` is ignored when using `filter()` function.
*/
template <typename T>
class BiquadFilterT : public FilterBaseT<T> {
public:
    BiquadFilterT(float a1, float a2, float b0, float b1, float b2) :
        _weight(1.0F),
        _a1(a1), _a2(a2),
        _b0(b0), _b1(b1), _b2(b2)
        {}
    BiquadFilterT() : BiquadFilterT(0.0F, 0.0F, 1.0F, 0.0F, 0.0F) {}
    struct state_t {
        T x1;
        T x2;
        T y1;
        T y2;
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
    inline void setParameters(const BiquadFilterT& other) {
        _weight = other._weight;
        _a1 = other._a1;
        _a2 = other._a2;
        _b0 = other._b0;
        _b1 = other._b1;
        _b2 = other._b2;
    }

    inline void reset() { _state.x1 = 0.0F; _state.x2 = 0.0F; _state.y1 = 0.0F; _state.y2 = 0.0F; }
    inline void setToPassthrough() { _b0 = 1.0F; _b1 = 0.0F; _b2 = 0.0F; _a1 = 0.0F; _a2 = 0.0F;  _weight = 1.0F; reset(); }

    inline T filter(const T& input) {
        const T output = _b0*input + _b1*_state.x1 + _b2*_state.x2 - _a1*_state.y1 - _a2*_state.y2;
        _state.x2 = _state.x1;
        _state.x1 = input;
        _state.y2 = _state.y1;
        _state.y1 = output;
        return output;
    }
    virtual T filterVirtual(const T& input) override { return filter(input); }

    inline T filterWeighted(const T& input) {
        const T output = filter(input);
        // weight of 1.0 gives just output, weight of 0.0 gives just input
        return _weight*(output - input) + input;
    }

    inline void initLowPass(float frequencyHz, float loopTimeSeconds, float Q) {
        assert(Q!=0.0F && "Q cannot be zero");
        setLoopTime(loopTimeSeconds);
        setQ(Q);
        setLowPassFrequency(frequencyHz);
        reset();
    }
    inline void initNotch(float frequencyHz, float loopTimeSeconds, float Q) {
        assert(Q!=0.0F && "Q cannot be zero");
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
    static constexpr float PI_F = static_cast<float>(M_PI);
};

/*!
Note: weight must be in range [0, 1].
*/
template <typename T>
inline void BiquadFilterT<T>::setLowPassFrequencyWeighted(float frequencyHz, float weight)
{
    _weight = weight;

    const float omega = frequencyHz*_2PiLoopTimeSeconds;
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
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
template <typename T>
inline void BiquadFilterT<T>::setNotchFrequencyWeighted(float frequencyHz, float weight)
{
    _weight = weight;

    const float omega = frequencyHz*_2PiLoopTimeSeconds;
    const float cosOmega = cosf(omega);
    const float alpha = sinf(omega)*_2Q_reciprocal;
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
template <typename T>
inline void BiquadFilterT<T>::setNotchFrequencyWeighted(float sinOmega, float two_cosOmega, float weight)
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
