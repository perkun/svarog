#ifndef BATCH_H_
#define BATCH_H_

#include <iostream>
#include <stdio.h>
#include "IndexedModel.h"
#include "Core.h"

using namespace std;

class Batch
{
public:
    Batch();
	~Batch();

	void push_back(const IndexedModel &model);

	IndexedModel indexed_model;;
private:
	int faces_shift = 0;
};

#endif /* BATCH_H_ */
