#include "SceneStatus.h"

SceneStatus::SceneStatus() : active(false)
{
}

SceneStatus::SceneStatus(bool active, bool render)
    : active(active), render(render)
{
}

SceneStatus::~SceneStatus()
{
}
