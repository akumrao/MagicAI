/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/

/**
 * The API of the FI solution (vision cell) we will be delivering to the end customer should support:
 * - be able to load (/ unload) a specific vision cell (path to .so probably + arguments).
 * - be able to configure the vision cell (tweak thresholds, parameters, modes). by user or from backend
 * - register entities (faces) from images, one or multiple poses. Export them to blobs or load from blobs.
 * - verify one or multiple entities (faces) from one image (can be configured the behavior).
 * - obtain more detailed information from each stage over callbacks (like dnn results, lm?, ... TBD more in details what)
 * - configure sampling and stats gathering to be gathered on the callbacks or at the end of each inference.
 */

#ifndef XAILIENT_FI_VISION_CELL_H_
#define XAILIENT_FI_VISION_CELL_H_

// std
#include <stdlib.h>
// xailient
#include <xailient-fi/fi_defs.h>
#include <xailient-fi/fi_image.h>
#include <xailient-fi/fi_face.h>
#include <xailient-fi/fi_registration_defs.h>
#include <xailient-fi/fi_verification_defs.h>



#ifdef __cplusplus
extern "C" {
#endif

// opaque definition of the vision cell structure
typedef struct xa_fi_vision_cell_t_ xa_fi_vision_cell_t;


typedef struct {
    // the verification callback, or null if not needed
    xa_fi_cb_verify_mode_t verify_mode_cb;
} xa_fi_cb_mode_config_t;


//////////////////////////////////////////////////////////////////////////////////////////
/// Public API
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Current sdk version
 *
 * @return const xa_fi_sdk_info_t*
 */
const xa_fi_sdk_info_t*
xa_fi_sdk_version();


///
/// General functions
///

/**
 * @brief Sets the callback to receive notifications / logs.
 *        If the user wants to remove the callback, should pass NULL as argument.
 *        Only one callback will be hooked up.
 *        Log messages / notifications are triggered when evaluating any of the xa_fi_* functions.
 *        Note that not always errors nor notifications will be emitted.
 *
 * @param cb The callback to receive the log notifications
 */
void
xa_fi_set_log_callback(xa_fi_log_cb_t cb);

///
/// Vision cell functions
///

/**
 * @brief Allocates a new fi vision cell object.
 *        The caller is responsible for deleting this object when is not longer
 *        needed (@ref xa_fi_free).
 *        The vision cell object is the core instance and usually is one per app.
 *        It allow to load / unload different version of vision cells (plugin based).
 *
 * @return xa_fi_vision_cell_t* on success or NULL on error
 */
xa_fi_vision_cell_t*
xa_fi_new(void);

/**
 * @brief Frees an allocated FI vision cell object.
 *        After this function is called, @p fi will be no longer valid for future
 *        usage.
 *
 * @param fi  The vision cell instance we want to free.
 */
void
xa_fi_free(xa_fi_vision_cell_t* fi);


/**
 * @brief Loads a specific version of the cell based on given arguments.
 *        For those platforms that do not support shared libraries `xa_fi_vision_cell_args_t.vision_cell_path` should be nullptr.
 *
 * @param fi    The vision cell main object
 * @param args  The arguments required to load the vision cell
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_load(xa_fi_vision_cell_t* fi, const xa_fi_vision_cell_args_t* args);

/**
 * @brief Unloads the current vision cell if any.
 *
 * @param fi    The vision cell main object
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_unload(xa_fi_vision_cell_t* fi);


/**
 * @brief Returns the current vision cell information.
 *        This should be called after the vision cell is loaded
 *
 * @param fi    The vision cell main object
 * @return const xa_fi_vision_cell_info_t* on success | NULL if not loaded
 */
const xa_fi_vision_cell_info_t*
xa_fi_info(const xa_fi_vision_cell_t* fi);


/**
 * @brief Returns the current configuration of the vision cell.
 *        Every vision cell comes with a default configuration created
 *        at build time, still this can be overridden (@ref
 *        xa_fi_configure())
 *
 * @param fi    The vision cell main object
 * @return const xa_fi_config_t* containing the current configuration information
 */
const xa_fi_config_t*
xa_fi_config(const xa_fi_vision_cell_t* fi);


/**
 * @brief Returns the default (NOT current) configuration of the vision
 *        cell. Every vision cell comes with a default configuration created
 *        at build time, still this can be overridden (@ref
 *        xa_fi_configre())
 *
 * @param fi    The vision cell main object
 * @return const xa_fi_config_t* containing the default configuration
 *         information
 */
const xa_fi_config_t*
xa_fi_default_config(const xa_fi_vision_cell_t* fi);


/**
 * @brief Setup a new configuration mode for the vision cell.
 *        In case you want to change only a particular field, request the
 *        current configuration first and update the desired fields only.
 *
 * @param fi          The vision cell main object
 * @param parameters  The new configuration to be used
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_configure(xa_fi_vision_cell_t* fi, const xa_fi_config_t* parameters);

/**
 * @brief Resets the configuration to the default ones (set at build time)
 *
 * @param fi    The vision cell main object
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_reset_default_config(xa_fi_vision_cell_t* fi);


///
/// Face registrar creation / destruction interfaces
///

/**
 * @brief Allocates a new empty face object.
 *        The caller should call @ref xa_fi_face_registrar_free after is not used anymore
 *
 * @param fi  The vision cel object
 * @return xa_fi_face_registrar_t*
 */
xa_fi_face_registrar_t*
xa_fi_face_registrar_new(xa_fi_vision_cell_t* fi);

/**
 * @brief Frees the memory of an allocated face
 *
 * @param fi  The vision cel object
 * @param face The face to deallocate
 */
void
xa_fi_face_registrar_free(xa_fi_vision_cell_t* fi, xa_fi_face_registrar_t* face_reg);

///
/// Face information utility
///

/**
 * @brief Converts / serializes a face info into a binary format and stores it into
 *        the blob object. The blob will be allocated so the caller is responsable
 *        of freeing the blob ( @ref xa_fi_blob_free ).
 *
 *        This function is useful when we need to persist or transmit a face object
 *        to a file or the network so can be later loaded / imported.
 *
 * @param fi  The vision cel object
 * @param face_info  The face information object that we want to serialize into a blob object.
 * @param blob       A pointer ref of a blob object that will be allocated.
 *                   Whatever blob pointer is pointing to wont be affected by this function.
 *                   On error no blob will be allocated.
 * @return xa_fi_error_t The associated error code.
 */
xa_fi_error_t
xa_fi_face_info_to_blob(xa_fi_vision_cell_t* fi, const xa_fi_face_info_t* face_info, xa_fi_blob_t** blob);

/**
 * @brief Allocates and creates a new face info from an allocated blob. This works the inverse
 *        of @ref xa_fi_face_information_to_blob(). The blob must be filled
 *        with the serialized face content (read from a file / network / ...) .
 *        The blob memory won't be handled in this function, so the user is
 *        responsible for freeing the blob @ref xa_fi_blob_free.  The face
 *        object doesn't reference the blob, so the blob may be deleted after
 *        this function without affecting the face.
 *
 *        The returned face memory must be handled by the caller and freed
 *        calling @ref xa_fi_face_info_free once its no longer required.
 *
 * @param fi  The vision cell object
 * @param blob      The blob containing the serialized face data
 * @return xa_fi_face_info_t* the allocated face information object on success, otherwise NULL if the
 *         blob content is invalid.
 */
xa_fi_face_info_t*
xa_fi_face_info_from_blob(xa_fi_vision_cell_t* fi, const xa_fi_blob_t* blob);

/**
 * @brief Frees the memory of a face info object
 *
 *
 * @param face_info The face info object to deallocate
 */
void
xa_fi_face_info_free(xa_fi_vision_cell_t* fi, xa_fi_face_info_t* face_info);


///
/// Common face registration methods
///

/**
 * @brief In order to perform verification agains a list of faces, we need to first
 *        register the list of available faces.
 *        This function registers a new face object with a given ID into the vision cell.
 *        All the registered faces will be checked when running (only) verify mode.
 *        The face should be complete ( @ref xa_fi_face_complete() )
 *        When this function is called with an already registered identity, it will return XA_ERR_INVALID_ARGS.
 *
 * @param fi        The vision cell main object
 * @param face      The face object to register, must be a complete face (check @ref xa_fi_face_complete ).
 *                  The face information will be copied fater registering, so no need
 *                  of keeping the face referenced.
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_register_face(xa_fi_vision_cell_t* fi, const xa_fi_face_info_t* face);

/**
 * @brief Returns a struct listing all the currently registered faces
 *
 * @note IMPORTANT: the result will be valid only till the next time any
 *                  function in this file is called
 *
 * @param fi               The vision cell main object
 * @param registered_faces A struct listing the currently registered faces and
 *                         how many
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_get_registered_faces(xa_fi_vision_cell_t* fi, const xa_fi_registered_faces_t * * registered_faces);

/**
 * @brief Deregister an already registered identity. This will remove the identity from the identities library
 *        and verifications against that identity will not be longer performed.
 *        In case the identity does not exists this function will return XA_ERR_INVALID_ARGS
 *
 * @param fi        The vision cell main object
 * @param identity  The identity to deregister.
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_deregister_face(xa_fi_vision_cell_t* fi, const xa_fi_face_identity_t* identity);

/**
 * @brief Deregister all registered identity.
 *
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_deregister_all_faces(xa_fi_vision_cell_t* fi);

/**
 * @brief Updates an embedding for an already registered face.  Images may come
 *        in after a new gallery has been registered - this is how to updated
 *        them.
 *
 * @param fi                    The vision cell object
 * @param identity_id           The identity to update
 * @param image_id              The image id to update
 * @param embedding             The new embedding
 * @param vision_cell_id        The vision cell id
 * @param vision_cell_version   The vision cell version
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_update_embedding(xa_fi_vision_cell_t* fi, const char* identity_id, const char* image_id, xa_fi_float_array_t embedding,
                       const char * vision_cell_id, int vision_cell_version);

/**
 * @brief Updates an embedding for an already registered face.  Images may come
 *        in after a new gallery has been registered - this is how to updated
 *        them.
 *
 * @param fi                    The vision cell object
 * @param identity_id           The identity to update
 * @param image_id              The image id to update
 * @param image                 The image
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_update_embedding_from_registration_image(xa_fi_vision_cell_t* fi, const char* identity_id, const char* image_id, const xa_fi_image_t* image);

/**
 * @brief Search for the best identity matching on the gallery from a given embedding.
 *        The result is temporary kept till any other API function is called.
 *
 * @param fi            The vision cell object
 * @param embedding     The new embedding
 * @param result        The best matching result
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_find_best_matching_face(xa_fi_vision_cell_t* fi, const xa_fi_float_array_t* embedding, const xa_fi_face_best_matching_t** result);


///
/// API for running the 2 different modes: register / verify
/// Note that on some platforms (callback based, check below), this set of functions may not be enabled
/// (i.e. will return XA_ERR_NOT_IMPLEMENTED)
///

//
// Register mode
//

/**
 * @brief The registration mode is one of the 2 most important (supported) modes of the vision cell.
 *        In order to perform verification we need to first specify the persons we will be
 *        verifying against.
 *        Every person / face that we need to verify ( @ref fi_face.h file ) should be
 *        generated mainly from embeddings, can be one or multiple, for one or multiple poses
 *        depending on the configuration.
 *
 *        This function will run the vision cell in such a way that, given an @p image, will
 *        generate an embedding for it ( @p result ). If error occur or no faces are detected or encountered
 *        on the provided image, no results will be returned.
 *
 *        The application is responsible to handle how many embeddings and poses are still
 *        required to fullfill a face object. Also, the amount of faces that are needed to be
 *        created.
 *
 *
 * @note IMPORTANT: The vision cell keep only the last results of each run, so the if this function
 *                  is called again, any reference to the result object will be invalidated.
 *                  If the vision cell is unloaded or destroyed then this data will be invalidated.
 *
 * @param fi      The vision cell main object
 * @param image   The image to extract the registration results from. This should be a buffer containing
 *                an image in one of the supported formats.
 * @param result  The registration results on success | NULL on failure. This object is allocated
 *                by the vision cell and the memory is handled internally. Caller should NOT free
 *                this data.
 *                The data will be kept till the next call of this same function, or till the vision
 *                cell is unloaded / destroyed.
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_run_register_mode(xa_fi_vision_cell_t* fi, const xa_fi_image_t* image, const xa_fi_register_result_t** result);


/**
 * @brief The verification mode is one of the 2 most important (supported) modes of the vision cell.
 *        This mode process an image extracting one or multiple faces from it (based
 *        on settings) and performs verification of each of those faces agains the list
 *        of registered faces.
 *        The result of this function will be the list of evaluated faces, with their
 *        associated metadata, id (check @p xa_fi_verify_results_t for more information).
 *
 * @note IMPORTANT: the result will be valid only till the next call of this function.
 *                  any reference to the result will be invalidated
 *                  If the vision cell is unloaded or destroyed then this data will be invalidated.
 *
 * @param fi      The vision cell main object
 * @param image   The image to extract the faces and perform verification in one or many
 * @param result  The verification results on success | NULL on failure. This object is allocated
 *                by the vision cell and the memory is handled internally. Caller should NOT free
 *                this data.
 *                The data will be kept till the next call of this same function, or till the vision
 *                cell is unloaded / destroyed.
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_run_verify_mode(xa_fi_vision_cell_t* fi, const xa_fi_image_t* image, const xa_fi_verify_results_t** result);



///
/// Callback based execution methods
/// This will be only valid on platforms that AI models are executed using a callback method.
/// Otherwise these API will be disabled (XA_ERR_NOT_IMPLEMENTED)
///


/**
 * @brief Configures the callback-based mode setting the callbacks and mode to run.
 * @param fi        The vision cell main object
 * @param config    The configuration of the callback-based mode
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_cb_mode_configure(xa_fi_vision_cell_t* fi, const xa_fi_cb_mode_config_t* config);

/**
 * @brief Starts running the configured callback method (see @ref xa_fi_cb_mode_configure).
 *        The appropiate callback will be called after each obtained result
 *
 * @param fi        The vision cell main object
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_cb_mode_start(xa_fi_vision_cell_t* fi);

/**
 * @brief Stops capturing and running the configured pipeline.
 *
 * @param fi        The vision cell main object
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_cb_mode_stop(xa_fi_vision_cell_t* fi);


#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_VISION_CELL_H_
