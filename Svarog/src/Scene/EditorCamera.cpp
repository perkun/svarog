#include "svpch.h"
#include "EditorCamera.h"


EditorCamera::EditorCamera(float fov, float aspect, float zNear, float zFar)
 : PerspectiveCamera(fov, aspect, zNear, zFar)
{

}


EditorCamera::~EditorCamera()
{
}


