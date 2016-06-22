//
// Created by Richard Chien on 6/20/16.
//

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "core.h"
#include "matrix.h"

#pragma mark - Inner

int max(int a, int b, int c) {
    int m = a;
    m = b > m ? b : m;
    m = c > m ? c : m;
    return m;
}

float avg(int a, int b, int c) {
    return (a + b + c) / 3.0f;
}

float avgWeighted(int r, int g, int b) {
    return 0.30f * r + 0.59f * g + 0.11f * b;
}

int grayValueOfPixel(ScvPixel pxl, SCV_GRAYING_TYPE type) {
    switch (type) {
        case SCV_GRAYING_R:
            return pxl.r;
            break;
        case SCV_GRAYING_G:
            return pxl.g;
            break;
        case SCV_GRAYING_B:
            return pxl.b;
            break;
        case SCV_GRAYING_MAX:
            return max(pxl.b, pxl.g, pxl.r);
            break;
        case SCV_GRAYING_AVG:
            return (int) avg(pxl.r, pxl.g, pxl.b);
            break;
        case SCV_GRAYING_W_AVG:
            return (int) avgWeighted(pxl.r, pxl.g, pxl.b);
            break;
        default:
            return -1;
            break;
    }
}

float thresholdOtsu(ScvHistogram *hist, int total) {
    // https://en.wikipedia.org/wiki/Otsu%27s_method
    int sum = 0;
    for (int i = 1; i < 256; ++i)
        sum += i * hist->val[i];
    int sumB = 0;
    int wB = 0;
    int wF = 0;
    int mB;
    int mF;
    float max = 0.0;
    float between = 0.0;
    float threshold1 = 0.0;
    float threshold2 = 0.0;
    for (int i = 0; i < 256; ++i) {
        wB += hist->val[i];
        if (wB == 0)
            continue;
        wF = total - wB;
        if (wF == 0)
            break;
        sumB += i * hist->val[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * (mB - mF) * (mB - mF);
        if (between >= max) {
            threshold1 = i;
            if (between > max) {
                threshold2 = i;
            }
            max = between;
        }
    }
    return (threshold1 + threshold2) / 2.0f;
}

#pragma mark - Export

#pragma mark -- Make

ScvImage *scvCreateImage(ScvSize size) {
    ScvImage *image = malloc(sizeof(ScvImage));
    image->origin = 0;
    image->width = size.width;
    image->height = size.height;
    const int realWidthBytes = image->width * 3;
    image->widthBytes = realWidthBytes % 4 ? ((realWidthBytes >> 2) + 1) << 2 : realWidthBytes;
    const int dataSize = image->widthBytes * image->height;
    image->data = malloc((size_t) dataSize);
    memset(image->data, 0, dataSize);
    return image;
}

ScvImage *scvCloneImage(const ScvImage *image) {
    ScvImage *result = scvCreateImage(scvGetSize(image));
    scvCopyImage(image, result);
    return result;
}

void scvCopyImage(const ScvImage *src, ScvImage *dst) {
    dst->origin = src->origin;
    dst->width = src->width;
    dst->height = src->height;
    dst->widthBytes = src->widthBytes;
    memcpy(dst->data, src->data, src->widthBytes * src->height);
}

void scvReleaseImage(ScvImage *image) {
    free(image->data);
    free(image);
}

ScvMat *scvCreateMat(int rows, int cols) {
    ScvMat *mat = malloc(sizeof(ScvMat));
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(rows * cols * sizeof(float));
    return mat;
}

ScvMat *scvCloneMat(const ScvMat *mat) {
    ScvMat *result = scvCreateMat(mat->rows, mat->cols);
    scvCopyMat(mat, result);
    return result;
}

void scvCopyMat(const ScvMat *src, ScvMat *dst) {
    dst->rows = src->rows;
    dst->cols = src->cols;
    for (int i = 0; i < src->rows * src->cols; i++) {
        dst->data[i] = src->data[i];
    }
}

void scvReleaseMat(ScvMat *mat) {
    free(mat->data);
    free(mat);
}

ScvHistogram *scvCreateHist(SCV_GRAYING_TYPE grayingType) {
    ScvHistogram *histogram = malloc(sizeof(ScvHistogram));
    histogram->grayingType = grayingType;
    histogram->val = malloc(256 * sizeof(int));
    memset(histogram->val, 0, 256 * sizeof(int));
    return histogram;
}

ScvHistogram *scvCloneHist(const ScvHistogram *histogram) {
    ScvHistogram *result = scvCreateHist(histogram->grayingType);
    scvCopyHist(histogram, result);
    return result;
}

void scvCopyHist(const ScvHistogram *src, ScvHistogram *dst) {
    dst->grayingType = src->grayingType;
    memset(dst->val, src->val, 256 * sizeof(int));
}

void scvReleaseHist(ScvHistogram *histogram) {
    free(histogram->val);
    free(histogram);
}

#pragma mark -- Getter and Setter

ScvPixel *scvGetPixelRef(const ScvImage *image, int x, int y) {
    const int w = image->width;
    const int h = image->height;
    const int bW = image->widthBytes;
    const int o = image->origin;

    if (x < 0 || x >= w || y < 0 || y >= h) {
        return NULL;
    }

    // Offer different offset by image.origin
    return image->data + (o ? h - 1 - y : y) * bW + x * 3;
}

ScvPixel scvGetPixel(const ScvImage *image, int x, int y) {
    ScvPixel pixel = {0};

    ScvPixel *tmpPxl = scvGetPixelRef(image, x, y);
    if (NULL == tmpPxl) {
        return pixel;
    }

    pixel.b = tmpPxl->b;
    pixel.g = tmpPxl->g;
    pixel.r = tmpPxl->r;
    return pixel;
}

void scvSetPixel(ScvImage *image, int x, int y, ScvPixel pixel) {
    ScvPixel *tmpPxl = scvGetPixelRef(image, x, y);
    if (NULL == tmpPxl) {
        return;
    }

    tmpPxl->b = pixel.b;
    tmpPxl->g = pixel.g;
    tmpPxl->r = pixel.r;
}

ScvSize scvGetSize(const ScvImage *image) {
    ScvSize size;
    size.width = image->width;
    size.height = image->height;
    return size;
}

#pragma mark -- Calculator

void scvCalcHist(const ScvImage *image, ScvHistogram *hist) {
    memset(hist->val, 0, 256 * sizeof(int));
    for (int iy = 0; iy < image->height; iy++) {
        for (int ix = 0; ix < image->width; ix++) {
            ScvPixel pxl = scvGetPixel(image, ix, iy);
            int index = grayValueOfPixel(pxl, hist->grayingType);
            if (index >= 0 && index < 256) {
                hist->val[index]++;
            }
        }
    }
}

#pragma mark -- Geometrical Transformation

void scvWarpAffine(const ScvImage *src, ScvImage *dst, const ScvMat *mat, ScvPixel fillPxl) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    int cloned = 0;
    if (src == dst) {
        src = scvCloneImage(dst);
        cloned = 1;
    }

    scvFillImage(dst, fillPxl);

    float m[9];
    for (int i = 0; i < 6; i++) {
        m[i] = mat->data[i];
    }
    ScvMat stdMat = scvMat(3, 3, m);
    scvMatSetVal(&stdMat, 2, 0, 0.0f);
    scvMatSetVal(&stdMat, 2, 1, 0.0f);
    scvMatSetVal(&stdMat, 2, 2, 1.0f);

    float mInv[9];
    ScvMat stdMatInv = scvMat(3, 3, mInv);
    scvMatInverse(&stdMat, &stdMatInv);

    float pNew[3];
    ScvMat stdPntNew = scvMat(3, 1, pNew);
    scvMatSetVal(&stdPntNew, 2, 0, 1.0f);

    float pOrig[3];
    ScvMat stdPntOrig = scvMat(3, 1, pOrig);

    // Traverse the destination image
    for (int iy = 0; iy < dst->height; iy++) {
        for (int ix = 0; ix < dst->width; ix++) {
            /**
             * Calculate the origin point of
             * the current point on new image,
             * and fill the empty pixels using
             * Nearest Neighbor Interpolation algorithm.
             */

            scvMatSetVal(&stdPntNew, 0, 0, ix + 0.5f);
            scvMatSetVal(&stdPntNew, 1, 0, iy + 0.5f);

            scvMatDotProduct(&stdMatInv, &stdPntNew, &stdPntOrig);
            float fxO = scvMatGetVal(&stdPntOrig, 0, 0);
            float fyO = scvMatGetVal(&stdPntOrig, 1, 0);

            fxO += fxO > 0 ? 0 : -0.5f;
            fyO += fyO > 0 ? 0 : -0.5f;

            scvSetPixel(dst, ix, iy, scvGetPixel(src, (int) fxO, (int) fyO));
        }
    }

    if (cloned) {
        scvReleaseImage((ScvImage *) src);
    }
}

void scvRotationMatrix(ScvPoint center, float angle, ScvMat *mat) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    const float rad = (float) (angle / 180 * M_PI);
    const float cos = cosf(rad);
    const float sin = sinf(rad);
    const int cx = center.x;
    const int cy = center.y;

    scvMatSetVal(mat, 0, 0, cos);
    scvMatSetVal(mat, 0, 1, sin);
    scvMatSetVal(mat, 0, 2, cos * (-cx) + sin * (-cy) + cx);
    scvMatSetVal(mat, 1, 0, -sin);
    scvMatSetVal(mat, 1, 1, cos);
    scvMatSetVal(mat, 1, 2, -sin * (-cx) + cos * (-cy) + cy);
}

void scvScaleMatrix(ScvPoint center, float scaleX, float scaleY, ScvMat *mat) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    const float cx = center.x;
    const float cy = center.y;
    const float newCX = cx * scaleX;
    const float newCY = cy * scaleY;

    scvMatSetVal(mat, 0, 0, scaleX);
    scvMatSetVal(mat, 0, 1, 0);
    scvMatSetVal(mat, 0, 2, -(newCX - cx));
    scvMatSetVal(mat, 1, 0, 0);
    scvMatSetVal(mat, 1, 1, scaleY);
    scvMatSetVal(mat, 1, 2, -(newCY - cy));
}

void scvTranslationMatrix(float dx, float dy, ScvMat *mat) {
    if (!(2 == mat->rows && 3 == mat->cols)) {
        /**
         * Must be:
         * [ a b | c ]
         * [ d e | f ]
         */
        return;
    }

    scvMatSetVal(mat, 0, 0, 1.0f);
    scvMatSetVal(mat, 0, 1, 0);
    scvMatSetVal(mat, 0, 2, dx);
    scvMatSetVal(mat, 1, 0, 0);
    scvMatSetVal(mat, 1, 1, 1.0f);
    scvMatSetVal(mat, 1, 2, dy);
}

#pragma mark -- Point Transformation

void scvFillImage(ScvImage *image, ScvPixel fillPxl) {
    for (int iy = 0; iy < image->height; iy++) {
        for (int ix = 0; ix < image->width; ix++) {
            scvSetPixel(image, ix, iy, fillPxl);
        }
    }
}

void scvGraying(ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE type) {
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            ScvPixel *dPxlRef = scvGetPixelRef(dst, ix, iy);
            if (NULL != dPxlRef) {
                int value = grayValueOfPixel(sPxl, type);
                if (value >= 0) {
                    dPxlRef->b = dPxlRef->g = dPxlRef->r = (ScvUByte) value;
                }
            }
        }
    }
}

void scvThreshold(ScvImage *src, ScvImage *dst, SCV_GRAYING_TYPE grayingType) {
    ScvHistogram *hist = scvCreateHist(grayingType);
    scvCalcHist(src, hist);
    float thresh = thresholdOtsu(hist, src->width * src->height);
    scvReleaseHist(hist);
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            ScvPixel *dPxlRef = scvGetPixelRef(dst, ix, iy);
            if (NULL != dPxlRef) {
                int value = grayValueOfPixel(sPxl, grayingType);
                if (value >= 0 && value < 256) {
                    dPxlRef->b = dPxlRef->g = dPxlRef->r = (ScvUByte) (value > thresh ? 255 : 0);
                }
            }
        }
    }
}

void scvSplit(ScvImage *src, ScvImage *b, ScvImage *g, ScvImage *r) {
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            scvSetPixel(b, ix, iy, scvPixelAll(sPxl.b));
            scvSetPixel(g, ix, iy, scvPixelAll(sPxl.g));
            scvSetPixel(r, ix, iy, scvPixelAll(sPxl.r));
        }
    }
}

void scvInverse(ScvImage *src, ScvImage *dst) {
    for (int iy = 0; iy < src->height; iy++) {
        for (int ix = 0; ix < src->width; ix++) {
            ScvPixel sPxl = scvGetPixel(src, ix, iy);
            scvSetPixel(dst, ix, iy, scvPixel(255 - sPxl.b, 255 - sPxl.g, 255 - sPxl.r));
        }
    }
}
