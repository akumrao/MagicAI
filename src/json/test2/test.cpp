

#include <string>

#include <xailient-fi/sdk_json_interface.h>

#include <iostream>



int main(int argc, char** argv) {



    std::cout << "Hello, world!" << std::endl;


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
