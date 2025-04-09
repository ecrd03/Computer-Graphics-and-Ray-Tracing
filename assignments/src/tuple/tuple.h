#ifndef TUPLE
#define TUPLE

typedef struct tuple_struct
{
    double x;
    double y;
    double z;
    double w;
} Tuple;

typedef struct color_struct
{
    double red;
    double green;
    double blue;
} Color;

/* Point and Vectors, using generic 4-Tuples*/
unsigned int is_point(Tuple);
unsigned int is_vector(Tuple);

Tuple tuple(double, double, double, double);
Tuple point(double, double, double);
Tuple vector(double, double, double);
Tuple addTuples(Tuple, Tuple);
Tuple subTuples(Tuple, Tuple);
Tuple negateTuple(Tuple);
Tuple scalarMultTuple(double, Tuple);
Tuple scalarDivideTuple(Tuple, double);
double magnitudeOfVector(Tuple);
Tuple normalizeVector(Tuple);
double dotProduct(Tuple, Tuple);

#if 0 // not needed for limited raycaster project CSC 251
Tuple crossProduct(Tuple, Tuple);
#endif

/* Color ops, using specialized three-component color struct */
Color color(double, double, double);
Color addColors(Color, Color);
Color subColors(Color, Color);
Color scalarMultColor(double, Color);
Color multColors(Color, Color);

#endif



