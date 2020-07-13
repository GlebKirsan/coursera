#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

class Figure{
public:
    virtual string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle: public Figure{
public:
    Triangle(double a, double b, double c):
    a(a), b(b), c(c){}

    string Name() const override{
        return "TRIANGLE";
    }

    double Perimeter() const override{
        return a + b + c;
    }

    double Area() const override{
        double p = (a + b + c)/2;
        return sqrt(p * (p-a) * (p-b) * (p-c));
    }
private:
    double a, b, c;
};

class Rect: public Figure{
public:
    Rect(double a, double b):
            a(a), b(b){}

    string Name() const override{
        return "RECT";
    }

    double Perimeter() const override{
        return 2*(a + b);
    }

    double Area() const override{
        return a * b;
    }
private:
    double a, b;
};

class Circle: public Figure{
public:
    Circle(double r):
            r(r){}

    string Name() const override{
        return "CIRCLE";
    }

    double Perimeter() const override{
        return 3.14*2*r;
    }

    double Area() const override{
        return 3.14*r*r;
    }
private:
    double r;
};

shared_ptr<Figure> CreateFigure(istringstream &is){
    string fig;
    is >> fig;
    shared_ptr<Figure> ans;
    if (fig == "RECT"){
        double a, b;
        is >> a >> b;
        ans = make_shared<Rect>(a, b);
    } else if (fig == "CIRCLE"){
        double r;
        is >> r;
        ans = make_shared<Circle>(r);
    } else {
        double a, b, c;
        is >> a >> b >> c;
        ans = make_shared<Triangle>(a, b, c);
    }
    return ans;
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}