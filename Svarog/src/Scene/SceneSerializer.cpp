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



SceneSerializer::SceneSerializer(Scene *sc) : scene(sc) { }

SceneSerializer::~SceneSerializer() { }

void serialize_entity(YAML::Emitter& out, Entity entity)
{
    out << YAML::BeginMap; // Entity
    out << YAML::Key << "Entity" << YAML::Value << "12837192831273";

	if (entity.has_component<Transform>())
	{
		Transform &tr = entity.get_component<Transform>();

		out << YAML::Key << "Transform";
		out << YAML::BeginMap; // Transform

		out << YAML::Key << "position" << YAML::Value << tr.position;

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

        out << YAML::EndMap; // Entity
}

void SceneSerializer::serialize(const string filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    scene->registry.each([&](auto entityID) {
        Entity entity(entityID, &scene->registry);
        if (!entity)
            return;

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

    string scene_name = data["Scene"].as<string>();

	cout << "Scene name: " << scene_name << endl;

    auto entities = data["Entities"];
    if (entities)
    {
        for (auto entity : entities)
        {
//             uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO
//
            string name;
            auto tagComponent = entity["TagComponent"];
            if (tagComponent)
                name = tagComponent["tag"].as<string>();
//
			cout << "Ent name: " <<  name << endl;
        }
    }

    return true;
}
