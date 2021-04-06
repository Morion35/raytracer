//
// Created by simon on 2/23/21.
//

#include "image.hh"
#include <iostream>
#include <fstream>

namespace raytracing {
    Image::Image(const std::string& filename) {
        std::ifstream ss(filename);
        if (!ss) {
            std::cerr << "Could not open file: " << filename << '\n';
        }
        std::string magic;
        unsigned maxval;
        ss >> magic >> width_ >> height_ >> maxval;

        data_.reserve(height_);
        char r;
        char g;
        char b;
        for (size_t j = 0; j < height_; ++j) {
            std::vector<color> row(width_);
            for (size_t i = 0; i < width_; ++i) {
                ss.read(&r, 1);
                ss.read(&g, 1);
                ss.read(&b, 1);
                row[i] = color(r, g, b);
            }
            data_.push_back(row);
        }
    }
    void Image::to_ppm(const std::string& filename) const {
        std::ofstream ppm(filename);

        ppm << "P6" << std::endl << width_ << std::endl << height_ << std::endl << "255" << std::endl;
        for (const auto& row : data_) {
            for (const auto& pixel : row) {
                ppm << (unsigned char) (pixel.r * 255) << (unsigned char) (pixel.g * 255) << (unsigned char) (pixel.b * 255);
            }
        }
    }
}