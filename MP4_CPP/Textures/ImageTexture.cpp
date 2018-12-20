#include "ImageTexture.h"

int hres;
int vres;
Image* image_ptr;
Mapping* mapping_ptr;

ImageTexture::ImageTexture(void):
    Texture(),
    hres(100),
    vres(100),
    image_ptr(NULL),
    mapping_ptr(NULL){}
ImageTexture::ImageTexture(Image* image_ptr, Mapping* mapping_ptr):
    Texture(),
    hres(image_ptr->hres),
    vres(image_ptr->vres),
    image_ptr(image_ptr),
    mapping_ptr(mapping_ptr){}
ImageTexture::ImageTexture(const ImageTexture& it):
    Texture(it),
    hres(it.hres),
    vres(it.vres){
    if (it.image_ptr){
        image_ptr = it.image_ptr;
    } else {
        image_ptr = NULL;
    }
    if (it.mapping_ptr){
        mapping_ptr = it.mapping_ptr->clone();
    } else {
        mapping_ptr = NULL;
    }
}
ImageTexture& ImageTexture::operator= (const ImageTexture& rhs){
    if (this == &rhs){
        return *this;
    }
    Texture::operator=(rhs);

    hres = rhs.hres;
    vres = rhs.vres;
    
    if (image_ptr){
        delete image_ptr;
        image_ptr = NULL;
    }
    if (rhs.image_ptr){
        image_ptr = rhs.image_ptr;
    }

    if (mapping_ptr){
        delete mapping_ptr;
        mapping_ptr = NULL;
    }
    if (rhs.mapping_ptr){
        mapping_ptr = rhs.mapping_ptr;
    }
    
    return *this;
}
ImageTexture* ImageTexture::clone(void) const{
    return new ImageTexture(*this);
}
ImageTexture::~ImageTexture(void){
    if (image_ptr) {
		delete image_ptr;
		image_ptr = NULL;
	}
	
	if (mapping_ptr) {
		delete mapping_ptr;
		mapping_ptr = NULL;
	}
}

Vector3D ImageTexture::get_color(const ShadeRec& sr) const{
    int row, col;

    if (mapping_ptr){
        mapping_ptr->get_texel_coordinates(sr.local_hit_point, hres, vres, row, col);
    } else {
        row = (int)(sr.v * (vres - 1));
        col = (int)(sr.u * (hres - 1));
    }
    return image_ptr->get_color(row, col);
};