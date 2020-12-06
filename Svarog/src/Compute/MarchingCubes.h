#ifndef MARCHINGCUBES_H_
#define MARCHINGCUBES_H_

#include "IndexedModel.h"
#include "VolumetricData.h"

using namespace std;
using namespace glm;

#define ABS(x) (x < 0 ? -(x) : (x))

namespace MarchingCubes
{

typedef struct
{
    double x, y, z;
} XYZ;

typedef struct
{
    XYZ p[8];
    XYZ n[8];
    double val[8];
} GRIDCELL;

typedef struct
{
    XYZ p[3]; /* Vertices */
    XYZ c;    /* Centroid */
    XYZ n[3]; /* Normal   */
} TRIANGLE;

XYZ VertexInterp(double isolevel, XYZ p1, XYZ p2, double valp1, double valp2);
int PolygoniseCube(GRIDCELL g, double iso, TRIANGLE *tri);

// 	#define index(i, j, k) (k) * dim_y * dim_x + (j) * dim_x + (i)

template <typename T>
IndexedModel polygonise_space(VolumetricData<T> *vol_data, T isolevel)
{
    unsigned int dim_x = vol_data->dim_x;
    unsigned int dim_y = vol_data->dim_y;
    unsigned int dim_z = vol_data->dim_z;

    T *data = vol_data->get_data();

    IndexedModel model;

    unsigned int i, j, k;
    int l, n;
    GRIDCELL grid;
    TRIANGLE triangles[10];
    TRIANGLE *tri =
        (TRIANGLE *)malloc(dim_x * dim_y * dim_x * 8 * sizeof(TRIANGLE));
    int ntri = 0;

    // data layout: data[z][y][x] -> data(xyz) = data[z*dim_y + y*dim_x + x]

    for (i = 0; i < dim_x - 1; i++)
    {
        //         if (i % (dim_x / 10) == 0)
        //             fprintf(stderr, "   Slice %d of %d\n", i, dim_x);
        for (j = 0; j < dim_y - 1; j++)
        {
            for (k = 0; k < dim_z - 1; k++)
            {

                // i, j, k => x, y, z
                grid.p[0].x = i;
                grid.p[0].y = j;
                grid.p[0].z = k;
                grid.val[0] = (double)data[k * dim_y * dim_x + j * dim_x + i];
                grid.p[1].x = i + 1;
                grid.p[1].y = j;
                grid.p[1].z = k;
                grid.val[1] =
                    (double)data[k * dim_y * dim_x + j * dim_x + i + 1];
                grid.p[2].x = i + 1;
                grid.p[2].y = j + 1;
                grid.p[2].z = k;
                grid.val[2] =
                    (double)data[k * dim_y * dim_x + (j + 1) * dim_x + i + 1];
                grid.p[3].x = i;
                grid.p[3].y = j + 1;
                grid.p[3].z = k;
                grid.val[3] =
                    (double)data[k * dim_y * dim_x + (j + 1) * dim_x + i];
                grid.p[4].x = i;
                grid.p[4].y = j;
                grid.p[4].z = k + 1;
                grid.val[4] =
                    (double)data[(k + 1) * dim_y * dim_x + j * dim_x + i];
                grid.p[5].x = i + 1;
                grid.p[5].y = j;
                grid.p[5].z = k + 1;
                grid.val[5] =
                    (double)data[(k + 1) * dim_y * dim_x + j * dim_x + i + 1];
                grid.p[6].x = i + 1;
                grid.p[6].y = j + 1;
                grid.p[6].z = k + 1;
                grid.val[6] = (double)
                    data[(k + 1) * dim_y * dim_x + (j + 1) * dim_x + i + 1];
                grid.p[7].x = i;
                grid.p[7].y = j + 1;
                grid.p[7].z = k + 1;
                grid.val[7] =
                    (double)data[(k + 1) * dim_y * dim_x + (j + 1) * dim_x + i];

                n = PolygoniseCube(grid, (double)isolevel, triangles);
                //                 tri = (TRIANGLE *)realloc(tri, (ntri + n)
                //                 * sizeof(TRIANGLE));
                for (l = 0; l < n; l++)
                    tri[ntri + l] = triangles[l];
                ntri += n;
            }
        }
    }

    model.vertices.reserve(ntri * 3 * (3 + 3));
    model.indices.reserve(ntri * 3);

    for (i = 0; i < ntri; i++)
    {
        for (k = 0; k < 3; k++)
        {
            model.vertices.emplace_back(tri[i].p[k].x - dim_x / 2.);
            model.vertices.emplace_back(tri[i].p[k].y - dim_y / 2.);
            model.vertices.emplace_back(tri[i].p[k].z - dim_z / 2.);
            // normal
            model.vertices.emplace_back(tri[i].n[k].x);
            model.vertices.emplace_back(tri[i].n[k].y);
            model.vertices.emplace_back(tri[i].n[k].z);

            // color (white for now)
            // 			model.vertices.push_back(1.);
            // 			model.vertices.push_back(1.);
            // 			model.vertices.push_back(1.);
            // 			model.vertices.push_back(1.);

            //             fprintf(outf, "v %lf %lf %lf\n",
            //                     // zamieniona os x z osia z
            //                     tri[i].p[k].z - NX / 2.,
            // 					tri[i].p[k].y - NY / 2.,
            //                     tri[i].p[k].x - NZ / 2.);
        }
        // zamieniona os x z osia z
        //         fprintf(outf, "f %d %d %d\n", (i * 3) + 1, (i * 3) + 3,
        //         (i * 3) + 2);
        model.indices.emplace_back((i * 3) + 0);
        model.indices.emplace_back((i * 3) + 2);
        model.indices.emplace_back((i * 3) + 1);
    }

    model.layout.elements.push_back(VertexDataType::FLOAT3);
    model.layout.elements.push_back(VertexDataType::FLOAT3);

    return model;
}
} // namespace MarchingCubes

#endif /* MARCHINGCUBES_H_ */
