#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "helper.h"

void LOAD_ASCII(IMAGE *im, char magic_aux[], FILE *in_file, int *photo);
void LOAD_BINARY(IMAGE *im, char magic_aux[], FILE *in_file, int *photo);
void LOAD(IMAGE *im, int *photo);
void SELECT(IMAGE *im, int photo);
void EQUALIZE(IMAGE *im, int photo);
void CROP_MATRIX(IMAGE *im);
void CROP_RGB(IMAGE *im);
void CROP(IMAGE *im, int photo);
void APPLY_EDGE(IMAGE *im);
void APPLY_SHARPEN(IMAGE *im);
void APPLY_BLUR(IMAGE *im);
void APPLY_GAUSSIAN_BLUR(IMAGE *im);
void APPLY(IMAGE *im, int photo);
void SAVE_ASCII(IMAGE *im, FILE *out_file);
void SAVE_BINARY(IMAGE *im, FILE *out_file);
void SAVE(IMAGE *im, int photo);
void EXIT(IMAGE *im, int photo);

#endif // FUNCTIONS_H