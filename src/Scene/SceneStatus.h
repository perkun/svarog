#ifndef SCENESTATUS_H_
#define SCENESTATUS_H_

#include <iostream>
#include <stdio.h>

using namespace std;

class SceneStatus
{
public:
	SceneStatus();
    SceneStatus(bool);
	~SceneStatus();

	bool active;
};

#endif /* SCENESTATUS_H_ */
