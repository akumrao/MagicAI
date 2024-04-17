#include "conftests.h"
#include <json/json.hpp>
using json = nlohmann::json;

#include "json/configuration.h"
//#include "json/json.hpp"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

using std::cout;
using std::cerr;
using std::endl;



//using namespace nlohmann;
using namespace base;
using namespace base::cnfg;


#include <xailient-fi/sdk_json_interface.h>



int main(int argc, char** argv) {


    //Logger::instance().add(new RotatingFileChannel("test", "/tmp/test", Level::Trace, "log", 10));


    Logger::instance().add(new ConsoleChannel("debug", Level::Trace));



    // =========================================================================
    
    cnfg::Configuration config;

    config.load("./test2.json");

    std::string key = "version"; 
    config.setRaw(key, "22") ;

    std::string value;
    config.getRaw(key , value);
    

    config.save();

    {
         STrace <<  "start of facial rec.";

        xa_fi_error_t returnValue;

        const char * path_to_vision_cell ="/mnt"; // For shared lib
        returnValue = xa_sdk_initialize(path_to_vision_cell); // For shared lib

       // returnValue = xa_sdk_initialize(); // For static lib

        if (returnValue != XA_ERR_NONE) {

           
           STrace <<   "Error ";

         }

     }


     {

            xa_fi_error_t returnValue;

        const char * path_to_vision_cell = "/mnt/libxailient-fi-vcell.so"; // For shared lib
        returnValue = xa_sdk_initialize(path_to_vision_cell); // For shared lib

       // returnValue = xa_sdk_initialize(); // For static lib

        if (returnValue != XA_ERR_NONE) {

           
           STrace <<   "Error ";

         }



     }







}
