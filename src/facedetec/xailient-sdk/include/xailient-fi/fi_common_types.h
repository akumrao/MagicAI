/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_COMMON_TYPES_H_
#define XAILIENT_FI_COMMON_TYPES_H_

// std
#include <stdlib.h>



#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    float* values;
    size_t count;
} xa_fi_float_array_t;


typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} xa_fi_bounding_box_t;


typedef struct {
    int x;
    int y;
} xa_fi_int_point_t;

typedef struct {
    float x;
    float y;
} xa_fi_float_point_t;

#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_COMMON_TYPES_H_