// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class ImageTexture
#include <Magick++.h>
#include <iostream>
#include "ImageTexture.h"

using namespace Magick;

ImageTexture::ImageTexture()
: image_ptr(NULL), mapping_ptr(NULL)
{}

ImageTexture::ImageTexture(const std::string& filename)
: image_ptr(new Image(filename)), mapping_ptr(NULL)
{}

ImageTexture::ImageTexture(const ImageTexture& cc)
{}

ImageTexture& ImageTexture::operator= (const ImageTexture& rhs)
{
    Texture::operator=(rhs);
    if(image_ptr){
        delete image_ptr;
    }
    image_ptr = rhs.image_ptr;
    if(mapping_ptr){
        delete mapping_ptr;
    }
    mapping_ptr = rhs.mapping_ptr->clone();
    return (*this);
}

ImageTexture::~ImageTexture()
{
}

ImageTexture* ImageTexture::clone() const
{
    return new ImageTexture(*this);
}

RGBColor ImageTexture::get_color(const ShadeRec& sr) const {
    int row, column;

    if (mapping_ptr){
        mapping_ptr->get_texel_coordinates(sr.local_hit_point, (int) image_ptr->columns(), (int) image_ptr->rows(), row, column);
    }
    else {
        row = (int)(sr.v * (image_ptr->rows() - 1));
        column = (int)(sr.u * (image_ptr->columns() - 1));
    }

    ColorRGB color = image_ptr->pixelColor(column, image_ptr->rows() - 1 - row);
    return (RGBColor(color.red(), color.green(), color.blue()));
}
