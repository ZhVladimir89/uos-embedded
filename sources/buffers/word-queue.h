#ifndef _WORD_QUEUE_H_
#define _WORD_QUEUE_H_

typedef struct _wq_t
{
    unsigned long * array;
    unsigned long   mask;
    unsigned long   puti;
    unsigned long   geti;
} wq_t;

static inline int ones_count (unsigned long value)
{
    int count = 0;
    do {
        if (value & 1)
            count++;
        value >>= 1;
    } while (value != 0);
    
    return count;
}

static inline void wq_init (wq_t *wq, unsigned long *buffer, unsigned long size)
{
    assert ((ones_count (size) == 1) && (size > 1));
    
    wq->array = buffer;
    wq->mask = size - 1;
    wq->puti = 0;
    wq->geti = 0;    
}

static inline void wq_put (wq_t *wq, unsigned long word)
{
    wq->array[wq->puti] = word;
    wq->puti = (wq->puti + 1) & wq->mask;
}

static inline unsigned long wq_get (wq_t *wq)
{
    uint8_t word;
    word = wq->array[wq->geti];
    wq->geti = (wq->geti + 1) & wq->mask;
    return word;
}

static inline int wq_is_empty(wq_t *wq)
{
    return wq->puti == wq->geti;
}

static inline int wq_is_full(wq_t *wq)
{
    return ((wq->puti + 1) & wq->mask) == wq->geti;
}

static inline void wq_clear(wq_t *wq)
{
    wq->puti = 0;
    wq->geti = 0;
}

#endif // _WORD_QUEUE_H_
