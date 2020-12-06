#include "svpch.h"
#include "Batch.h"


Batch::Batch()
{
}

Batch::~Batch()
{
}


void Batch::push_back(const IndexedModel &model)
{
	if (indexed_model.vertices.size() == 0)
	{
		indexed_model.layout = model.layout;
		indexed_model.draw_type = model.draw_type;
	}
	else
	{
		ASSERT(indexed_model.layout.elements.size() == model.layout.elements.size(),
			   "Batching eror! Indexed Model Layout mismatch");

		for (int i = 0; i < indexed_model.layout.elements.size(); i++)
		{
			ASSERT(indexed_model.layout.elements[i].data_type ==
				   model.layout.elements[i].data_type,
			       "Batching eror! Indexed Model Layout mismatch");
		}
	}

	indexed_model.vertices.insert(
		indexed_model.vertices.end(),
		model.vertices.begin(),
		model.vertices.end());

// 	shift indexes
	vector<unsigned int> shifted_indices;
	shifted_indices.reserve(model.indices.size());
	for (int i = 0; i < model.indices.size(); i++)
	{
		shifted_indices.push_back(model.indices[i] + faces_shift);
	}

	indexed_model.indices.insert(
		indexed_model.indices.end(),
		shifted_indices.begin(),
		shifted_indices.end());

	faces_shift += model.indices.size();

}

// void Batch::make_batch()
// {
//     if (models.size() == 0)
//         return;
//
//     batch.vertices.clear();
//     batch.indices.clear();
//
//     int faces_shift = 0;
//     for (IndexedModel im : models)
//     {
//         batch.vertices.insert(batch.vertices.end(), im.vertices.begin(),
//                               im.vertices.end());
//
//         for (unsigned int i = 0; i < im.indices.size(); i++)
//             im.indices[i] += faces_shift;
//
//         batch.indices.insert(batch.indices.end(), im.indices.begin(),
//                              im.indices.end());
//
//         faces_shift += im.indices.size();
//     }
//
//     batch.layout = models[0].layout;
// 	batch.draw_type = models[0].draw_type;
//
// 	models.clear();
// }
