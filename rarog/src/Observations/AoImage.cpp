#include "svpch.h"
#include "AoImage.h"


AoImage::AoImage(Entity &target, Entity &observer, int width, int height)
	: Observation(target, observer)
{
	TextureSpec specs;

	specs.target = GL_TEXTURE_2D;
	specs.level = 0;
	specs.internal_format = GL_RGB32F;
	specs.width = width;
	specs.height = height;
	specs.depth = 1;
	specs.border = 0;
	specs.format = GL_RGB;
	specs.type = GL_FLOAT;

	texture = make_shared<Texture>(specs);

	ghost_color = vec4(237 / 256., 84 / 256., 84 / 256., 0.2);
}


AoImage::~AoImage()
{
}


void AoImage::serialize(YAML::Emitter &out)
{
	out << YAML::BeginMap;

	out << YAML::Key << "jd" << YAML::Value << julian_day;

	out.SetSeqFormat(YAML::Flow);
	out << YAML::Key << "target_position" << YAML::BeginSeq;
	out << YAML::Value << target_transform.position.x;
	out << YAML::Value << target_transform.position.y;
	out << YAML::Value << target_transform.position.z << YAML::EndSeq;

	out << YAML::Key << "observer_position" << YAML::BeginSeq;
	out << YAML::Value << observer_transform.position.x;
	out << YAML::Value << observer_transform.position.y;
	out << YAML::Value << observer_transform.position.z << YAML::EndSeq;
	out.SetSeqFormat(YAML::Block);

	out << YAML::Key << "ao_size" << YAML::Value << texture->get_dimentions().x;

	out << YAML::Key << "type";
	out << YAML::BeginSeq <<  YAML::Value << "AO" << YAML::EndSeq;

	out << YAML::EndMap;

}
