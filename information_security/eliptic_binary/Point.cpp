#include "Point.h"

ostream & operator << (ostream &out, const Point &point){
    out << "x = " << point.x;
    out << "y = " << point.y;
    out << "on curve = " << point.is_on_curve();
    out << "\n";
}
