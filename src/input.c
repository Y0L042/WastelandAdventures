#include "input.h"
#include <stdlib.h>

#define MAX_BUFFERED_KEYS 16
#define MAX_KEY_ID 500 /* Corresponds to the key values in raylib.h */

static BufferKey buffered_keys[MAX_KEY_ID];
static int consumed_keys[MAX_BUFFERED_KEYS];
static int consumed_keys_size = 0;

static void _consume_key(int key);

int is_key_buffered(int i)
{
    int x = buffered_keys[i].is_buffered;
    _consume_key(i);
    return x;
}

void process_key_buffer()
{
    double curr_time = GetTime();

    for (int i = 1; i < MAX_KEY_ID; i++)
    {
        if (buffered_keys[i].is_buffered)
        {
            if ((curr_time - buffered_keys[i].time_pressed) > MAX_BUFFER_TIME)
            {
                _consume_key(i);
            }
        }
    }

    for (int i = 1; i < MAX_KEY_ID; i++)
    {
        if (IsKeyPressed(i))
        {
            buffered_keys[i].is_buffered = 1;
            buffered_keys[i].time_pressed = curr_time;
        }
    }
}

void consume_key_buffer()
{
    for (int i = 0; i < consumed_keys_size; i++)
    {
        int idx = consumed_keys[i];
        consumed_keys[i] = -1;

        buffered_keys[idx].is_buffered = 0;
        buffered_keys[idx].time_pressed = 0.0;
    }
    consumed_keys_size = 0;
}

static void _consume_key(int key)
{
    consumed_keys[consumed_keys_size++] = key;
}
