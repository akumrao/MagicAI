
#include "base/test.h"
#include "base/logger.h"
#include "base/filesystem.h"
#include "base/platform.h"

#include <json/json.hpp>

using json = nlohmann::json;

#include "json/configuration.h"
//#include "json/json.hpp"
#include <fstream>   // std::ifstream
#include <iostream>  // std::cout

#include <chrono>
#include <thread>

using std::cerr;
using std::cout;
using std::endl;

// using namespace nlohmann;
using namespace base;
using namespace base::cnfg;

#include <xailient-fi/sdk_json_interface.h>

int start_video_t31();

int main(int argc, char** argv) {
    // Logger::instance().add(new RotatingFileChannel("test", "/tmp/test",
    // Level::Trace, "log", 10));

    Logger::instance().add(new ConsoleChannel("debug", Level::Trace));

    // =========================================================================



    start_video_t31();

    // cnfg::Configuration config;

    // config.load("./config.json");

    // std::string xaconfig = config.root.dump();

    // xa_fi_error_t returnValue;

    // const char* path_to_vision_cell =
    //     "/mnt/libxailient-fi-vcell.so";                    // For shared lib
    // returnValue = xa_sdk_initialize(path_to_vision_cell);  // For shared lib

    // // returnValue = xa_sdk_initialize(); // For static lib

    // if (returnValue != XA_ERR_NONE) {
    //     SError << "Error at xa_sdk_initialize";

    //     return -1;
    // }

    // const char* configuration = xaconfig.c_str();

    // STrace << "config json: " << configuration;

    // // xa_sdk_update_identities
    // // xa_sdk_update_identity_image
    // returnValue = xa_sdk_configure(configuration);
    // if (returnValue != XA_ERR_NONE) {
    //     SError << "Error at xa_sdk_configure";

    //     return -1;
    // }

    // returnValue = xa_sdk_is_face_recognition_enabled();
    // if (returnValue != XA_ERR_NONE) {
    //     SError << "Error at xa_sdk_configure";

    //     return -1;
    // }

    // xa_fi_image_t image;
    // image.width = 800;
    // image.height = 640;

    // image.pixel_format =
    //     XA_FI_COLOR_RGB888;  // signifies the buffer data format

    // uint8_t* buffer_containing_raw_rgb_data = new uint8_t [image.width*3*image.height];

    // memset(buffer_containing_raw_rgb_data, 't', image.width*3*image.height);


    // image.buff =  buffer_containing_raw_rgb_data;  // note this is in RGB order, otherwise
    //                                      // colors will be swapped

    // //xa_fi_error_t returnValue;

    // xa_sdk_process_image_outputs* process_image_outputs;

    // while (1) {
    //     returnValue = xa_sdk_process_image(&image, &process_image_outputs);

    //     if (returnValue == XA_ERR_NONE) {
    //         for (int index = 0;
    //              index < process_image_outputs->number_of_json_blobs; ++index) {
    //             xa_sdk_json_blob_t blob = process_image_outputs->blobs[index];

    //             if (blob.blob_descriptor == XA_FACE_TRACK_EVENT) {
    //                 //<
    //                 // send blob -> json to Face Track Event endpoint >
    //                 STrace << "json to Face Track Event endpoint: " <<  blob.json;

    //             } else if (blob.blob_descriptor == XA_ACCURACY_MONITOR) {
    //                 //<
    //                 // send blob -> json to Accuracy Monitor endpoint >
    //                 STrace << "send blob -> json to Accuracy Monitor endpoint: " <<  blob.json;
    //             } else {
    //                 SError << "Not a possible state";
    //             }
    //         }
    //     } else {
    //         SError << "Error at process_image_outputs";
    //     }

    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // }
}