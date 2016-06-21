//
// Created by Richard Chien on 6/21/16.
//

#include "matrix.h"
#include "core.h"

#pragma mark - Export

float scvMatGetVal(const ScvMat *mat, int i, int j) {
    if (i < 0 || i >= mat->rows || j < 0 || j >= mat->cols) {
        return 0;
    }
    return mat->data[i * mat->cols + j];
}

void scvMatSetVal(ScvMat *mat, int i, int j, float value) {
    if (i < 0 || i >= mat->rows || j < 0 || j >= mat->cols) {
        return;
    }
    mat->data[i * mat->cols + j] = value;
}

void scvMatDotProduct(const ScvMat *left, const ScvMat *right, ScvMat *dst) {
    if (left->cols != right->rows || dst->rows != left->rows || dst->cols != right->cols) {
        // Cannot calculate product
        return;
    }

    int cloned = 0;
    if (left == dst) {
        left = scvCloneMat(dst);
        cloned |= (1 << 1);
    }
    if (right == dst) {
        right = scvCloneMat(dst);
        cloned |= 1;
    }

    const int n = left->cols;
    for (int i = 0; i < dst->rows; i++) {
        for (int j = 0; j < dst->cols; j++) {
            float cur = 0;
            for (int k = 0; k < n; k++) {
                cur += scvMatGetVal(left, i, k) * scvMatGetVal(right, k, j);
            }
            scvMatSetVal(dst, i, j, cur);
        }
    }

    if (cloned & (1 << 1)) {
        scvReleaseMat((ScvMat *) left);
    }
    if (cloned & 1) {
        scvReleaseMat((ScvMat *) right);
    }
}

void scvMatNumProduct(float k, const ScvMat *mat, ScvMat *dst) {
    if (mat->rows != dst->rows || mat->cols != dst->cols) {
        // Cannot calculate
        return;
    }

    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            scvMatSetVal(dst, i, j, k * scvMatGetVal(mat, i, j));
        }
    }
}

void scvMatInverse(const ScvMat *src, ScvMat *dst) {
    if (src->rows != src->cols || dst->rows != dst->cols || src->rows != dst->rows) {
        // Cannot calculate
        return;
    }

    const int n = src->rows;
    float det = scvMatDet(src);
    float temp[n * n];
    ScvMat adj = scvMat(n, n, temp);
    scvMatAdjugate(src, &adj);
    scvMatNumProduct(1 / det, &adj, dst);
}

float scvMatDet(const ScvMat *mat) {
    if (mat->rows != mat->cols) {
        // Must be square matrix
        return 0;
    }

    if (1 == mat->rows) {
        return mat->data[0];
    }

    if (2 == mat->rows) {
        float *m = mat->data;
        return m[0] * m[3] - m[1] * m[2];
    }

    if (3 == mat->rows) {
        float *m = mat->data;
        return m[0] * m[4] * m[8] + m[3] * m[7] * m[2] + m[6] * m[1] * m[5]
               - m[2] * m[4] * m[6] - m[5] * m[7] * m[0] - m[8] * m[1] * m[3];
    }

    const int minorRows = mat->rows - 1;
    float result = 0;
    int sign = 1;
    for (int i = 0; i < mat->rows; i++) {
        float curM[minorRows * minorRows];
        ScvMat curMinor = scvMat(minorRows, minorRows, curM);
        scvMatMinor(mat, &curMinor, i, 0);
        result += scvMatGetVal(mat, i, 0) * sign * scvMatDet(&curMinor);
        sign = -sign;
    }
    return result;
}

/**
 * Obtain minor matrix by removing just one row and one column.
 */
void scvMatMinor(const ScvMat *src, ScvMat *dst, int i, int j) {
    if (src->rows != src->cols || dst->rows != dst->cols || src->rows != dst->rows + 1) {
        // Cannot calculate
        return;
    }

    for (int k = 0, nowI = 0; k < dst->rows; nowI++) {
        if (nowI == i) {
            // Skip the selected row
            continue;
        }

        for (int l = 0, nowJ = 0; l < dst->cols; nowJ++) {
            if (nowJ == j) {
                // Skip the selected column
                continue;
            }

            scvMatSetVal(dst, k, l, scvMatGetVal(src, nowI, nowJ));
            l++;
        }
        k++;
    }
}

void scvMatAdjugate(const ScvMat *src, ScvMat *dst) {
    if (src->rows != src->cols || dst->rows != dst->cols || src->rows != dst->rows) {
        // Cannot calculate
        return;
    }

    int cloned = 0;
    if (src == dst) {
        src = scvCloneMat(dst);
        cloned = 1;
    }

    const int n = dst->rows;
    float m[n * n];
    ScvMat minor = scvMat(n - 1, n - 1, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sign = (i + j) % 2 == 0 ? 1 : -1;
            scvMatMinor(src, &minor, j, i);
            scvMatSetVal(dst, i, j, sign * scvMatDet(&minor));
        }
    }

    if (cloned) {
        scvReleaseMat((ScvMat *) src);
    }
}

void scvMatTranspose(const ScvMat *src, ScvMat *dst) {
    if (src->rows != dst->cols || src->cols != dst->rows) {
        // Cannot transpose
        return;
    }

    int cloned = 0;
    if (src == dst) {
        src = scvCloneMat(dst);
        cloned = 1;
    }

    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            scvMatSetVal(dst, j, i, scvMatGetVal(src, i, j));
        }
    }

    if (cloned) {
        scvReleaseMat((ScvMat *) src);
    }
}
