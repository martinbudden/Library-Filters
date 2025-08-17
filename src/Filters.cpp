#include "Filters.h"

float FilterNull::filterVirtual(float input)
{
    return filter(input);
}

template <size_t N>
float FilterMovingAverage<N>::filterVirtual(float input)
{
    return filter(input);
}

template <size_t N>
float FIR_filter<N>::filterVirtual(float input)
{
    return filter(input);
}

float ButterWorthFilter::filterVirtual(float input)
{
    return filter(input);
}

float IIR_filter::filterVirtual(float input)
{
    return filter(input);
}

float PowerTransferFilter1::filterVirtual(float input)
{
    return filter(input);
}

float PowerTransferFilter2::filterVirtual(float input)
{
    return filter(input);
}

float PowerTransferFilter3::filterVirtual(float input)
{
    return filter(input);
}

float BiquadFilter::filterVirtual(float input)
{
    return filter(input);
}
