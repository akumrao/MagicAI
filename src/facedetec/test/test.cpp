

#include <string>

#include <xailient-fi/sdk_json_interface.h>
#include <iostream>


#define JSON_ASSERT(x) /* value */

#include <json/json.hpp>
using json = nlohmann::json;



int main(int argc, char** argv) {

    json test = json::object();

    // test["arv"] = 1;

    std::string msg = "Hello, world!";

    std::cout << msg << std::endl;


    {
      //   printf("start of facial rec.\n");

        xa_fi_error_t returnValue;

        const char * path_to_vision_cell ="/mnt"; // For shared lib
        returnValue = xa_sdk_initialize(path_to_vision_cell); // For shared lib

       // returnValue = xa_sdk_initialize(); // For static lib

        if (returnValue != XA_ERR_NONE) {

           
            //printf("error1\n");

         }

     }


     {

            xa_fi_error_t returnValue;

        const char * path_to_vision_cell = "/mnt/libxailient-fi-vcell.so"; // For shared lib
        returnValue = xa_sdk_initialize(path_to_vision_cell); // For shared lib

       // returnValue = xa_sdk_initialize(); // For static lib

        if (returnValue != XA_ERR_NONE) {

           
            // printf("error2\n");
         
         }



     }





}
