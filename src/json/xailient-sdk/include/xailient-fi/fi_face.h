/**
* Copyright © 2022 Xailient Inc. All rights reserved
*/


#ifndef XAILIENT_FI_FACE_H_
#define XAILIENT_FI_FACE_H_

// std
#include <stdlib.h>
// cailient
#include <xailient-fi/fi_defs.h>
#include <xailient-fi/fi_common_types.h>
#include <xailient-fi/fi_blob.h>



#ifdef __cplusplus
extern "C" {
#endif

typedef void xa_fi_face_registrar_t;
typedef void xa_fi_face_info_t;

typedef struct
{
    // number of registered faces
    unsigned int number_of_registered_faces;
    // array of pointers of registered face_info objects
    const xa_fi_face_info_t * * registered_faces;
} xa_fi_registered_faces_t;


/**
 * @brief Register an embedding to the given face.
 *        The embedding can be obtained running the register mode of the fi
 *        vision cell
 *
 * @param face          The face registrar object
 * @param embedding     The embedding to be registered
 * @return xa_fi_error_t
 */
xa_fi_error_t
xa_fi_face_registrar_register_embedding(xa_fi_face_registrar_t* face_reg, const xa_fi_float_array_t* embedding);

/**
 * @brief Checks if the face is already complete and can be used for verification.
 *        If some pose still needs more embeddings, this will return false
 *
 * @param face      The face registrar object
 * @return true     if the face is already complete and can be used for verification
 * @return false    otherwise
 */
bool
xa_fi_face_registrar_complete(const xa_fi_face_registrar_t* face_reg);

/**
 * @brief Returns the processed face information to be registered on the vision
 *        cell for verification ( @ref xa_fi_register_face ).
 *        The face will be available after @ref xa_fi_face_registrar_complete returns true
 *        and till the xa_fi_face_registrar_t is destroyed.
 *
 * @param face  The face registrar object
 * @return const xa_fi_face_info_t*
 */
xa_fi_face_info_t*
xa_fi_face_registrar_face_information(xa_fi_face_registrar_t* face_reg);

///
/// Face info interface
///

/**
 * @brief Returns the identity associated to a face info object.
 *        The result object contains references to the face_info object, and those will be valid
 *        as long as the face_info object is not modified / deleted. The caller should not keep a reference
 *        of this object if needs to keep the information longer than the lifetime-scope of the face_info.
 *
 * @param face_info     The face info
 * @return const xa_fi_face_identity_t*
 */
xa_fi_face_identity_t
xa_fi_face_info_identity(const xa_fi_face_info_t* face_info);

/**
 * @brief Sets a new identity to the face_info object. The @p identity is copied into the
 *        face_info object so the caller can safely remove it after this call.
 *        Updating a face info after calling @c xa_fi_register_face() has no effect on the registered face,
 *        xa_fi_register_face makes a copy of it. In order to reflect changes the face info should be
 *        re-registered
 *
 * @param face_info The face info object
 * @param identity  The identity
 * @return xa_fi_error_t associated error
 */
xa_fi_error_t
xa_fi_face_info_set_identity(xa_fi_face_info_t* face_info, const xa_fi_face_identity_t* identity);


#ifdef __cplusplus
}
#endif


#endif // XAILIENT_FI_FACE_H_
