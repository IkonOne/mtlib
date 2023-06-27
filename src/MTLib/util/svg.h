#ifndef _MTLIB_SVG_H_
#define _MTLIB_SVG_H_

#include <fstream>
#include <string>
#include <sstream>

namespace mtlib {

class svg {
public:
    static float stroke_width;
    static float point_size;
    static std::string stroke_color;
    static std::string fill_color;
    static std::string point_color;

    static void begin(int width, int height) {
        _ss << "<?xml version=\"1.0\" standalone=\"no\" ?>\n";
        _ss << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 " << width << ' ' << height <<  "\">\n";
        _ss << "\t<rect width=\"100%\" height=\"100%\" fill=\"white\" />\n";
    }

    static void end(const std::string& filename) {
        _ss << "</svg>\n";
        std::ofstream fs(filename, std::ofstream::out | std::ofstream::trunc);
        fs << _ss.str();
        fs.close();
    }

    template <typename Scalar>
    static void point(Scalar x, Scalar y) {
        _ss << "\t<circle ";
        _ss << "cx=\"" << x << "\" ";
        _ss << "cy=\"" << y << "\" ";
        _ss << "r=\"" << point_size << "\" ";
        _ss << "fill=\"" << point_color << "\" ";
        _ss << "/>\n";
    }

    template <typename Scalar = float, typename ForwardIt>
    static void points(ForwardIt first, ForwardIt last) {
        for (auto it = first; it != last; ++it) {
            point((*it)[0], (*it)[1]);
        }
    }

    template <typename Scalar>
    static void line(Scalar x1, Scalar y1, Scalar x2, Scalar y2) {
        _ss << "\t<line ";
        _ss << "x1=\"" << x1 << "\" y1=\"" << y1 << "\" ";
        _ss << "x2=\"" << x2 << "\" y2=\"" << y2 << "\" ";
        _ss << "stroke=\"" << stroke_color << "\" ";
        _ss << "stroke-width=\"" << stroke_width << "\" ";
        _ss << "/>\n";
    }

    template <typename Scalar = float, typename ForwardIt>
    static void polygon(ForwardIt first, ForwardIt last) {
        _ss << "\t<polygon ";
        _ss << "points=\"";
        for (auto point_it = first; point_it != last; ++point_it) {
            _ss << (*point_it)[0] << ',' << (*point_it)[1] << ' ';
        }
        _ss << "\" ";
        _ss << "fill=\"" << fill_color << "\" ";
        _ss << "stroke=\"" << stroke_color << "\" ";
        _ss << "stroke-width=\"" << stroke_width << "\" ";
        _ss << "/>\n";
    }

private:
    static std::stringstream _ss;
};

float svg::stroke_width = 0.5f;
float svg::point_size = 1.3f;
std::string svg::fill_color = "#00695C11";
std::string svg::stroke_color = "black";
std::string svg::point_color = "#EF5350FF";
std::stringstream svg::_ss;

}

#endif