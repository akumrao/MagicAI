/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_VERIFICATION_DEFS_H_
#define XAILIENT_FI_VERIFICATION_DEFS_H_

// std
#include <stdlib.h>
#include <stdint.h>
// xailient
#include <xailient-fi/fi_common_types.h>
#include <xailient-fi/fi_defs.h>
#include <xailient-fi/fi_image.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Indices for the landmarks on the buffer
 *
 */
typedef enum {
    XA_LM_POS_LEFT_EYE = 0,
    XA_LM_POS_RIGHT_EYE = 1,
    XA_LM_POSNOSE = 2,
    XA_LM_POS_LEFT_MOUTH = 3,
    XA_LM_POS_RIGHT_MOUTH = 4,
} xa_fi_landmark_pos_t;


/**
 * @brief Face track event types
 *
 */
typedef enum {
    // when a identity jumps into a field of view
    XA_FTRACK_FOV_IN = 0,
    // when a new identity is recognized (from non-recognized -> to recognized)
    XA_FTRACK_IDENTITY_RECOGNIZED,
    // when a recognized identity stops being recognized. This is most probably due to
    // a generated embedding that does not match anymore (lighting, face position, noise on the image)
    // against the registered face
    XA_FTRACK_IDENTITY_DERECOGNIZED,
    // when a face identity change from a current identified face to a completely different one
    // this may happen when consensus is 1 and embeddings of registered persons are quite similar.
    // this is probably a rare case
    XA_FTRACK_IDENTITY_CHANGE,
    // when a identity goes out of the field of view
    XA_FTRACK_FOV_OUT,
    // when a identity is on the FOV and we are sure that we cannot recognize it (i.e. its not
    // on the gallery)
    XA_FTRACK_IDENTITY_NOT_IN_GALLERY,
} xa_fi_face_track_event_type_t;


/**
 * @brief Struct containing the metadata about a face that has been verified.
 *
 */
typedef struct {
    // dnn confidence
    float dnn_confidence{0.0f};
    // dnn bounding box
    xa_fi_bounding_box_t dnn_bbox;
    // normalized detected (dnn) bounding box
    xa_fi_bounding_box_t normalized_dnn_bbox;
    // normalized landmark bounding box
    xa_fi_bounding_box_t normalized_lm_bbox;
    // lm confidence
    float lm_confidence{0.0f};
    // the pixel positions of each of the landmarks on the input image
    xa_fi_int_point_t landmarks[5];
    // the distance value to the closest registered face we found on the gallery.
    // when the identity is defined (not empty), the distance is associated to the
    // identified (matching) identity.
    // in case the identity is empty, the distance will be associated to the nearest_identity instead.
    float encoder_distance{0.0f};
} xa_fi_face_verification_metadata;

/**
 * @brief Struct containing the faces that had been processed
 *        in the last frame.
 *
 */
typedef struct {
    // the id of the face verified if any. If this is null
    // means that the face was not verified.
    xa_fi_face_identity_t identity;
    // the closest identity we found on the gallery when performing verification. This is only
    // set when @ref identity is empty.
    xa_fi_face_identity_t nearest_identity;
    // the embedding generated and used when processing the current face
    xa_fi_float_array_t embedding;
    xa_fi_face_verification_metadata metadata;
} xa_fi_face_verification_t;


/**
 * @brief Struct returned when searching for a similar identity or matching identity on the gallery
 *        @ref xa_fi_find_best_matching_face
 *
 */
typedef struct {
    // the closest identity that matches the given embedding. This does not necessarily means that
    // the embedding matches the identity (note that there is a threshold and other parameters used
    // to determine a "matching").
    xa_fi_face_identity_t nearest_identity;
    // associated distance to nearest identity
    float encoder_distance;
} xa_fi_face_best_matching_t;


/**
 * @brief The face track events that occur on each execution of the verification mode
 *
 */
typedef struct {
    // the type of event
    xa_fi_face_track_event_type_t event_type;
    // the unique id that the system keeps track of for handling events.
    // Note that identity_name does not represent a unique id, since can be that appear simultaneously
    // on the same image.
    unsigned int track_id;
    // when the event took place, in seconds
    uint32_t timestamp;
    // the identity name involved on the event
    xa_fi_face_identity_t identity;
    // The last image that was passed to the encoder model for this face
    xa_fi_image_t encoder_cropped_image;
    // Like `encoder_cropped_image` only more loosely cropped
    xa_fi_image_t loosely_cropped_image;
    // The metadata from the FV pipeline about the face that this event is for. All coordinates are in the original
    // input image's range.
    xa_fi_face_verification_metadata face_verification_metadata;
    // These coordinates are localized to `loosely_cropped_image`
    xa_fi_face_verification_metadata loosely_cropped_face_verification_metadata;
    // Whether the branch customer should forward images to Orchestrait
    bool should_brand_customer_forward_images_to_orchestrait;
} xa_fi_face_track_event_t;


/**
 * @brief Struct containing the different type of data of monitoring events.
 *        These events occur when performing verification.
 *        The amount of monitoring events we will get is defined by the monitoring_day_max_count
 *        configuration parameter and based on how many faces we detect during a day.
 *
 */
typedef struct {
    // the id of the face verified if any. If this is null
    // means that the face was not verified.
    xa_fi_face_identity_t identity;
    // timestamp in ms when this face appear on the FOV
    uint64_t fov_in_timestamp_ms;
    // timestamp in ms when the identification happened
    uint64_t identification_timestamp_ms;
    // timestamp when the image was captured at, in milliseconds
    uint64_t image_capture_timestamp_ms;
    // the crop of the original image of the detected face
    xa_fi_image_t image;
    xa_fi_face_verification_metadata face_verification_metadata;
} xa_fi_monitoring_event_t;

/**
 * @brief Struct containing the information of objects that were also detected on the image
 *
 */
typedef struct {
    // the bounding box of the detected object
    xa_fi_bounding_box_t bbox;
    // the score / confidence
    float score;
    // the name or label
    const char* label;
} xa_fi_object_detected_info_t;

typedef struct {
    // the bounding box of the tracked face
    xa_fi_bounding_box_t face_bbox;
    // bounding box of tracked person
    xa_fi_bounding_box_t person_bbox;
    // the ID name
    const char* name = nullptr;
} xa_fi_tracked_face_info_t;

typedef struct {
    xa_fi_face_verification_t* faces;
    size_t count;
} xa_fi_face_data_array_t;

typedef struct {
    xa_fi_face_track_event_t* evts;
    size_t count;
} xa_fi_face_track_event_array_t;

typedef struct {
    xa_fi_monitoring_event_t* evts;
    size_t count;
} xa_fi_face_monitoring_event_array_t;

typedef struct {
    xa_fi_object_detected_info_t* objects;
    size_t count;
} xa_fi_object_detected_info_array_t;

typedef struct {
    xa_fi_tracked_face_info_t* tracked_faces_info;
    size_t count;
} xa_fi_tracked_face_info_array_t;

/**
 * @brief
 *
 */
typedef struct {
    // the list of faces that were processed on the
    // last verification call.
    xa_fi_face_data_array_t processed_faces;
    // the list of events that happened on the current verification call.
    // note that this list is not persisted internally and is flushed / refreshed on every
    // @p xa_fi_run_verify_mode
    xa_fi_face_track_event_array_t events_array;
    // the amount of faces that we are currently keeping track of
    size_t tracked_faces;
    // the list of monitoring events that occur on this verification run.
    // note that this list is not persisted internally and is flushed / refreshed on every
    // @p xa_fi_run_verify_mode
    xa_fi_face_monitoring_event_array_t monitoring_events_array;
    // an array of objects that have been detected on the image
    xa_fi_object_detected_info_array_t objects_array;
    xa_fi_tracked_face_info_array_t tracked_faces_info;
} xa_fi_verify_results_t;

/**
 * @brief verification mode callback, will be called whenever we obtain verification results
 *        after running the verification pipeline, for each processed image.
 *
 */
typedef void(*xa_fi_cb_verify_mode_t)(const xa_fi_verify_results_t*);


#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_VERIFICATION_DEFS_H_
