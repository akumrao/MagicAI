/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_REGISTRATION_DEFS_H_
#define XAILIENT_FI_REGISTRATION_DEFS_H_

// std
#include <stdlib.h>
// xailient
#include <xailient-fi/fi_common_types.h>
#include <xailient-fi/fi_defs.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    // dnn confidence obtained when detecting the face
    float dnn_confidence;
    // dnn bounding box
    xa_fi_bounding_box_t dnn_bbox;
    // normalized detected (dnn) bounding box
    xa_fi_bounding_box_t normalized_dnn_bbox;
    // normalized landmark bounding box
    xa_fi_bounding_box_t normalized_lm_bbox;
    // lm confidence
    float lm_confidence;
    // the raw landmark positions we found, this are values between [0,1] representing the relative position on
    // the normalized_lm_bbox
    xa_fi_float_point_t raw_landmarks[5];
    // the pixel positions of each of the landmarks on the input image. The input image is what is passed
    // to the run @p xa_fi_run_register_mode function (note that the landmark takes the full image and performs
    // the crop internally)
    xa_fi_int_point_t landmarks[5];
    // the embedding generated and used when processing the current face
    xa_fi_float_array_t embedding;
    // ID and version of the vision cell used to create the embedding
    const char * vision_cell_id;
    int vision_cell_version;
} xa_fi_register_result_t;

#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_REGISTRATION_DEFS_H_
