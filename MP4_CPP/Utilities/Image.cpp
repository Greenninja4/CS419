#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Constants.h"
#include "Image.h"

// Big 6
Image::Image(void):
    hres(100),
    vres(100) {}
Image::Image(const Image& image):
    hres(image.hres),
    vres(image.vres),
    pixels(image.pixels) {}
Image& Image::operator= (const Image& rhs){
    if (this == &rhs){
        return *this;
    }
    hres = rhs.hres;
    vres = rhs.vres;
    pixels = rhs.pixels;
    return *this;
}
Image::~Image(void){}

// Functions
void Image::read_ppm_file(const char* file_name){

    // Open file
    FILE* file = fopen(file_name, "r");
    
    if (file == 0){
		cout << "PPM image could not be opened." << endl;
	}

    // PPM header
    char ppm_type;
    if (fscanf(file, "P%c\n", &ppm_type) != 1){
		cout << "PPM image missing signature \"P3\"" << endl;
	}
    if (ppm_type != '3'){
		cout << "PPM image not type \"P3\"" << endl;
	}

    // Skip comments
    char dummy;
    while (fscanf(file ,"#%c", &dummy)) 
        while (fgetc(file) != '\n');

    // Image size
    if (fscanf(file, "%d %d\n", &hres, &vres) != 2){
		cout << "Invalid image size" << endl;
	}

    // Max pixel strength (usually 255)
    int max_value;
    if (fscanf(file, "%d\n", &max_value) != 1){
		cout << "Invalid max value" << endl;
	}
	float inv_max_value = 1.0 / (float)max_value;

    // Read in image data
    pixels.reserve(hres * vres);    
    for (int y = 0; y < vres; y++) {
        for (int x = 0; x < hres; x++) {
            char red;
            char green;
            char blue;
            
            if (fscanf(file, "%c%c%c", &red, &green, &blue) != 3) {
				cout << "Invalid image" << endl;
			}

			float r = red   * inv_max_value;
			float g = green * inv_max_value;
			float b = blue  * inv_max_value;

			pixels.push_back(Vector3D(r, g, b));
        }
    }

    // Close file
    fclose(file);
	cout << "Finished reading PPM image" << endl;

}

Vector3D Image::get_color(const int row, const int col) const{
    int index = col + hres * (vres - row - 1);
    if (index < pixels.size()){
        return pixels[index];
    } else {
        return BLACK;
    }
}