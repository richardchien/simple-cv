//
// Created by Richard Chien on 6/21/16.
//

#include "types.h"

#ifndef SIMPLECV_MATRIX_H
#define SIMPLECV_MATRIX_H

float scvMatGetVal(const ScvMat *mat, int i, int j);

void scvMatSetVal(ScvMat *mat, int i, int j, float value);

void scvMatDotProduct(const ScvMat *left, const ScvMat *right, ScvMat *dst);

void scvMatNumProduct(float k, const ScvMat *mat, ScvMat *dst);

void scvMatInverse(const ScvMat *src, ScvMat *dst);

float scvMatDet(const ScvMat *mat);

void scvMatMinor(const ScvMat *src, ScvMat *dst, int i, int j);

void scvMatAdjugate(const ScvMat *src, ScvMat *dst);

void scvMatTranspose(const ScvMat *src, ScvMat *dst);

#endif //SIMPLECV_MATRIX_H
