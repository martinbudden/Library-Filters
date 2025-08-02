![license](https://img.shields.io/badge/license-MIT-green) ![open source](https://badgen.net/badge/open/source/blue?icon=github)

# Filters Library

This library contains a collection of filter classes. They have been developed for use in a Self Balancing Robot and have been used to:

1. Filter gyro and accelerometer output for use in the Attitude and Heading Reference System (AHRS).
2. Filter motor encoder values for use in the motor controller.
3. Filter derivative terms in a PID controller.
4. Filter motor power input values to smooth the motor speed.

## Class Diagram

The filters are somewhat statically (build-time) polymorphic, but not dynamically (run-time) polymorphic.<br>
This is deliberate.<br>
The filters have functions that have names and signatures in common, but these functions are not virtual.<br>
This means the the filters are somewhat interchangeable at build time, depending on which functions are used

```mermaid
classDiagram
    class FilterNull {
        init(float k)
        reset()
        setToPassthrough()
        setCutoffFrequency(float cutoffFrequency, float dT)
        setCutoffFrequencyAndReset(float cutoffFrequency, float dT)
        filter(float input) float
        filter(float input, float dT) float
    }
```

```mermaid
classDiagram
    class FilterMovingAverage~N~ {
        reset()
        filter(float input) float
        filter(float input, float dT) float
    }
```

```mermaid
classDiagram
    class ButterWorthFilter {
        setParameters(const ButterWorthFilter& other)
        reset()
        setToPassthrough()
        filter(float input) float
    }
```

```mermaid
classDiagram
    class IIR_filter {
        init(float alpha)
        setAlpha(float alpha)
        reset()
        setToPassthrough()
        setCutoffFrequency(float cutoffFrequency, float dT)
        setCutoffFrequencyAndReset(float cutoffFrequency, float dT)
        filter(float input) float
        filter(float input, float dT) float
    }
```

```mermaid
classDiagram
    class PowerTransferFilter1 {
        init(float k)
        reset()
        setToPassthrough()
        setCutoffFrequency(float cutoffFrequency, float dT)
        setCutoffFrequencyAndReset(float cutoffFrequency, float dT)
        filter(float input) float
        gainFromDelay(float delay, float dT) float $
        gain(float cutoffFrequency, float dT) float $
    }

    class PowerTransferFilter2 {
        init(float k)
        reset()
        setToPassthrough()
        setCutoffFrequency(float cutoffFrequency, float dT)
        setCutoffFrequencyAndReset(float cutoffFrequency, float dT)
        filter(float input) float
        gainFromDelay(float delay, float dT) float $
        gain(float cutoffFrequency, float dT) float $
    }

    class PowerTransferFilter3 {
        init(float k)
        reset()
        setToPassthrough()
        setCutoffFrequency(float cutoffFrequency, float dT)
        setCutoffFrequencyAndReset(float cutoffFrequency, float dT)
        filter(float input) float
        gainFromDelay(float delay, float dT) float $
        gain(float cutoffFrequency, float dT) float $
    }
```

```mermaid
classDiagram
    class IIR_filter {
        setWeight(float weight)
        setParameters(float a1, float a2, float b0, float b1, float b2, float weight)
        setParameters(float a1, float a2, float b0, float b1, float b2)
        setParameters(const BiquadFilter& other)

        reset()
        setToPassthrough()

        filter(float input) float
        filterWeighted(float input) float

        initNotch(float frequency, uint32_t loopTimeUs, float Q)

        calculateOmega(float frequency) float
        setLowPassFrequency(float frequency, float weight)
        setNotchFrequency(float frequency, float weight)
        setNotchFrequency(float sinOmega, float two_cosOmega, float weight)

        calculateQ(float centerFrequency, float lowerCutoffFrequency) float $
        setQ(float centerFrequency, float lowerCutoffFrequency)
        setQ(float Q)
        getQ() float
        setLoopTime(uint32_t loopTimeUs)
    }
```
