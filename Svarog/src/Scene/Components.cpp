#include "svpch.h"
#include "Components.h"


MeshComponent::MeshComponent(shared_ptr<VertexArrayObject> vao)
{
	this->vao = vao;
}


MeshComponent::~MeshComponent()
{
}


TextureComponent::TextureComponent(shared_ptr<Texture> tex)
{
	this->texture = tex;
}

TextureComponent::~TextureComponent()
{
}


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

CameraComponent::CameraComponent(shared_ptr<Camera> c)
{
	camera = c;
}

CameraComponent::~CameraComponent()
{
}

FramebufferComponent::FramebufferComponent(shared_ptr<Framebuffer> fb)
{
	framebuffer = fb;
}

FramebufferComponent::~FramebufferComponent()
{
}


OrbitalComponent::OrbitalComponent() { }

OrbitalComponent::~OrbitalComponent() { }


vec3 OrbitalComponent::xyz_from_lbg()
{
    quat ql, qb, qg;
    // rotate gamma about z axis
    qg.w = cos((gamma + rotation_phase) / 2);
    qg.x = 0;
    qg.y = 0;
    qg.z = sin((gamma + rotation_phase) / 2);

    // rotate beta about y axis
    qb.w = cos((M_PI_2 - beta) / 2);
    qb.x = 0;
    qb.y = sin((M_PI_2 - beta) / 2);
    qb.z = 0;

    // rotate alpha about z axis
    ql.w = cos(lambda / 2);
    ql.x = 0;
    ql.y = 0;
    ql.z = sin(lambda / 2);

    return glm::eulerAngles(ql * qb * qg);
}


void OrbitalComponent::calculate_rot_phase(double julian_day)
{
    rotation_phase = (julian_day - jd_0) / (rot_period / 24.0) * 2 * M_PI;

    while (rotation_phase < 0)
        rotation_phase += 2 * M_PI;
    while (rotation_phase >= 2 * M_PI)
        rotation_phase -= 2 * M_PI;
}
