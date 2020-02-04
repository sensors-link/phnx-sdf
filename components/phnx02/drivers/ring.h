#ifndef __RING_H__
#define __RING_H__

#include <stdint.h>

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t size;
    volatile uint8_t data[];
} RingBuffer;

static inline void buf_reset(RingBuffer *buf, int size)
{
    buf->head = buf->tail = 0;
    buf->size = size;
}

static inline int buf_len(const RingBuffer *buf)
{
    int len = buf->tail - buf->head;
    if (len < 0)
        len += buf->size;
    
    return len;
}

static inline int buf_isfull(const RingBuffer *buf)
{
    return buf_len(buf) == (buf->size-1);
}

static inline int buf_isempty(const RingBuffer *buf)
{
    return buf->head == buf->tail;
}

static inline int advance(uint16_t i, uint16_t size)
{
    if (++i >= size)             // Avoid modulo or divide
        i = 0;

    return i;
}

static inline uint8_t buf_get_byte(RingBuffer *buf)
{
    const uint8_t item = buf->data[buf->head];
    buf->head = advance(buf->head, buf->size);
    return item;
}

static inline void buf_put_byte(RingBuffer *buf, uint8_t val)
{
    buf->data[buf->tail] = val;
    buf->tail = advance(buf->tail, buf->size);
}

#endif