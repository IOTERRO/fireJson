# This directory contains the required nlohmann-json library, specifically the json-develop subdirectory, which is necessary for the proper functioning of this module. 

# The json-develop directory contains the source code for the library, and it needs to be included in the build process to link against the library. 

# If this directory is missing or not properly included in the build process, the jsonLib module will not work as intended. Therefore, it is crucial to ensure that this directory is present and correctly referenced in the module's cmake file (jsonLib.cmake).

# To install the nlohmann json library, you can visit their official GitHub repository

    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version xx.xx.xx
|_____|_____|_____|_|___|  https://github.com/nlohmann/json