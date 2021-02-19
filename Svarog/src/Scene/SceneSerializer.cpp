#include "svpch.h"
#include "SceneSerializer.h"
namespace YAML
{

template <> struct convert<glm::vec3>
{
    static Node encode(const glm::vec3 &rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node &node, glm::vec3 &rhs)
    {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template <> struct convert<glm::vec4>
{
    static Node encode(const glm::vec4 &rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node &node, glm::vec4 &rhs)
    {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};
} // namespace YAML

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}


SceneSerializer::SceneSerializer(Scene *sc) : scene(sc)
{
}

SceneSerializer::~SceneSerializer()
{
}

void serialize_entity(YAML::Emitter &out, Entity entity)
{
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity" << YAML::Value << entity.get_uuid();

	if (entity.has_component<SceneGraphComponent>())
	{
        out << YAML::Key << "parent_uuid" << YAML::Value
			<< entity.get_component<SceneGraphComponent>().parent.get_uuid();
	}

    if (entity.has_component<Transform>())
    {
        Transform &tr = entity.get_component<Transform>();

        out << YAML::Key << "Transform";
        out << YAML::BeginMap; // Transform

        out << YAML::Key << "scale" << YAML::Value << tr.scale;
        out << YAML::Key << "position" << YAML::Value << tr.position;
        out << YAML::Key << "rotation" << YAML::Value << tr.rotation;

        out << YAML::EndMap; // Transform
    }

    if (entity.has_component<TagComponent>())
    {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap; // Tag

        out << YAML::Key << "tag" << YAML::Value
            << entity.get_component<TagComponent>().tag;

        out << YAML::EndMap; // TagComponent
    }

    if (entity.has_component<MeshComponent>())
    {
        MeshComponent &mc = entity.get_component<MeshComponent>();
        out << YAML::Key << "MeshComponent";
        out << YAML::BeginMap; // MeshComponent

        out << YAML::Key << "ModelType" << (int)mc.model_type;
        out << YAML::Key << "Filename" << mc.filename;
		out << YAML::Key << "DrawType" << mc.vao->draw_type;

        out << YAML::EndMap; // MeshComponent
    }

    if (entity.has_component<OrbitalComponent>())
    {
        OrbitalComponent &oc = entity.get_component<OrbitalComponent>();
        out << YAML::Key << "OrbitalComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "jd_0" << oc.jd_0;
        out << YAML::Key << "rotation_phase" << oc.rotation_phase;
        out << YAML::Key << "rotation_speed" << oc.rotation_speed;
        out << YAML::Key << "rot_period" << oc.rot_period;
        out << YAML::Key << "lambda" << oc.lambda;
        out << YAML::Key << "beta" << oc.beta;
        out << YAML::Key << "gamma" << oc.gamma;
        // TODO
        // orbital elements

        out << YAML::EndMap;
    }

    if (entity.has_component<LightComponent>())
    {
        out << YAML::Key << "LightComponent";
        out << YAML::BeginMap;
        out << YAML::Key << "color"
            << entity.get_component<LightComponent>().color;
        out << YAML::EndMap;
    }

    if (entity.has_component<FramebufferComponent>())
    {
        FramebufferComponent &fc = entity.get_component<FramebufferComponent>();
        out << YAML::Key << "FramebufferComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "flags" << fc.framebuffer->specification.flags;
        out << YAML::Key << "width" << fc.framebuffer->specification.width;
        out << YAML::Key << "height" << fc.framebuffer->specification.height;

        out << YAML::EndMap;
    }

    if (entity.has_component<CameraComponent>())
    {
        CameraComponent &cc = entity.get_component<CameraComponent>();
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;

        out << YAML::Key << "aspect" << cc.camera->aspect;
        out << YAML::Key << "position" << cc.camera->position;
        out << YAML::Key << "front" << cc.camera->front;
        out << YAML::Key << "up" << cc.camera->up;
        out << YAML::Key << "right" << cc.camera->right;
        out << YAML::Key << "speed" << cc.camera->speed;
        out << YAML::Key << "rotation_speed" << cc.camera->rotation_speed;
        out << YAML::Key << "z_near" << cc.camera->view_box_z_near;
        out << YAML::Key << "z_far" << cc.camera->view_box_z_far;

        out << YAML::EndMap;
    }

    if (entity.has_component<SceneStatus>())
    {
        SceneStatus &ss = entity.get_component<SceneStatus>();
        out << YAML::Key << "SceneStatus";
        out << YAML::BeginMap;

        out << YAML::Key << "active" << ss.active;
        out << YAML::Key << "render" << ss.render;
        out << YAML::Key << "casting_shadow" << ss.casting_shadow;

        out << YAML::EndMap;
    }

	if (entity.has_component<Material>())
	{
		Material &mc = entity.get_component<Material>();

		out << YAML::Key << "Material";
		out << YAML::BeginMap; // Material
		out << YAML::Key << "shader_name" << mc.shader_name;

		// uniforms
		out << YAML::Key << "Uniforms";
		out << YAML::BeginMap; // Uniforms


		out << YAML::Key << "Vec3";
		out << YAML::BeginMap;
		for (auto u: mc.uniforms_vec3)
			out << YAML::Key << u.first << YAML::Value << u.second;
		out << YAML::EndMap;

		out << YAML::Key << "Vec4";
		out << YAML::BeginMap;
		for (auto u: mc.uniforms_vec4)
			out << YAML::Key << u.first << YAML::Value << u.second;
		out << YAML::EndMap;

		out << YAML::Key << "Float";
		out << YAML::BeginMap;
		for (auto u: mc.uniforms_float)
			out << YAML::Key << u.first << YAML::Value << u.second;
		out << YAML::EndMap;

		out << YAML::Key << "Int";
		out << YAML::BeginMap;
		for (auto u: mc.uniforms_int)
			out << YAML::Key << u.first << YAML::Value << u.second;
		out << YAML::EndMap;

		out << YAML::EndMap;  // Uniforms
		out << YAML::EndMap;  // Material
	}

    out << YAML::EndMap; // Entity
}

Entity SceneSerializer::deserialize_entity(
    YAML::detail::iterator_value serialized_ent)
{
    string name;
    auto tagComponent = serialized_ent["TagComponent"];
    if (tagComponent)
        name = tagComponent["tag"].as<string>();

    TRACE("Deserializing Entity {}", name);

    Entity entity = scene->create_entity(name);


    if (serialized_ent["Transform"])
    {
        Transform &tr = entity.get_component<Transform>(); // should already be
        auto node = serialized_ent["Transform"];

        tr.scale = node["scale"].as<vec3>();
        tr.position = node["position"].as<vec3>();
        tr.rotation = node["rotation"].as<vec3>();
    }

    if (serialized_ent["MeshComponent"])
    {
        auto node = serialized_ent["MeshComponent"];
        if (node["ModelType"].as<int>() == (int)ModelType::FILE)
            entity.add_component<MeshComponent>(node["Filename"].as<string>());
        else
            entity.add_component<MeshComponent>((ModelType)node["ModelType"].as<int>());

        entity.get_component<MeshComponent>().vao->draw_type =
            node["DrawType"].as<unsigned int>();

        scene->target = entity;
    }

    if (serialized_ent["Material"])
    {
        auto node = serialized_ent["Material"];
        if (Application::shaders.find(node["shader_name"].as<string>()) !=
            Application::shaders.end())
        {
            Material &mc = entity.add_component<Material>(
                node["shader_name"].as<string>());
            // uniforms...
            auto uniforms = node["Uniforms"];
            for (auto univec3 : uniforms["Vec3"])
                mc.uniforms_vec3[univec3.first.as<string>()] =
                    univec3.second.as<vec3>();
            for (auto univec4 : uniforms["Vec4"])
                mc.uniforms_vec4[univec4.first.as<string>()] =
                    univec4.second.as<vec4>();
            for (auto uniint : uniforms["Int"])
                mc.uniforms_int[uniint.first.as<string>()] =
                    uniint.second.as<int>();
            for (auto unifloat : uniforms["Float"])
                mc.uniforms_float[unifloat.first.as<string>()] =
                    unifloat.second.as<float>();
        }
    }

    if (serialized_ent["OrbitalComponent"])
    {
        OrbitalComponent &oc = entity.add_component<OrbitalComponent>();
        auto node = serialized_ent["OrbitalComponent"];

        oc.jd_0 = node["jd_0"].as<double>();
        oc.rotation_phase = node["rotation_phase"].as<double>();
        oc.rotation_speed = node["rotation_speed"].as<double>();
        oc.rot_period = node["rot_period"].as<double>();
        oc.lambda = node["lambda"].as<double>();
        oc.beta = node["beta"].as<double>();
        oc.gamma = node["gamma"].as<double>();
    }

    if (serialized_ent["LightComponent"])
    {
        auto node = serialized_ent["LightComponent"];
        LightComponent &lc = entity.add_component<LightComponent>();
        lc.color = node["color"].as<vec4>();

        scene->light = entity;
    }

    if (serialized_ent["FramebufferComponent"])
    {
        auto node = serialized_ent["FramebufferComponent"];
        FramebufferComponent &fb =
            entity.add_component<FramebufferComponent>(make_shared<Framebuffer>(
                node["width"].as<int>(), node["height"].as<int>(),
                node["flags"].as<char>()));
    }

    if (serialized_ent["CameraComponent"])
    {
        auto node = serialized_ent["CameraComponent"];
        CameraComponent &cc = entity.add_component<CameraComponent>(
            make_shared<OrthograficCamera>(0.2, node["aspect"].as<float>(),
                                           node["z_near"].as<float>(),
                                           node["z_far"].as<float>()));

        cc.camera->position = node["position"].as<vec3>();
        cc.camera->front = node["front"].as<vec3>();
        cc.camera->up = node["up"].as<vec3>();
        cc.camera->right = node["right"].as<vec3>();
        cc.camera->speed = node["speed"].as<float>();
        cc.camera->rotation_speed = node["rotation_speed"].as<float>();

        // TODO distingush between perspective and orthografic cameras!!!
        scene->observer = entity;
    }

    if (serialized_ent["SceneStatus"])
    {
        auto node = serialized_ent["SceneStatus"];
        SceneStatus &ss = entity.get_component<SceneStatus>();
        ss.active = node["active"].as<bool>();
        ss.render = node["render"].as<bool>();
        ss.casting_shadow = node["casting_shadow"].as<bool>();
    }

    return entity;
}

void SceneSerializer::serialize(const string filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    scene->registry.each([&](auto entityID)
	{
        Entity entity(entityID, &scene->registry);
        if (!entity)
            return;

		cout << entity.get_component<TagComponent>().tag << endl;

        serialize_entity(out, entity);
    });

    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}

bool SceneSerializer::deserialize(const string filepath)
{
    YAML::Node data = YAML::LoadFile(filepath);
    if (!data["Scene"])
        return false;

    auto entities = data["Entities"];
	if (!entities)
	{
		WARN("Empty scene");
		return false;
	}

	map<uint32_t, Entity> deserialized;
	bool found_root = false;

	for (auto entity : entities)
		if (entity["TagComponent"]["tag"].as<string>() == "root")
		{
			found_root = true;
			scene->root_entity.destroy();

			uint32_t uuid = entity["Entity"].as<uint32_t>();
			Entity ent = deserialize_entity(entity);
			deserialized[uuid] = ent;
			scene->root_entity = ent;
		}

	if (!found_root)
	{
		WARN("No root entity in scene!");
		return false;
	}

    string scene_name = data["Scene"].as<string>();

    cout << "Scene name: " << scene_name << endl;


	for (auto entity : entities)
	{
		if (entity["TagComponent"]["tag"].as<string>() != "root")
		{
			uint32_t uuid = entity["Entity"].as<uint32_t>();
			deserialized[uuid] = deserialize_entity(entity);
		}
	}

	// scene hierarchy
	for (auto entity : entities)
	{
		unsigned int uuid = entity["Entity"].as<unsigned int>();
		unsigned int parent_uuid = entity["parent_uuid"].as<unsigned int>();

		if (deserialized.find(uuid) == deserialized.end())
			continue;
		if (deserialized.find(parent_uuid) == deserialized.end())
			continue;

		Entity ent = deserialized[uuid];
		Entity parent_ent = deserialized[parent_uuid];

		if (ent && parent_ent)
		{
			parent_ent.add_child(ent);
		}
	}

    return true;
}
