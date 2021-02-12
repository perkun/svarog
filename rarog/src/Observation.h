#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include "Entity.h"

using namespace std;

class Observation
{
public:
	Observation();
	virtual ~Observation();

	Entity target, observer, ghost_target, ghost_observer;
};

#endif /* OBSERVATION_H_ */
