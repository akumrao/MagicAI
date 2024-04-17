/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_IMAGE_H_
#define XAILIENT_FI_IMAGE_H_

// std
#include <stdlib.h>
#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Pixel format of the image (supported)
 */
typedef enum {
    XA_FI_COLOR_RGB888,
    // TODO we should add YUV probably in the future
    XA_FI_COLOR_UNKNOWN,
} xa_fi_pixel_format_t;


/**
 * @brief Image structure definition
 *
 */
typedef struct {
  uint8_t* buff;
  size_t width;
  size_t height;
  // TODO: we may also need to add this if we support multichanel images
  //       unsigned int channels;
  xa_fi_pixel_format_t pixel_format;
} xa_fi_image_t;


/* An example of creating an xa_fi_image_t suitable for passing to xa_fi_run_verify_mode:
   xa_fi_image_t image;
   image.buff = &buffer_containing_raw_rgb_data;   // note this is in RGB order, otherwise colors will be swapped
   image.width = buffer_image_width;
   image.height = buffer_image_height;
   image.pixel_format = XA_FI_COLOR_RGB888;        // signifies the buffer data format
*/

#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_IMAGE_H_