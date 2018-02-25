#include "file_load.h"

ofImage loadFromPath(string path) {

	ofFile file(path);

	string fileExtension = ofToUpper(file.getExtension());

	if (fileExtension == "JPG" || fileExtension == "PNG") {

		ofImage image = ofImage();
		image.load(path);
		return image;

	}
	else {

		ofLogError("Image must be of format JPG or PNG");

	}

}