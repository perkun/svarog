#ifndef MARCHINGCUBES_H_
#define MARCHINGCUBES_H_

#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include "IndexedModel.h"

#define ABS(x) (x < 0 ? -(x) : (x))



using namespace std;
using namespace glm;

typedef struct {
   double x,y,z;
} XYZ;

typedef struct {
   XYZ p[8];
   XYZ n[8];
   double val[8];
} GRIDCELL;

typedef struct {
   XYZ p[3];         /* Vertices */
   XYZ c;            /* Centroid */
   XYZ n[3];         /* Normal   */
} TRIANGLE;


class MarchingCubes
{
public:
	IndexedModel polygonise_space(short int *data, unsigned int dim_x,
                                  unsigned int dim_y, unsigned int dim_z,
                                  short int isolevel);

private:
    int PolygoniseCube(GRIDCELL, double, TRIANGLE *);
    XYZ VertexInterp(double, XYZ, XYZ, double, double);

	unsigned int dim_x, dim_y, dim_z;
	inline unsigned int index(int i, int j, int k) {
		return k * dim_y*dim_x + j * dim_x + i;
	}
};

#endif /* MARCHINGCUBES_H_ */
