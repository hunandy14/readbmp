/*****************************************************************
Name :
Date : 2017/11/10
By   : CharlotteHonG
Final: 2018/05/28
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "readbmp/readbmp.h"

void testImg(const char* inputName, const char* outputName, struct basic_ImgData* img){
    Imgraw_read(inputName, img);
    Imgraw_write(outputName, img);
}
/*==============================================================*/
int main(int argc, char const *argv[]) {
    struct basic_ImgData img;
    testImg("testImg/kanna1.bmp", "outImg/_kanna1.bmp", &img);
    testImg("testImg/kanna2.bmp", "outImg/_kanna2.bmp", &img);
    testImg("testImg/kanna3.bmp", "outImg/_kanna3.bmp", &img);
    testImg("testImg/kanna4.bmp", "outImg/_kanna4.bmp", &img);
    return 0;
}
/*==============================================================*/