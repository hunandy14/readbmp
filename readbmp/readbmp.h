#ifndef READBMP_H
#define READBMP_H


#include <inttypes.h>

struct basic_ImgData {
    uint32_t width, height;
    uint16_t bits;
    unsigned char* data;
};

void Imgraw_read(const char* inputName, struct basic_ImgData* img);
void Imgraw_write(const char* outputName, const struct basic_ImgData* img);


#endif // READBMP_H