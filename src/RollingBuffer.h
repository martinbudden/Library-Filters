#pragma once

#include <array>
#include <cstddef>
#include <cstring>


/*!
Static rolling buffer of type T and capacity C.
Items are pushed on the back and, once the buffer is full, items just fall off the front.
*/
template <typename T, size_t C>
class RollingBuffer {
public:
    RollingBuffer() : _begin(0), _end(0), _size(0) {}
private:
    enum { CAPACITY = C };
public:
    inline size_t size() const { return _size; }
    inline bool isEmpty() const { return _size == 0; }
    void pushBack(const T& value);
    inline const T& operator[](size_t index) const {
        size_t pos = _begin + index;
        if (pos > capacity()) {
            pos -= capacity() + 1;
        }
        return _buffer[pos];
    }
    inline const T& front() const { return _buffer[_begin]; }
    inline const T& back() const { return _end > 0 ? _buffer[_end - 1] : _buffer[CAPACITY]; }
    inline void copy(std::array<T, C>& dest) const {
        if (_end >= _begin) {
            memcpy(&dest[0], &_buffer[_begin], (_end - _begin) * sizeof(T));
        } else {
            memcpy(&dest[0], &_buffer[_begin], (CAPACITY + 1 - _begin) * sizeof(T));
            memcpy(&dest[CAPACITY + 1 - _begin], &_buffer[0], _end * sizeof(T));
        }
    }
    inline size_t getBegin() { return _begin; }
    inline size_t getEnd() { return _end; }
    
    inline size_t capacity() const { return CAPACITY; }

    class Iterator {
    public:
        Iterator(const RollingBuffer& rb, size_t pos) : _rb(rb), _pos(pos) {}
        inline const T& operator*() const { return _rb._buffer[_pos]; }
        inline const T* operator->() const { return &_rb._buffer[_pos]; }
        inline Iterator& operator++() { ++_pos; if (_pos > _rb._size) _pos = 0; return *this; }
        inline bool operator!=(const Iterator& other) const { return _pos != other._pos || &_rb != &other._rb; }
        size_t pos() const { return _pos; }
    private:
        const RollingBuffer& _rb;
        size_t _pos;
    };
    const Iterator begin() const { return Iterator(*this, _begin); }
    const Iterator end() const { return Iterator(*this, _end); }
private:
    size_t _begin; //!< The virtual beginning of the rolling buffer.
    size_t _end;   //!< The virtual end of the rolling buffer (one behind the last element).
    size_t _size;  //!< The number of items in the rolling buffer.
    std::array<T, CAPACITY + 1> _buffer {}; // need one spare empty cell so we can avoid _end == _begin when full
};

template <typename T, size_t C>
void RollingBuffer<T, C>::pushBack(const T& value)
{
    _buffer[_end] = value; // sizeof(_buffer) = CAPACITY + 1, so always OK to store value at _end
    ++_end;

    if (_size >= capacity()) {//[[likely]]
        // buffer is full, so don't increment size, instead drop items off front by incrementing _begin
        ++_begin;
        // wrap _begin if required
        if (_begin > capacity()) {
            _begin = 0;
        }
        // wrap _end if required
        if (_end > capacity()) {
            _end = 0;
        }
    } else {
        ++_size;
    }
}

/*!
Static rolling buffer of type T and capacity C.
Items are pushed on the back and, once the buffer is full, items just fall off the front.
Maintains sum of items in buffer.
*/
template <typename T, size_t C>
class RollingBufferWithSum {
public:
    RollingBufferWithSum() : _begin(0), _end(0), _size(0) {}
private:
    enum { CAPACITY = C };
public:
    inline size_t size() const { return _size; }
    void pushBack(const T& value);
    inline const T& operator[](size_t index) const {
        size_t pos = _begin + index;
        if (pos > capacity()) {
            pos -= capacity() + 1;
        }
        return _buffer[pos];
    }
    inline const T& front() const { return _buffer[_begin]; }
    inline const T& back() const { return _end > 0 ? _buffer[_end - 1] : _buffer[CAPACITY]; }
    inline void copy(std::array<T, C>& dest) const {
        if (_end >= _begin) {
            memcpy(&dest[0], &_buffer[_begin], (_end - _begin) * sizeof(T));
        } else {
            memcpy(&dest[0], &_buffer[_begin], (CAPACITY + 1 - _begin) * sizeof(T));
            memcpy(&dest[CAPACITY + 1 - _begin], &_buffer[0], _end * sizeof(T));
        }
    }
    inline size_t capacity() const { return CAPACITY; }
    inline T sum() const { return _sum; }
    T recalculateSum();

    class Iterator {
    public:
        Iterator(const RollingBufferWithSum& rb, size_t pos) : _rb(rb), _pos(pos) {}
        inline const T& operator*() const { return _rb._buffer[_pos]; }
        inline const T* operator->() const { return &_rb._buffer[_pos]; }
        inline Iterator& operator++() { ++_pos; if (_pos > _rb._size) _pos = 0; return *this; }
        inline bool operator!=(const Iterator& other) const { return _pos != other._pos || &_rb != &other._rb; }
        size_t pos() const { return _pos; }
    private:
        const RollingBufferWithSum& _rb;
        size_t _pos;
    };
    const Iterator begin() const { return Iterator(*this, _begin); }
    const Iterator end() const { return Iterator(*this, _end); }
private:
    size_t _begin; //!< The virtual beginning of the rolling buffer.
    size_t _end;   //!< The virtual end of the rolling buffer (one behind the last element).
    size_t _size;  //!< The number of items in the rolling buffer.
    T _sum {};
    std::array<T, CAPACITY + 1> _buffer {}; // need one spare empty cell so we can avoid _end == _begin when full
};

template <typename T, size_t C>
void RollingBufferWithSum<T, C>::pushBack(const T& value)
{
    _sum += value;
    _buffer[_end] = value; // sizeof(_buffer) = CAPACITY + 1, so always OK to store value at _end
    ++_end;

    if (_size >= capacity()) {//[[likely]]
        // buffer is full, so don't increment size, instead drop items off front by incrementing _begin
        _sum -= _buffer[_begin];
        ++_begin;
        // wrap _begin if required
        if (_begin > capacity()) {
            _begin = 0;
        }
        // wrap _end if required
        if (_end > capacity()) {
            _end = 0;
        }
    } else {
        ++_size;
    }
}

template <typename T, size_t C>
T RollingBufferWithSum<T, C>::recalculateSum()
{
    _sum = 0;
    for (auto it = begin(); it != end(); ++it) {
        _sum += *it;
    }
    return _sum;
}
