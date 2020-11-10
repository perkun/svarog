#include "Batch.h"

Batch::Batch()
{
}

Batch::~Batch()
{
}

void Batch::make_batch()
{
    if (models.size() == 0)
        return;

    batch.vertices.clear();
    batch.indices.clear();

    int faces_shift = 0;
    for (IndexedModel im : models)
    {
        batch.vertices.insert(batch.vertices.end(), im.vertices.begin(),
                              im.vertices.end());

        for (unsigned int i = 0; i < im.indices.size(); i++)
            im.indices[i] += faces_shift;

        batch.indices.insert(batch.indices.end(), im.indices.begin(),
                             im.indices.end());

        faces_shift += im.indices.size();
    }

    batch.layout = models[0].layout;

	models.clear();
}
