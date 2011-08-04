#include <stdint.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    uint16_t height;
    uint16_t width;
    uint8_t* data;
} Map;

void byteArrayToBitArray(uint8_t* bytes, uint32_t numBytes, uint8_t* bits);
void clearMap(Map* map);
void initMap(Map* map, uint16_t height, uint16_t width);
void printMap(Map* map);
uint8_t getVal(uint16_t x, uint16_t y, Map* map);
void setVal(uint16_t x, uint16_t y, uint8_t val, Map* map);
void drawGrid(Map* map);
void drawMap(Map* map);
uint8_t getVal_float(float x, float y, Map* map);
void setVal_float(float x, float y, uint8_t val, Map* map);

