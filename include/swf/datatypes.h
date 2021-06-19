#pragma once
#include "stream.h"

namespace swf {
/* SWF uses twips as unit. A twip is 1/20th of a screen's pixel (without scaling). */
constexpr unsigned int TWIPS = 20;

/* A coordinate in twips.
 */
struct Position {
    int32_t x;
    int32_t y;
};

/* Represent a rectangle region defined by a minimum x and y position and a maximum x and y position.
 */
struct Rect {
    Position min, max;

    Rect();

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    std::string toString();
};

/* Represent a 24-bit color with red (r), green (g) and blue (b) values.
 */
struct RGB {
    uint8_t r, g, b;

    RGB() : r(0), g(0), b(0) { }
    RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) { }

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    std::string toString();
};

/* Represent a  32-bit color with red (r), green (g), blue (b) and alpha (a) values.
   A value of 255 for the alpha means it is completly opaque while a value of 0 means it is completly transparent.
*/
struct RGBA : RGB {
    uint8_t a;

    RGBA() : RGB(), a(255) { }
    RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : RGB(r, g, b), a(a) { }

    void read(StreamReader& stream);
    void write(StreamWriter& stream);

    std::string toString();
};
}