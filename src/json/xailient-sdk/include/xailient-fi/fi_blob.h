/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_BLOB_H_
#define XAILIENT_FI_BLOB_H_

// std
#include <stdlib.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct xa_fi_blob_t_ xa_fi_blob_t;


/**
 * @brief Allocates a new blob of a given size (bytes).
 *        The caller must call @ref xa_fi_blob_free() once the blob
 *        is not needed anymore.
 *
 *        The blob is like a normal buffer that allow to extract and inject
 *        binary data (serialized) from/to the library.
 *
 * @param size  The size of the blob we want to allocate.
 * @return xa_fi_blob_t*
 */
xa_fi_blob_t*
xa_fi_blob_new(size_t size);

/**
 * @brief Frees an allocated blob
 *
 * @param blob The blob to free
 */
void
xa_fi_blob_free(xa_fi_blob_t* blob);

/**
 * @brief The size of the allocated blob buffer in bytes.
 *        This is the number of bytes that can be stored / read on
 *        the given blob.
 *
 * @param blob  The blob to get the size for
 * @return size_t amount of bytes of the blob buffer
 */
size_t
xa_fi_blob_size(const xa_fi_blob_t* blob);

/**
 * @brief Returns the internal buffer of the blob.
 *        Its a const accessor function
 *
 * @param blob  The blob we want to access the buffer to
 * @return const uint8_t* the buffer of the blob, check @ref xa_fi_blob_size() to
 *         obtain the size of the buffer
 */
const uint8_t*
xa_fi_blob_buff_const(const xa_fi_blob_t* blob);

/**
 * @brief Returns the internal buffer of the blob.
 *        Its a non const accessor function
 *
 * @param blob  The blob we want to access the buffer to
 * @return uint8_t* the buffer of the blob, check @ref xa_fi_blob_size() to
 *         obtain the size of the buffer
 */
uint8_t*
xa_fi_blob_buff(xa_fi_blob_t* blob);

/**
 * @brief Returns the internal buffer of the blob as a null terminated c string
 *        Its a non const accessor function
 *
 * @param blob  The blob we want to access the buffer to
 * @return char* the buffer of the blob, check @ref xa_fi_blob_size() to obtain
 *         the size of the buffer
 */
const char*
xa_fi_blob_get_c_string(xa_fi_blob_t* blob);


#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_BLOB_H_