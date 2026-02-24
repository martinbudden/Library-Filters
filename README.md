![license](https://img.shields.io/badge/license-MIT-green) ![open source](https://badgen.net/badge/open/source/blue?icon=github)

# Filters Library

This library contains a collection of filter classes. They have been developed for use in a Self Balancing Robot and have been used to:

1. Filter gyro and accelerometer output for use in the Attitude and Heading Reference System (AHRS).
2. Filter motor encoder values for use in the motor controller.
3. Filter derivative terms in a PID controller.
4. Filter motor power input values to smooth the motor speed.

## Class Diagram

The filters are somewhat statically (build-time) polymorphic, and somewhat dynamically (run-time) polymorphic.<br>
This is deliberate.<br>

The filters have functions that have names and signatures in common, but the only virtual function is `filter_virtual`.

The filter function may called directly by calling `filter`, or indirectly (via the vtable) using `filter_virtual`.

This means the the filters are somewhat interchangeable at build time, depending on which functions are used.

```mermaid
classDiagram
    class FilterNull {
        init(float k)
        reset()
        set_to_passthrough()
        set_cutoff_frequency(float cutoffFrequency, float dt)
        set_cutoff_frequency_and_reset(float cutoffFrequency, float dt)
        filter(float input) float
        filter(float input, float dt) float
    }
```

```mermaid
classDiagram
    class FilterMovingAverage~N~ {
        reset()
        filter(float input) float
        filter(float input, float dt) float
    }
```

```mermaid
classDiagram
    class ButterWorthFilter {
        set_parameters(const ButterWorthFilter& other)
        reset()
        set_to_passthrough()
        filter(float input) float
    }
```

```mermaid
classDiagram
    class IIR_filter {
        init(float alpha)
        setAlpha(float alpha)
        reset()
        set_to_passthrough()
        set_cutoff_frequency(float cutoffFrequency, float dt)
        set_cutoff_frequency_and_reset(float cutoffFrequency, float dt)
        filter(float input) float
        filter(float input, float dt) float
    }
```

```mermaid
classDiagram
    class PowerTransferFilter1 {
        init(float k)
        reset()
        set_to_passthrough()
        set_cutoff_frequency(float cutoffFrequency, float dt)
        set_cutoff_frequency_and_reset(float cutoffFrequency, float dt)
        filter(float input) float
        gain_from_delay(float delay, float dt) float $
        gain(float cutoffFrequency, float dt) float $
    }

    class PowerTransferFilter2 {
        init(float k)
        reset()
        set_to_passthrough()
        set_cutoff_frequency(float cutoffFrequency, float dt)
        set_cutoff_frequency_and_reset(float cutoffFrequency, float dt)
        filter(float input) float
        gain_from_delay(float delay, float dt) float $
        gain(float cutoffFrequency, float dt) float $
    }

    class PowerTransferFilter3 {
        init(float k)
        reset()
        set_to_passthrough()
        set_cutoff_frequency(float cutoffFrequency, float dt)
        set_cutoff_frequency_and_reset(float cutoffFrequency, float dt)
        filter(float input) float
        gain_from_delay(float delay, float dt) float $
        gain(float cutoffFrequency, float dt) float $
    }
```

```mermaid
classDiagram
    class IIR_filter {
        set_weight(float weight)
        set_parameters(float a1, float a2, float b0, float b1, float b2, float weight)
        set_parameters(float a1, float a2, float b0, float b1, float b2)
        set_parameters(const BiquadFilter& other)

        reset()
        set_to_passthrough()

        filter(float input) float
        filter_weighted(float input) float

        init_notch(float frequency, float looptime_seconds, float Q)

        calculate_omega(float frequency) float
        set_low_pass_frequency(float frequency, float weight)
        set_notch_frequency(float frequency, float weight)
        set_notch_frequency(float sin_omega, float two_cos_omega, float weight)

        calculateQ(float centerFrequency, float lower_cutoff_frequency) float $
        setQ(float centerFrequency, float lower_cutoff_frequency)
        setQ(float Q)
        getQ() float
        set_looptime(float looptime_seconds)
    }
```
