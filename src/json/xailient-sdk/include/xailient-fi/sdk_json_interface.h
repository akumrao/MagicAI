#ifndef SDK_JSON_INTERFACE_H_
#define SDK_JSON_INTERFACE_H_

#include <xailient-fi/fi_image.h>
#include <xailient-fi/fi_defs.h>
#include <xailient-fi/fi_vision_cell.h>

#ifdef __cplusplus
extern "C" {
#endif


//
// Identity update structs
//
typedef struct
{
    const char * identity_id;
    const char * image_id;
} xa_sdk_identity_image_t;

typedef struct
{
    unsigned int number_of_remaining_images;
    const xa_sdk_identity_image_t * identity_images;
} xa_sdk_identity_images_t;


//
// Process image output structs
//
typedef enum
{
    XA_FACE_TRACK_EVENT,
    XA_ACCURACY_MONITOR
} xa_sdk_json_blob_descriptor_t;

typedef struct
{
    xa_sdk_json_blob_descriptor_t blob_descriptor;
    const char * json;
} xa_sdk_json_blob_t;

typedef struct
{
    unsigned int number_of_json_blobs;
    xa_sdk_json_blob_t * blobs;
} xa_sdk_process_image_outputs_t;

// An older version of this file referred to `xa_sdk_process_image_outputs` instead of following the convention of appending `_t`
// to type names. This is here to maintain backward compatibility so that existing customers don't need to update their code.
typedef xa_sdk_process_image_outputs_t xa_sdk_process_image_outputs;


// Initializes the SDK to receive identities, configure, and process images
// If path_to_vision_cell is NULL, it will attempt to load from ../lib/libxailient-fi-vcell.so
xa_fi_error_t xa_sdk_initialize(const char * path_to_vision_cell);

// Configures the vision cell with configuration json received from Orchestrait
xa_fi_error_t xa_sdk_configure(const char * json_configuration);

// Initiates an update to gallery identities from persisted memory at startup or an incoming update
//   json_identities - json from Orchestrait or persisted json_identities
//   remaining_identity_image_pairs - a list of images that need to be added to the gallery with
//                                    xa_sdk_add_identity_images
//   updated_json_identities - json identities updated with embeddings and other metadata
// Returns XA_ERR_FR_NOT_ENABLED if face recognition is disabled
xa_fi_error_t xa_sdk_update_identities(const char * json_identities,
                                       const xa_sdk_identity_images_t * * remaining_identity_image_pairs,
                                       const char * * updated_json_identities);

// Adds image data to the gallery
//   identity_id - the identity the image belongs to
//   image_id - the id of the image
//   image - the image
//   remaining_identity_image_pairs - a list of images that need to be added to the gallery
//   updated_json_identities - json identities updated with embeddings and other metadata
// Returns XA_ERR_FR_NOT_ENABLED if face recognition is disabled
xa_fi_error_t xa_sdk_add_identity_image(const char * identity_id,
                                        const char * image_id,
                                        const xa_fi_image_t * image,
                                        const xa_sdk_identity_images_t * * remaining_identity_image_pairs,
                                        const char * * updated_json_identities);

// "Returns" a number of JSON blobs and enumerations specifying what to do with them
// *process_image_outputs and all nested pointers are valid until this function is called again
// Returns XA_ERR_FR_NOT_ENABLED if face recognition is disabled
xa_fi_error_t xa_sdk_process_image(const xa_fi_image_t * image,
                                   const xa_sdk_process_image_outputs_t * * process_image_outputs);

// Returns JSON suitable for sending to the Xailient cloud via the customer cloud
// The returned pointer is valid until this function is called again
const char * xa_sdk_get_device_checkin_json();

// Returns XA_ERR_NONE if fr is permitted, XA_ERR_FR_NOT_ENABLED otherwise
xa_fi_error_t xa_sdk_is_face_recognition_enabled();


///
/// BEGIN Callback based execution methods
/// This will be only valid on platforms that AI models are executed using a callback method.
/// Otherwise these API will be disabled (XA_ERR_NOT_IMPLEMENTED)
///

// The type definition of the callback that gets called whenever the SDK detects that something interesting happening.
// The contents of the struct in `process_image_outputs` will become invalid after the completion of the callback.
// So the caller should make a copy of any information that it needs to access later.
typedef void(*xa_sdk_process_image_callback_t)(const xa_sdk_process_image_outputs_t* process_image_outputs);

typedef struct {
    // the verification callback, or null if not needed
    xa_sdk_process_image_callback_t verify_mode_cb;
} xa_sdk_cb_mode_config_t;

// Registers a callback that gets called whenever the SDK detects that something interesting happened.
// Only one callback can be configure at a time.
xa_fi_error_t xa_sdk_cb_configure(const xa_sdk_cb_mode_config_t* callback_config);

///
/// END Callback based execution methods
///


// Deactivates all face identification functionality
xa_fi_error_t xa_sdk_uninitialize();


//
// Logging interface
//

// Logging callback function signature
typedef void (*xa_sdk_log_callback_function_t)(xa_fi_log_type_t log_level, const char * log_string);

// Configures the SDK to pass logging messages through the log_function callback
xa_fi_error_t xa_sdk_register_log_callback(xa_sdk_log_callback_function_t log_function);


#ifdef __cplusplus
}
#endif

#endif

