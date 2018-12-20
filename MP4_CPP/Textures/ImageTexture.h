#ifndef __IMAGE_TEXTURE__
#define __IMAGE_TEXTURE__

#include "Image.h"
#include "Mapping.h"
#include "Texture.h"

class ImageTexture : public Texture{
    public:
        int hres;
        int vres;
        Image* image_ptr;
        Mapping* mapping_ptr;

        ImageTexture(void);
        ImageTexture(Image* image_ptr, Mapping* mapping_ptr);
        ImageTexture(const ImageTexture& it);
        ImageTexture& operator= (const ImageTexture& rhs);
        virtual ImageTexture* clone(void) const;
        virtual ~ImageTexture(void);

        virtual Vector3D get_color(const ShadeRec& sr) const;
};

#endif