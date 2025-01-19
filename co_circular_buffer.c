#include "co_circular_buffer.h"
#include "string.h"

static uint8_t *_buf;
static size_t _head; // always points to the next place to put data
static size_t _cap;
static size_t _size;

/**********************************************************************************************/
// private:

static void _clear()
{
    _head = 0;
    _size = 0;
}

static void _increase_head_and_size(size_t size)
{
    _head = _head + size;
    _head %= _cap;

    _size = _size + size;
    if (_size > _cap)
    {
        _size = _cap;
    }
}

static void _decrease_head_and_size(size_t size)
{
    if (_head < size)
    {
        _head += _cap;
    }
    _head -= size;
    _size -= size;
}

static size_t _get_empty_until_end()
{
    return _cap - _head;
}

/**********************************************************************************************/
// public:

size_t co_circular_buffer_get_empty()
{
    return _cap - _size;
}

size_t co_circular_buffer_get_size()
{
    return _size;
}

bool co_circular_buffer_is_empty()
{
    return (0 == _size);
}

bool co_circular_buffer_is_full()
{
    return (_cap == _size);
}

#ifdef CO_CIRCULAR_BUFFER_MANAGE_MEMORY
void co_circular_buffer_init(size_t buffer_capacity)
{
    _clear();
    _cap = buffer_capacity;
    _buf = malloc(buffer_capacity);
}

void co_circular_buffer_terminate()
{
    free(_buf);
}
#else
void co_circular_buffer_init(uint8_t *buffer, size_t buffer_capacity)
{
    _clear();
    _cap = buffer_capacity;
    _buf = buffer;
}

void co_circular_buffer_terminate()
{
}
#endif

void co_circular_buffer_push(uint8_t byte)
{
    _buf[_head] = byte;
    _increase_head_and_size(1);
}

void co_circular_buffer_push_arr(uint8_t *arr, size_t size)
{
    if (size < _cap)
    {
        const size_t empty_until_end = _get_empty_until_end();
        if (size < empty_until_end)
        {
            memcpy(_buf, arr, size);
        }
        else
        {
            memcpy(_buf + _head, arr, empty_until_end); // put first chunk towards the end

            const size_t size_after_beginning = size - empty_until_end;
            memcpy(_buf, arr + size_after_beginning, size_after_beginning); // put the rest from beginning
        }

        _increase_head_and_size(size);
    }
    else
    {
        memcpy(_buf, arr + size - _cap, _cap); // only get last _cap number of elements
        _head = 0;
        _size = _cap;
    }
}

size_t co_circular_buffer_pop(uint8_t *byte)
{
    size_t popped = 0;
    if (_size > 0)
    {
        popped = 1;
        _decrease_head_and_size(1);
        *byte = _buf[_head];
    }
    return popped;
}

size_t co_circular_buffer_pop_arr(uint8_t *buf, size_t size)
{
    size_t popped = (_size > size) ? size : _size;

    if ((0 == size) || (0 == _size))
    {
        // nothing to pop
    }
    if (_head > size)
    {
        memcpy(buf, _buf + _head - popped, popped);
    }
    else
    {
        const size_t section1 = popped - _head;
        memcpy(buf, _buf + _cap - section1, section1);
        memcpy(buf + section1, _buf, _head);
    }
    
    _decrease_head_and_size(popped);
    
    return popped;
}