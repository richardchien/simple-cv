//
// Created by Richard Chien on 6/20/16.
//

#include "types.h"

#ifndef SIMPLECV_CORE_H
#define SIMPLECV_CORE_H

#pragma mark - Make

ScvMat *scvCreateMat(int rows, int cols);

ScvMat *scvCloneMat(const ScvMat *mat);

void scvCopyMat(const ScvMat *src, ScvMat *dst);

void scvReleaseMat(ScvMat *mat);

ScvImage *scvCreateImage(ScvSize size);

ScvImage *scvCloneImage(const ScvImage *image);

void scvCopyImage(const ScvImage *src, ScvImage *dst);

void scvReleaseImage(ScvImage *image);

ScvHistogram *scvCreateHist(SCV_GRAYING_TYPE grayingType);

ScvHistogram *scvCloneHist(const ScvHistogram *histogram);

void scvCopyHist(const ScvHistogram *src, ScvHistogram *dst);

void scvReleaseHist(ScvHistogram *histogram);

#pragma mark - Getter and Setter

ScvPixel *scvGetPixelRef(const ScvImage *image, int x, int y);

ScvPixel scvGetPixel(const ScvImage *image, int x, int y);

void scvSetPixel(ScvImage *image, int x, int y, ScvPixel pixel);

ScvSize scvGetSize(const ScvImage *image);

#pragma mark - Calculator

void scvCalcHist(const ScvImage *image, ScvHistogram *hist);

#pragma mark - Geometrical Transformation

void scvWarpAffine(const ScvImage *src, ScvImage *dst, const ScvMat *mat, ScvPixel fillPxl);

void scvRotationMatrix(ScvPoint center, float angle, ScvMat *mat);

void scvScaleMatrix(ScvPoint center, float scaleX, float scaleY, ScvMat *mat);

void scvTranslationMatrix(float dx, float dy, ScvMat *mat);

#pragma mark - Point Transformation

void scvFillImage(ScvImage *image, ScvPixel fillPxl);

void scvGraying(ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE type);

void scvThreshold(ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE grayingType);

void scvSplit(ScvImage *src, ScvImage *b, ScvImage *g, ScvImage *r);

void scvInverse(ScvImage *src, ScvImage *dst);

#endif //SIMPLECV_CORE_H
