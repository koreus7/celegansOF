option(RUN_TESTS "Run the tests" OFF)

# ======================= ofxCMake Vers. 0.1 =============

# ========================================================
# ===================== CMake Settings ===================
# ========================================================
cmake_minimum_required( VERSION 3.3 )
project( openframeworks )

# ========================================================
# ===================== User Settings ====================
# ========================================================
# ---------------------- App name  -----------------------
set( APP_NAME   ofApp )

# ------------------------ OF Path -----------------------
# --- If outside the OF structure, set an absolute OF path
set( OF_DIRECTORY_BY_USER "../../.." )



if(RUN_TESTS)
    set( TEST_FILES
            src/tests/serializable_test.h )
else()
    set( TEST_FILES "" )
endif()

# --------------------- Source Files ---------------------
set( ${APP_NAME}_SOURCE_FILES
        src/main.cpp
        src/ofApp.cpp
        src/app_state.cpp
        src/beam_parameters.cpp
        src/beam_tweaker.cpp
        src/serializable.cpp
        src/serializable_library.cpp
        src/point_selector.cpp
        src/imgui_utils.cpp
        src/beam_skeleton.cpp
        src/squared_error.cpp
        src/file_load.cpp
        src/constants.h
        src/experiment_metadata.cpp
        ${TEST_FILES}
                )


# ------------------------ AddOns  -----------------------
set( OFX_ADDONS_ACTIVE
        #        ofxXmlSettings
        #        ofxOsc
        #        ofxGui
        #        ofxNetwork
        #        ofxVectorGraphics
        #        ofxThreadedImageLoader
        #        ofxAssimpModelLoader
        #        ofxKinect
                 ofxOpenCv
                 ofxJSON
                 ofxImGui
        #        ofxSvg
        #        ofxEmscripten (untested)
                )

# =========================================================================
# ============================== OpenFrameworks ===========================
# =========================================================================
include( ${OF_DIRECTORY_BY_USER}/addons/ofxCMake/modules/main.cmake )
# =========================================================================


if(RUN_TESTS)
    set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/catch)
    add_library(Catch INTERFACE)
    target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
    target_link_libraries( ${APP_NAME} Catch )
    add_definitions(-DRUN_TESTS)
endif()