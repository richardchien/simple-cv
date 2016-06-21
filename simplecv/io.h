//
// Created by Richard Chien on 6/20/16.
//

#include "types.h"

#ifndef SIMPLECV_IO_H
#define SIMPLECV_IO_H

ScvImage *scvLoadImage(const char *filename);

BOOL scvSaveImage(ScvImage *image, const char *filename);

#endif //SIMPLECV_IO_H
