#ifndef BATCH_H_
#define BATCH_H_

#include <iostream>
#include <stdio.h>
#include "IndexedModel.h"

using namespace std;

class Batch
{
public:
    Batch();
	~Batch();

	void make_batch();

	vector<IndexedModel> models;

	IndexedModel batch;
};

#endif /* BATCH_H_ */
