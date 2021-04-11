//
// Created by simon on 2/23/21.
//

#include "image.hh"
#include <iostream>
#include <fstream>

namespace raytracing {
    Image::Image(const std::string& filename)
    {
        std::ifstream file = std::ifstream(filename, std::ios::binary);

        if (!file.is_open())
        {
            std::cout << "Could not access '" << filename << "'\n";
            return;
        }

        std::string file_type;
        file >> file_type;

        if (file_type != "P6")
        {
            std::cout << "'" << filename << "' has wrong file type\n";
            return;
        }

        file >> width_;
        file >> height_;

        unsigned max;
        file >> max;

        char ignore;
        file.read(&ignore, 1);

        data_ = std::vector<std::vector<color>>();
        data_.assign(height_, std::vector<color>(width_));

        for (size_t i = 0; i < height_; ++i)
        {
            for (size_t j = 0; j < width_; ++j)
            {
                char r;
                char g;
                char b;

                file.read(&r, 1);
                file.read(&g, 1);
                file.read(&b, 1);

                data_[i][j] = color((unsigned char) r, (unsigned char) g, (unsigned char) b);
            }
        }

        file.clear();
        file.close();
    }

    void Image::to_ppm(const std::string& filename) const {
        std::ofstream ppm(filename, std::ios::binary);

        ppm << "P6" << std::endl << width_ << std::endl << height_ << std::endl << "255" << std::endl;

        for (const auto& row : data_) {
            for (const auto& pixel : row) {
                ppm << (unsigned char) (pixel.r * 255) << (unsigned char) (pixel.g * 255) << (unsigned char) (pixel.b * 255);
            }
        }
    }
}