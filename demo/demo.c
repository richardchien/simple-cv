#include "scv.h"

int main() {
    ScvImage *image = scvLoadImage("image.bmp");

    // Test matrix transformation
    ScvImage *image2 = scvCreateImage(scvGetSize(image));
    float m[6] = {1.2f, 0, 0,
                  0, 1.0f, 0};
    ScvMat mat = scvMat(2, 3, m);
    scvRotationMatrix(scvPoint(image->width / 2, image->height / 2), 30, &mat);
    scvTranslationMatrix(20, 20, &mat);
    scvScaleMatrix(scvPoint(image->width / 2, image->height / 2), -1.2f, 0.8f, &mat);
    scvFlipMatrix(scvGetCenter(image), SCV_FLIP_HORIZONTAL, &mat);
    scvWarpAffine(image, image2, &mat, scvPixelAll(0));
    scvGraying(image2, image2, SCV_GRAYING_W_AVG);
    scvSaveImage(image2, "image2.bmp");

    // Test threshold
    ScvImage *imageBin = scvCreateImage(scvGetSize(image));
    scvThreshold(image, imageBin, SCV_GRAYING_W_AVG);
    scvSaveImage(imageBin, "bin.bmp");

    // Test split
    ScvImage *b = scvCreateImage(scvGetSize(image));
    ScvImage *g = scvCreateImage(scvGetSize(image));
    ScvImage *r = scvCreateImage(scvGetSize(image));
    scvSplit(image, b, g, r);
    scvSaveImage(b, "img_b.bmp");
    scvSaveImage(g, "img_g.bmp");
    scvSaveImage(r, "img_r.bmp");

    // Test inverse
    ScvImage *imageInv = scvCreateImage(scvGetSize(image));
    scvInverse(image, imageInv);
    scvSaveImage(imageInv, "inv.bmp");

    // Test equalize hist
    ScvHistogram *histogram = scvCreateHist(SCV_GRAYING_AVG);
    scvCalcHist(image, histogram);
    ScvImage *imageEquHist = scvCreateImage(scvGetSize(image));
    scvEqualizeHist(image, imageEquHist, histogram);
    scvSaveImage(imageEquHist, "equalized_hist.bmp");

    // Test smooth
    ScvImage *imageSmooth = scvCreateImage(scvGetSize(image));
    scvSmooth(image, imageSmooth, SCV_SMOOTH_MEDIAN);
    scvSaveImage(imageSmooth, "smooth.bmp");

    // Test canny
    ScvImage *imageCanny = scvCreateImage(scvGetSize(image));
    ScvImage *imageGray = scvCreateImage(scvGetSize(image));
    scvGraying(image, imageGray, SCV_GRAYING_AVG);
    scvCanny(imageGray, imageCanny);
    scvSaveImage(imageCanny, "canny.bmp");

    return 0;
}
