#include "simplecv/scv.h"

int main() {
	ScvImage *image = scvLoadImage("image.bmp");
	scvThreshold(image, image, SCV_GRAYING_W_AVG);
	scvSmooth(image, image, SCV_SMOOTH_GAUSSIAN);
	scvSaveImage(image, "out.bmp");

    return 0;
}
