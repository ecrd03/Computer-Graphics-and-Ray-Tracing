#ifndef CANVAS
#define CANVAS

#include "../tuple/tuple.h"

/* canvas related API */

typedef struct canvas {
    unsigned int channels;
    unsigned int width;
    unsigned int height;
    unsigned int* pixels;
} Canvas;

Canvas canvas(unsigned int, unsigned int);
Canvas write_pixel(Canvas, unsigned int, unsigned int, Color);
Color pixel_at(Canvas, unsigned int, unsigned int);
char* canvas_to_ppm(Canvas);

#endif