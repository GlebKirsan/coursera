#include "rectangle.h"

class Rectangle {
public:
    Rectangle(int width, int height) : width_(width), height_(height)
    {
    }

    int GetArea() const {
        return width_ * height_;
    }

    int GetPerimeter() const {
        return 2 * (width_ + height_);
    }

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

private:
    int width_, height_;
};