#include "Point.h"

ostream & operator << (ostream &out, const Point &point){
    out << "x = " << point.x;
    out << "y = " << point.y;
    out << "z = " << point.z;
    out << "\n";

}
