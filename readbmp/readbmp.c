#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "readbmp.h"
typedef unsigned char uch;

// 檔案結構
#pragma pack(2)
struct BmpFileHeader {
    uint16_t bfTybe;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
struct BmpInfoHeader {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes; // 1=defeaul, 0=custom
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter; // 72dpi=2835, 96dpi=3780
    uint32_t biYPelsPerMeter; // 120dpi=4724, 300dpi=11811
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack()

void bmpWrite(const char* name, const unsigned char* raw_img,
    uint32_t width, uint32_t height, uint16_t bits)
{
    if(!(name && raw_img)) {
        perror("Error bmpWrite.");
        return;
    }
    // 檔案資訊
    struct BmpFileHeader file_h = {
        .bfTybe=0x4d42,
        .bfReserved1=0,
        .bfReserved2=0,
        .bfOffBits=54,
    };
    file_h.bfSize = file_h.bfOffBits + width*height * bits/8;
    if(bits==8) {file_h.bfSize += 1024, file_h.bfOffBits += 1024;}
    // 圖片資訊
    struct BmpInfoHeader info_h = {
        .biSize=40,
        .biPlanes=1,
        .biCompression=0,
        .biXPelsPerMeter=0,
        .biYPelsPerMeter=0,
        .biClrUsed=0,
        .biClrImportant=0,
    };
    info_h.biWidth = width;
    info_h.biHeight = height;
    info_h.biBitCount = bits;
    info_h.biSizeImage = width*height * bits/8;
    if(bits == 8) {info_h.biClrUsed=256;}
    // 寫入檔頭
    FILE *pFile = fopen(name,"wb+");
    if(!pFile) {
        perror("Error opening file.");
        return;
    }
    fwrite((char*)&file_h, sizeof(char), sizeof(file_h), pFile);
    fwrite((char*)&info_h, sizeof(char), sizeof(info_h), pFile);
    // 寫調色盤
    if(bits == 8) {
        for(unsigned i = 0; i < 256; ++i) {
            uch c = i;
            fwrite((char*)&c, sizeof(char), sizeof(uch), pFile);
            fwrite((char*)&c, sizeof(char), sizeof(uch), pFile);
            fwrite((char*)&c, sizeof(char), sizeof(uch), pFile);
            fwrite("", sizeof(char), sizeof(uch), pFile);
        }
    }
    // 寫入圖片資訊
    size_t alig = ((width*bits/8)*3) % 4;
    for(int j = height-1; j >= 0; --j) {
        for(unsigned i = 0; i < width; ++i) {
            if(bits == 24) {
                fwrite((char*)&raw_img[(j*width+i)*3 + 2], sizeof(char), sizeof(uch), pFile);
                fwrite((char*)&raw_img[(j*width+i)*3 + 1], sizeof(char), sizeof(uch), pFile);
                fwrite((char*)&raw_img[(j*width+i)*3 + 0], sizeof(char), sizeof(uch), pFile);
            } else if(bits == 8) {
                fwrite((char*)&raw_img[j*width+i], sizeof(char), sizeof(uch), pFile);
            }
        }
        // 對齊4byte
        for(size_t i = 0; i < alig; ++i) {
            fwrite("", sizeof(char), sizeof(uch), pFile);
        }
    }
    fclose(pFile);
}
void bmpRead(const char* name, unsigned char** raw_img,
    uint32_t* width, uint32_t* height, uint16_t* bits)
{
    if(!(name && raw_img && width && height && bits)) {
        perror("Error bmpRead.");
        return;
    }
    // 檔案資訊
    struct BmpFileHeader file_h;
    // 圖片資訊
    struct BmpInfoHeader info_h;
    // 讀取檔頭
    FILE *pFile = fopen(name,"rb+");
    if(!pFile) {
        perror("Error opening file.");
        return;
    }
    fread((char*)&file_h, sizeof(char), sizeof(file_h), pFile);
    fread((char*)&info_h, sizeof(char), sizeof(info_h), pFile);
    // 讀取長寬
    *width = info_h.biWidth;
    *height = info_h.biHeight;
    *bits = info_h.biBitCount;
    *raw_img = (uch*)calloc((info_h.biWidth)*(info_h.biHeight)*3, sizeof(uch));
    // 讀取讀片資訊轉RAW檔資訊
    fseek(pFile, file_h.bfOffBits, SEEK_SET);// 修正資料開始處
    size_t alig = ((info_h.biWidth*info_h.biBitCount/8)*3) % 4;
    for(int j = *height-1; j >= 0; --j) {
        for(unsigned i = 0; i < *width; ++i) {
            if(*bits == 24) {
                fread((char*)&(*raw_img)[(j*(*width)+i)*3 + 2], sizeof(char), sizeof(uch), pFile);
                fread((char*)&(*raw_img)[(j*(*width)+i)*3 + 1], sizeof(char), sizeof(uch), pFile);
                fread((char*)&(*raw_img)[(j*(*width)+i)*3 + 0], sizeof(char), sizeof(uch), pFile);
            } else if(*bits == 8) {
                fread((char*)&(*raw_img)[j*(*width)+i], sizeof(char), sizeof(uch), pFile);
            }
        }
        fseek(pFile , alig , SEEK_CUR);
    }
    fclose(pFile);
}

// 封裝檔案
void ImgData_read(const char* inputName, struct basic_ImgData* img)
{
    if (inputName!=NULL && img!=NULL){
        bmpRead(inputName, &img->data, &img->width, &img->height, &img->bits);
    } else {
        printf("Error!");
    }
}
void ImgData_write(const char* outputName, const struct basic_ImgData* img)
{
    if (outputName!=NULL && img!=NULL){
        bmpWrite(outputName, img->data, img->width, img->height, img->bits);
    } else {
        printf("Error!");
    }
}

