#include "ofMain.h"
#include "ofApp.h"

#ifdef RUN_TESTS
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "../tests/serializable_test.h"
#endif

//========================================================================
int main(int argc, char* argv[]) {


#ifdef RUN_TESTS

    std::cout<< "Running Catch Tests" << std::endl;
    int result = Catch::Session().run( argc, argv );
	return result;

#endif


    ofGLWindowSettings settings;

	settings.width = 1024;
	settings.height = 768;
	settings.setGLVersion(3, 2);
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
