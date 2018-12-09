#ifndef __IMAGE_TEXTURE__
#define __IMAGE_TEXTURE__

// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
#include <string>
#include <Magick++.h>
#include "Mapping.h"
#include "Texture.h"

class ImageTexture: public Texture {
	public:
        // constructors, access functions, etc.
        ImageTexture();
        ImageTexture(const std::string& filename);
        ImageTexture(const ImageTexture& cc);
        ImageTexture& operator= (const ImageTexture& rhs);
        virtual ~ImageTexture();
        virtual ImageTexture* clone() const;

		void set_mapping(Mapping* mapping);
		void set_image(const std::string& image);
		void set_image(Magick::Image* image);

        virtual RGBColor get_color(const ShadeRec& sr) const;

    private:
		Magick::Image* image_ptr;         // the image
		Mapping* mapping_ptr;     // mapping technique used, if any
};

inline void ImageTexture::set_mapping(Mapping* mapping){
	if(mapping_ptr){
		delete mapping_ptr;
	}
	mapping_ptr = mapping;
}

inline void ImageTexture::set_image(Magick::Image* image){
	if(image_ptr){
		delete image_ptr;
	}
	image_ptr = image;
}

inline void ImageTexture::set_image(const std::string& filename){
	if(image_ptr){
		delete image_ptr;
	}
	image_ptr = new Magick::Image(filename);
}

#endif
