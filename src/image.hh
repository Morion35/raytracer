//
// Created by simon on 2/23/21.
//

#ifndef RAYTRACING_IMAGE_HH

#define RAYTRACING_IMAGE_HH

#include "type.hh"
#include <vector>
#include <string>
// #include <jpeglib.h>

namespace raytracing {

    class Image {

    public:
        Image(unsigned width, unsigned height) : width_{width}, height_{height} {
            data_.assign(height_, std::vector<color>(width_));
        };
        explicit Image(const std::string& ppm);

        void to_ppm(const std::string& filename) const;

        unsigned width() const { return width_; }
        unsigned height() const { return height_; }

        auto data() const { return data_; };
        void set_pixel(unsigned i, unsigned j, color c) {
            data_[i][j] = c;
        }

    private:
        unsigned width_;
        unsigned height_;
        std::vector<std::vector<color>> data_;
    };

}

#endif //RAYTRACING_IMAGE_HH
