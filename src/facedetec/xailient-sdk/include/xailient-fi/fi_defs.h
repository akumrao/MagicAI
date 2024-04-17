/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_DEFS_H_
#define XAILIENT_FI_DEFS_H_

// std
#include <stdlib.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


///
/// @note All this interfaces are subject to change, this is under development
///


/**
 * @brief Error definition
 *
 */
typedef enum {
    // no error happened
    XA_ERR_NONE = 0,
    // some of the arguments are invalid
    XA_ERR_INVALID_ARGS,
    // operation not permitted, when something was not properly initialized for example
    XA_ERR_INVALID_OP,
    // there was an internal error
    XA_ERR_INTERNAL,
    // we have reached a limit on the amount of items that can be registered
    XA_ERR_LIMIT,
    // not implemented error, this error is returned when certain features are not implemented
    // on the target platform
    XA_ERR_NOT_IMPLEMENTED,
    // not found based on arguments
    XA_ERR_NOT_FOUND,
    // FR disabled
    XA_ERR_FR_NOT_ENABLED,
    // Out of memory error
    XA_ERR_OUT_OF_MEMORY,
    // Malformed JSON
    XA_ERR_MALFORMED_JSON,
    // A JSON argument is missing a required field
    XA_ERR_MISSING_REQUIRED_FIELD
} xa_fi_error_t;


/**
 * @brief Face selection algorithm type. The face selection algorithm defines how and when faces should be
 *        selected for verification. This is useful only when we do not want to process (i.e. verify)
 *        all faces on the FOV, mainly for performance matters.
 *
 */
typedef enum {
    // picks only the biggest faces of all the ones detected
    XA_FSELECTOR_BIGGEST_FACE,
    // selects only the faces that were the least verified in previous iterations. This means
    // that faces that have not yet been verified (or try to be verified) will have higher priority
    // than the ones that were already processed.
    XA_FSELECTOR_LEAST_VERIFIED_FACE,
    // A round robin but prioritizing the non-identified faces yet with a threshold N indicating how many times we
    // should try to identify it before considering it as the same priority than the verified ones.
    // The way it works will be having 2 sets of faces to verify, a prioritized set, and a normal set. Each set of
    // faces will be verified using a basic round robin algorithm but the “prioritized set” will be always evaluated
    // first. After N times a face is evaluated on the “prioritized set”, its moved to the normal set.
    // Once the “prioritized set” is empty, verification on the “normal set” are performed using the basic round
    // robin approach.
    // The constant N is pre-calculated by xailient based on training and tests performed in-house.
    XA_FSELECTOR_RROBIN_PRIORITY_FACE,
    // select all faces
    XA_FSELECTOR_ALL_FACES,
    // select biggest unrecognized face
    XA_FSELECTOR_BIGGEST_UNRECOGNIZED_FACE
    // ...
} xa_fi_face_selector_t;

/**
 * @brief The different type of logs that the sdk can emit (check xa_fi_log_cb_t)
 *
 */
typedef enum {
  XA_LOG_TYPE_ALWAYS = 0,
  XA_LOG_TYPE_ERROR = 1,
  XA_LOG_TYPE_WARN = 2,
  XA_LOG_TYPE_INFO = 3,
  XA_LOG_TYPE_DEBUG = 4,
  XA_LOG_TYPE_TRACE = 5
} xa_fi_log_type_t;

/**
 * @brief Defines the callback signature for receiving log messages
 *
 */
typedef void(*xa_fi_log_cb_t)(xa_fi_log_type_t, const char*);

/**
 * @brief The list of arguments required to build the vision cell
 *
 */
typedef struct {
    // path to the .so vision cell to be loaded.
    // This must be nullptr for those platforms that do not support shared libraries.
    const char* vision_cell_path;
} xa_fi_vision_cell_args_t;


/**
 * @brief Information about the current vision cell
 *
 */
typedef struct {
    // id, version of the vision cell. These correspond to `Model.id`, 'Model.version` in the backend.
    const char* id;
    int int_version;
    // version of the vision cell in format "1.2.3"
    const char* version;
} xa_fi_vision_cell_info_t;

/**
 * @brief Information about the xailient face identification sdk
 *
 */
typedef struct {
    // id of the current sdk
    const char* id;
    // integer version of the current sdk
    int int_version;
    // string version of the current sdk
    const char* version;
} xa_fi_sdk_info_t;

/**
 * @brief Here we pass the list of all parameters that configure the working mode
 *        of the vision cell.
 *
 */
typedef struct {
    // The dnn threshold applied to the output heatmap from the dnn.
    // Pixel values larger than this threshold are considered to be a detection.
    // Range: [0.0 ... 1.0]
    // this will be only used if the detectum is used
    float dnn_threshold;
    // These 2 options are nanodet-related only, this will be used when the nanodet model is present
    float nanodet_score_threshold;
    float nanodet_nms_threshold;
    // we want to keep bboxes that have a area bigger than X% of the original image
    // if we take 1% seems to small but it is not actually, if we divide the image in 10 by 10, 1 cell is 1%
    // Range: [0.0 ... 1.0]
    float min_acceptable_bbox_area;
    // Min, max aspect ratios range (width / height) of bounding box that will be considered detections.
    // Detected bboxes outside of these values will discarded as they are likely false positives.
    float min_acceptable_bbox_aspect_ratio;
    float max_acceptable_bbox_aspect_ratio;
    // The "bounding box confidence" of a DNN detection is the average value of all the pixels inside the bbox
    // where the value of each pixel is taken from the the DNN's output heatmap.
    // Pixels that are a face should always be >=  `dnn_threshold`.
    // But background pixels that are included in the bbox will be < `dnn_threshold`, thus driving the average
    // (aka confidence) down.
    // Bboxes whose confidence value is below this value will be filtered out (discarded)
    // Range: [0.0 ... 1.0]
    float dnn_bbox_confidence_threshold;
    // The landmark confidence threshold to use.
    // We do not run MFN on any face (bbox) whose subsequently detected landmarks have a confidence below this threshold.
    // Range: [0.0 ... 1.0]
    float lm_confidence_threshold;

    // Maximum number of faces that can be registered per vision cell.
    unsigned int max_faces_to_register;
    // Maximum number of faces that we will be tracked by the object tracker simultaneously.
    // If there are 10 faces detected, but `max_verification_slots` is 3 then we will track only the
    // first 3 detected faces.
    // Range: [1...N]. "N" should be bounded to something reasonable for the device.
    unsigned int max_verification_slots;
    // The distance value used at the moment of deciding if 2 faces are similar enough or not.
    // Similar faces have a smaller distance between them. Dissimilar faces have a larger distance between them.
    // The face_threshold_distance can be increased to see if that helps reduce mis-identifications.
    // the default value for face_threshold_distance has been determined to be an optimal value based on the
    // Xailient training and testing data. But in field conditions may be slightly different and thus changing this value
    // may be helpful.
    // Range: [0.0 ... 1.0]
    float face_threshold_distance;
    // Threshold percentage used to determine the number of identifications that must occur internally in the SDK
    // before the SDK will consider the face identified. This is a ratio.
    // Relates to `consensus_num`.
    // Example: The goal is that identification should only be determined after at least 3 out of the last 4 MFN verifications
    //  determine the same identity for the same face track.
    //  To do this:
    //      voting_threshold = 0.75
    //      consensus_num = 4
    // Range: [0.0 ... 1.0]
    float voting_threshold;
    // We track the number of positive identifications that have been made for a given face over a sliding
    // window of size `consensus_num` identification attempts (meaning we run MFN to determine face
    // embeddings for the face).
    // This setting determines the size of that sliding window.
    // Relates to `face_threshold_distance`.
    // Units: The number of verifications (not frames)
    unsigned int consensus_num;
    // The distance function to be used, by default is selected the most optimal one
    // based on Xailient internals research.
    // Possible values: "COSINE", "EUCLIDEAN"
    const char* distance_function_type;
    // The algorithm to be used when determining which face(s) to process next in verification mode.
    // This applies primarily to cases where multiple faces are in the FoV. We processed FaceA
    // in frame N. Which face(s) should we process in frame N+1?
    // By default the solution process all the faces we find on the image.
    // Read more in docs for `xa_fi_face_selector_t`
    // Relates to `max_verification_faces_per_frame`
    xa_fi_face_selector_t face_selector_type;
    // Defines maximum number of faces we want to process simultaneously in a single frame.
    // This works together with the `face_selector_type`; in the case of XA_FSELECTOR_ALL_FACES this
    // value has no impact.
    unsigned int max_verification_faces_per_frame;
    // How many verifications we want to perform on a face before we generate a
    // XA_FTRACK_IDENTITY_NOT_IN_GALLERY event (assuming the face is not recognized).
    // This value SHOULD BE HIGHER than `consensus_num` (at least `consensus_num` + 1)
    // Units: MFN attempts
    unsigned int not_in_gallery_verification_count_threshold;
    // How frequently do we want to perform (re)verification on faces that we already generated a
    // XA_FTRACK_IDENTITY_RECOGNIZED event for.
    // This frequency indicates how often, measured in frames where the face is visible (detected), LM+MFN
    // will be run on face tracks that have already had a XA_FTRACK_IDENTITY_RECOGNIZED event generated.
    // Examples:
    //    0 - The SDK will not run verification ever again after a XA_FTRACK_IDENTITY_RECOGNIZED event is generated
    //        *unless* the face happens to merge with another face(s) in the FoV in which case the SDK may
    //        may run verification again but only for purposes of making sure the face belongs to the correct
    //        face track as the faces diverge.
    //    1 - The SDK WILL run verification again starting on the next eligible frame after a XA_FTRACK_IDENTITY_RECOGNIZED
    //        is generated. In other words, the face track will continually have verification (LM + MFN)
    //        run on each eligible frame even after a XA_FTRACK_IDENTITY_RECOGNIZED event is generated for the face track.
    //        This is inefficient from a resource (CPU, power) perspective. It's only purpose is for helping evaluate
    //        the efficacy of the FR pipeline.
    // Units: The number of frames
    unsigned int verification_after_identification_freq;

    // switch to enable/disable deskewing feature. The deskwing its applied to the detected face, before running mfn
    // using the 5 landmarks from the landmark model.
    bool deskewing_enabled;
    // Switch flag to enable or disable padding feature (adds padding to the input image to keep aspect ratio
    // at the moment of resizing to match the input model size).
    bool enable_padding;

    //
    // object tracking related
    //

    // Duration in seconds before considering the object is lost and remove from tracked
    // object list
    // Units: seconds
    float objtrack_lost_duration_secs;
    // This parameter is for testing mainly, currently it defines the maxium distance in
    // pixels that an bounding should be match with objects.
    float objtrack_max_cost;
    // The number frames we have to see a bounding box to consider it as a valid face.
    // The purpose of this is to help minimize false positive face detections. For example,
    // an FP face is detected for one frame then disappears. We want to ignore such "detections".
    // Units: frames
    // Range: 1..N.
    int objtrack_frames_to_be_valid;


    //
    // monitoring related options
    //

    // Number of `XA_ACCURACY_MONITOR` events that we want to emit to the Orchestrait backend per day.
    // To disable the monitoring feature set this to 0.
    // Range: 0..N
    unsigned int monitoring_day_max_count;
    // How much time we want to wait (in seconds) after emitting `XA_ACCURACY_MONITOR` event, before
    // starting to monitor again.
    unsigned int monitoring_waiting_between_evts_secs;

    // For the object tracker to recover from faces moving "too fast"
    // If it's negative, this feature is turned off
    // Otherwise it's the distance to clobber an existing face track
    float max_unrecognized_distance_to_supersede_face_track;

    // Device level flags whether Orchestrait should collect images related to face track events (product improvement) and accuracy monitoring
    bool orchestrait_collect_images_product_improvement_recognized;
    bool orchestrait_collect_images_product_improvement_nonrecognized;
    bool orchestrait_collect_images_accuracy_monitoring_recognized;
    bool orchestrait_collect_images_accuracy_monitoring_nonrecognized;

} xa_fi_config_t;

/**
 * @brief The identity structure contains all the information required to associated
 *        embeddings to a given identity uniquely (id). The name is a friendly
 *        user specified id which is not unique
 */
typedef struct {
    // the id of the identiyy
    const char* id;
    // the user friendly name to identify the identity (note that multiple identities can have the same name,
    // i.e. will not be used as unique identifier).
    const char* name;
    // user consent flag for accuracy_monitor
    bool accuracy_monitor_consent;
    // user consent flag for product improvement
    bool product_improvement_consent;

} xa_fi_face_identity_t;

///
/// TODO: this will be defined later, no need to worry about them now
///       are here for reference only

typedef struct {
  // whatever we need to configure in here like freq, stages, format, etc
} xa_fi_sampling_config_t;

typedef struct {
  // whatever we need to configure in here like freq, stages, format, etc
} xa_fi_stats_config_t;

typedef struct {
  // when processing dnn results callback
  // when picking faces to be verified? (we may not want to expose this)
  // when processing lm results callback (we may not want to expose this)
  // when identifying a given person callback (one for each person if multiple)
} xa_fi_callbacks_t;



#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_DEFS_H_
