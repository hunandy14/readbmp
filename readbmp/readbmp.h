#ifndef READBMP_H
#define READBMP_H
#ifdef  __cplusplus
extern  "C" {
#endif


#include <inttypes.h>

struct basic_ImgData {
    uint32_t width, height;
    uint16_t bits;
    unsigned char* data;
};

void ImgData_read(const char* inputName, struct basic_ImgData* img);
void ImgData_write(const char* outputName, const struct basic_ImgData* img);



#ifdef  __cplusplus
}
#endif
#endif // READBMP_H