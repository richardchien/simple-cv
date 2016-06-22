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

ScvPoint scvGetCenter(const ScvImage *image);

#pragma mark - Calculator

void scvCalcHist(const ScvImage *image, ScvHistogram *hist);

#pragma mark - Geometrical Transformation

void scvWarpAffine(const ScvImage *src, ScvImage *dst, const ScvMat *mat, ScvPixel fillPxl);

void scvRotationMatrix(ScvPoint center, float angle, ScvMat *mat);

void scvScaleMatrix(ScvPoint center, float scaleX, float scaleY, ScvMat *mat);

void scvTranslationMatrix(float dx, float dy, ScvMat *mat);

typedef enum {
    SCV_FLIP_HORIZONTAL,
    SCV_FLIP_VERTICAL
} SCV_FLIP_TYPE;

void scvFlipMatrix(ScvPoint center, SCV_FLIP_TYPE type, ScvMat *mat);

#pragma mark - Point Transformation

void scvFillImage(ScvImage *image, ScvPixel fillPxl);

void scvGraying(const ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE type);

/**
 * Applies fixed-level threshold to gray-scale image.
 * If the image is not gray-scaled, graying it with the parameter grayingType.
 */
void scvThreshold(const ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE grayingType);

void scvSplit(const ScvImage *src, ScvImage *b, ScvImage *g, ScvImage *r);

void scvInverse(const ScvImage *src, ScvImage *dst);

void scvEqualizeHist(const ScvImage *src, ScvImage *dst, const ScvHistogram *hist);

typedef enum {
    SCV_FILTER_AVG,
    SCV_FILTER_MED
} SCV_FILTER_TYPE;

void scvFilter(const ScvImage *src, ScvImage *dst, SCV_FILTER_TYPE type);

#endif //SIMPLECV_CORE_H
