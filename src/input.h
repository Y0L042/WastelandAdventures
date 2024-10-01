#ifndef INPUT_H
#define INPUT_H

#include <raylib.h>

#define MAX_BUFFER_TIME 0.500 /* max buffered time in s */

typedef struct BufferKey {
    KeyboardKey key;
    double time_pressed;
    int is_buffered;
} BufferKey;

int is_key_buffered(int i);
void process_key_buffer();
void consume_key_buffer();

#endif /* INPUT_H */
