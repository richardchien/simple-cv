# SimpleCV

[![License](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/richardchien/simple-cv/blob/master/LICENSE)

C 语言课程设计选了图像处理的题，要求只能使用 OpenCV 读取图片，不能调用其它处理图片的函数，（反正无聊），于是索性参照 OpenCV 的接口，自己实现了一个简易的 SimpleCV，功能很有限，且算法效率并不高，仅仅是按照课程设计的需求来的。

注意：现在只能读取和写入 24 位 3 通道的 BMP 文件。

## 用法

```c
ScvImage *image = scvLoadImage("image.bmp");

// Matrix transformation
ScvImage *imageTrans = scvCreateImage(scvGetSize(image));
float m[6];
ScvMat mat = scvMat(2, 3, m);
//scvRotationMatrix(scvPoint(image->width / 2, image->height / 2), 30, &mat);
//scvTranslationMatrix(20, 20, &mat);
//scvScaleMatrix(scvPoint(image->width / 2, image->height / 2), -1.2f, 0.8f, &mat);
scvFlipMatrix(scvGetCenter(image), SCV_FLIP_HORIZONTAL, &mat);
scvWarpAffine(image, imageTrans, &mat, scvPixelAll(0));
scvSaveImage(imageTrans, "trans.bmp");

// Threshold / Binarization
ScvImage *imageBin = scvCreateImage(scvGetSize(image));
scvThreshold(image, imageBin, SCV_GRAYING_W_AVG);
scvSaveImage(imageBin, "bin.bmp");

// Split RGB
ScvImage *b = scvCreateImage(scvGetSize(image));
ScvImage *g = scvCreateImage(scvGetSize(image));
ScvImage *r = scvCreateImage(scvGetSize(image));
scvSplit(image, b, g, r);
scvSaveImage(b, "b.bmp");
scvSaveImage(g, "g.bmp");
scvSaveImage(r, "r.bmp");

// Inverse
ScvImage *imageInv = scvCreateImage(scvGetSize(image));
scvInverse(image, imageInv);
scvSaveImage(imageInv, "inv.bmp");

// Graying
ScvImage *imageGray = scvCreateImage(scvGetSize(image));
scvGraying(image, imageGray, SCV_GRAYING_AVG);
scvSaveImage(imageGray, "gray.bmp");

// Equalize hist
ScvHistogram *histogram = scvCreateHist(SCV_GRAYING_MAX);
scvCalcHist(image, histogram);
ScvImage *imageEquHist = scvCreateImage(scvGetSize(image));
scvEqualizeHist(image, imageEquHist, histogram);
scvSaveImage(imageEquHist, "equalized_hist.bmp");

// Smooth
ScvImage *imageSmooth = scvCreateImage(scvGetSize(image));
scvSmooth(image, imageSmooth, SCV_SMOOTH_AVG);
scvSaveImage(imageSmooth, "smooth.bmp");

// Canny outline detection
ScvImage *imageCanny = scvCreateImage(scvGetSize(image));
ScvImage *imageGray = scvCreateImage(scvGetSize(image));
scvGraying(image, imageGray, SCV_GRAYING_AVG);
scvCanny(imageGray, imageCanny);
scvSaveImage(imageCanny, "canny.bmp");
```
