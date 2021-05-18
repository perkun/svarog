#include "svpch.h"
#include "Components.h"


MeshComponent::MeshComponent(shared_ptr<VertexArrayObject> v) : vao(v)
{
}


MeshComponent::MeshComponent(ModelType t) : model_type(t)
{
	if (model_type == ModelType::CUBE)
		vao = make_shared<VertexArrayObject>(IndexedCube(vec3(-0.5), vec3(1.)));
	else if (model_type == ModelType::ICO_SPHERE)
		vao = make_shared<VertexArrayObject>(IndexedIcoSphere(vec3(0.), vec3(1.)));
	else if (model_type == ModelType::QUAD)
		vao = make_shared<VertexArrayObject>(IndexedQuad(vec3(0.), vec3(1.)));
	r_max = 1;
}

MeshComponent::MeshComponent(string filename)
{
    if (File::is_extension(filename, "obj"))
    {
        vao = make_shared<VertexArrayObject>(
            IndexedModelObj(filename, NormalIndexing::PER_FACE));
		model_type = ModelType::FILE;
		this->filename = filename;

		header = ObjHeader(filename);
    }
    else if (File::is_extension(filename, "shp"))
	{
        vao = make_shared<VertexArrayObject>(
            IndexedModelShp(filename, NormalIndexing::PER_FACE));
		model_type = ModelType::FILE;
		this->filename = filename;
	}
    else
        vao = make_shared<VertexArrayObject>(IndexedCube(vec3(-0.5), vec3(1.)));

	r_max = vao->r_max;
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

OrbitalComponent::OrbitalComponent(ObjHeader header)
{
	if (!header.loaded)
		return;

	lambda = header.get_item<float>("lambda") * M_PI / 180.;
	beta = header.get_item<float>("beta") * M_PI / 180.;
	gamma = header.get_item<float>("gamma") * M_PI / 180.;
	jd_0 = header.get_item<double>("jd_gamma0");
	rot_period = header.get_item<double>("period[h]") ;

	set_rot_phase_at_jd(Time::julian_day_now());
}


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


double OrbitalComponent::calculate_rot_phase(double julian_day) const
{
    if (rot_period == 0.)
    {
        return 0;
    }

	double rp = (julian_day - jd_0) / (rot_period / 24.0) * 2 * M_PI;

    while (rp < 0)
        rp += 2 * M_PI;
    while (rp >= 2 * M_PI)
        rp -= 2 * M_PI;

    return rp;
}


void OrbitalComponent::set_rot_phase_at_jd(double julian_day)
{
	rotation_phase = calculate_rot_phase(julian_day);
}


