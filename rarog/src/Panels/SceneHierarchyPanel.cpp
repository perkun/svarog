#include "svpch.h"
#include "SceneHierarchyPanel.h"
#include "Components.h"
#include "CameraController.h"
#include "ModelController.h"
#include "AsteroidController.h"
#include <glm/gtc/type_ptr.hpp>

SceneHierarchyPanel::SceneHierarchyPanel(Scene *s, double *julian_day)
{
    scene = s;
    this->julian_day = julian_day;
}


SceneHierarchyPanel::~SceneHierarchyPanel()
{
}


void SceneHierarchyPanel::add_asteroid_from_file(Entity &model, string filename)
{
    model.add_component<Material>("tex_sha_ls").uniforms_int["u_has_texture"] = 0;
    Transform &mt = model.get_component<Transform>();

    if (File::is_extension(filename, "obj"))
    {
		if (model.has_component<MeshComponent>())
			model.remove_component<MeshComponent>();
		if (model.has_component<OrbitalComponent>())
			model.remove_component<OrbitalComponent>();
		if (model.has_component<NativeScriptComponent>())
			model.remove_component<NativeScriptComponent>();

        MeshComponent &mc = model.add_component<MeshComponent>(filename);
        OrbitalComponent &oc = model.add_component<OrbitalComponent>(mc.header);
		model.add_component<NativeScriptComponent>().bind<AsteroidController>();
        mt.rotation = oc.xyz_from_lbg();
    }
    else if (File::is_extension(filename, "shp"))
    {
        model.add_component<MeshComponent>(filename);
    }
    else
    {
        WARN("Wrong file extension, exiting");
        cout << "File extension not recognised" << endl;
    }
}

void SceneHierarchyPanel::on_imgui_render()
{
    ImGui::Begin("Scene Hierarchy");

    // 	scene->registry.each([&](auto entityID)
    // 	{
    // 		Entity entity(entityID, &scene->registry);
    // // 		if (entity.parent == NULL)  // root
    // 			draw_entity_node(entity);
    //
    // 	});
    draw_entity_node(scene->root_entity);

    // deselect when clicked elsewhere
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
        selected_entity = Entity();
    }


    ImGui::End();


    ImGui::Begin("Properties");
    if (selected_entity && selected_entity.get_parent())
    {
        draw_selected_properties(selected_entity);
    }
    ImGui::End();


    // deleting
    if (entity_to_delete)
    {
        if (selected_entity == entity_to_delete)
            selected_entity = Entity();

        if (scene->target == entity_to_delete)
            scene->target = Entity();

        if (scene->observer == entity_to_delete)
            scene->observer = Entity();

        if (scene->light == entity_to_delete)
            scene->light = Entity();

        entity_to_delete.destroy();
        entity_to_delete = Entity();
    }
}

void SceneHierarchyPanel::set_selected_entity(Entity entity)
{
	selected_entity = entity;
}


void SceneHierarchyPanel::draw_entity_node(Entity &entity)
{
    string &tag = entity.get_component<TagComponent>().tag;

    ImGuiTreeNodeFlags flags =
        ((selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ((entity == scene->root_entity) ? ImGuiTreeNodeFlags_DefaultOpen : 0);

    bool opened =
        ImGui::TreeNodeEx((void *)(entity.get_handle()), flags, tag.c_str());

    // Drag and Drop
    if (entity != scene->root_entity && ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_TREENODE", (void *)(&entity), 256);
        ImGui::Text("Move Entity");

        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload =
                ImGui::AcceptDragDropPayload("_TREENODE"))
        {
            Entity payload_entity = *(const Entity *)payload->Data;
            payload_entity.detatch();
            entity.add_child(payload_entity);
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemClicked())
    {
        selected_entity = entity;
    }

    // left click popup
    if (ImGui::BeginPopupContextItem())
    {
        ImGui::Checkbox("Render", &entity.get_component<SceneStatus>().render);
        ImGui::Checkbox("Cast shadows",
                        &entity.get_component<SceneStatus>().casting_shadow);
        ImGui::Separator();

        sprintf(buff, "%s  Add Empty Entity", "\xef\x81\xa7");
        if (ImGui::MenuItem(buff))
        {
            Entity e = scene->create_entity("Unnamed Entity");
            entity.add_child(e);
        }

        sprintf(buff, "%s  Add xyz axes", "\xef\x81\xa7");
        if (ImGui::MenuItem(buff))
        {
            Entity e = scene->create_entity("xyz axes");
            e.add_component<Material>("line_shader");
            Batch axes;
            axes.push_back(
                IndexedLine(vec3(0.), vec3(2., 0., 0), vec4(1., 0., 0., 1.)));
            axes.push_back(
                IndexedLine(vec3(0.), vec3(0., 2., 0), vec4(0., 1., 0., 1.)));
            axes.push_back(
                IndexedLine(vec3(0.), vec3(0., 0., 2), vec4(0., 0., 1., 1.)));
            e.add_component<MeshComponent>(
                make_shared<VertexArrayObject>(axes.indexed_model));
            e.get_component<SceneStatus>().casting_shadow = false;
            entity.add_child(e);
        }

		sprintf(buff, "%s  Load Asteroid from file", "\xef\x81\xa7");
		if (ImGui::MenuItem(buff))
		{
			string filename = FileDialog::open_file("\"*.shp\" \"*.obj\"");
			if (filename != "")
			{
				Entity e = scene->create_entity("Unnamed Entity");
				add_asteroid_from_file(e, filename);
				entity.add_child(e);
			}
		}

        ImGui::Separator();

        if (entity != scene->root_entity)
        {
            sprintf(buff, "%s  Add Mesh Component", "\xef\x9a\xa6");
            if (!entity.has_component<MeshComponent>() &&
                ImGui::BeginMenu(buff))
            {
                if (ImGui::MenuItem("Cube"))
                {
                    entity.add_component<MeshComponent>(ModelType::CUBE);
                }
                if (ImGui::MenuItem("Ico Sphere"))
                {
                    entity.add_component<MeshComponent>(ModelType::ICO_SPHERE);
                }
                if (ImGui::MenuItem("Quad"))
                {
                    entity.add_component<MeshComponent>(ModelType::QUAD);
                }
                if (ImGui::MenuItem("Obj file"))
                {
                    string filename = FileDialog::open_file("*.OBJ *.obj");
                    if (!filename.empty())
                        entity.add_component<MeshComponent>(filename);
                }
                if (ImGui::MenuItem("Shp file"))
                {
                    string filename = FileDialog::open_file("*.SHP *.shp");
                    if (!filename.empty())
                        entity.add_component<MeshComponent>(filename);
                }

                ImGui::EndMenu();
            }

            sprintf(buff, "%s  Add Material Component", "\xef\x83\xab");
            if (!entity.has_component<Material>() && ImGui::BeginMenu(buff))
            {
                for (pair<string, shared_ptr<Shader>> element :
                     Application::shaders)
                {
                    if (ImGui::MenuItem(element.first.c_str()))
                    {
                        entity.add_component<Material>(element.first);
                    }
                }
                ImGui::EndMenu();
            }

            sprintf(buff, "%s  Add Texture Component", "\xef\x80\xbe");
            if (!entity.has_component<TextureComponent>() &&
                ImGui::BeginMenu(buff))
            {
                if (ImGui::MenuItem("From File"))
                {
                    load_texture_from_file(entity);
                }
                ImGui::EndMenu();
            }

            sprintf(buff, "%s  Add Native Script Component", "\xef\x92\x89");
            if (!entity.has_component<NativeScriptComponent>() &&
                ImGui::BeginMenu(buff))
            {
                if (ImGui::MenuItem("Camera Controller"))
                {
                    entity.add_component<NativeScriptComponent>()
                        .bind<CameraController>();
                }
                if (ImGui::MenuItem("Model Controller"))
                {
                    entity.add_component<NativeScriptComponent>()
                        .bind<ModelController>();
                }

                bool enabled = entity.has_component<OrbitalComponent>();
                if (ImGui::MenuItem("Asteroid Controller", NULL, false,
                                    enabled))
                {

                    entity.add_component<NativeScriptComponent>()
                        .bind<AsteroidController>();
                }
                if (!enabled && ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(
                        "You need to add Orbital Component for "
                        "this to work");
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }

                ImGui::EndMenu();
            }
            sprintf(buff, "%s  Add Camera Component", "\xef\x80\xbd");
            if (!entity.has_component<CameraComponent>() &&
                ImGui::BeginMenu(buff))
            {
                if (ImGui::MenuItem("Perspective Camera"))
                {
                    entity.add_component<CameraComponent>(
                        make_shared<PerspectiveCamera>(radians(25.0), 1.0, 0.01,
                                                       50.0));
                }
                if (ImGui::MenuItem("Orthografic Camera"))
                {
                    entity.add_component<CameraComponent>(
                        make_shared<OrthograficCamera>(3., 1.0, 0.1, 20.));
                }
                ImGui::EndMenu();
            }

            sprintf(buff, "%s  Add Light Component", "\xef\xaf\xa6");
            if (!entity.has_component<LightComponent>() &&
                ImGui::MenuItem(buff))
            {
                entity.add_component<LightComponent>();


				FramebufferSpecification light_fb_specs;
				light_fb_specs.attachments = {
					FramebufferTextureFormat::RGBA32F,
					FramebufferTextureFormat::DEPTH32FSTENCIL8};
				light_fb_specs.width = 2048;
				light_fb_specs.height = 2048;

                entity.add_component<FramebufferComponent>(
                    make_shared<Framebuffer>(light_fb_specs));
            }

            sprintf(buff, "%s  Add Orbital Component", "\xef\x94\x97");
            if (!entity.has_component<OrbitalComponent>() &&
                ImGui::MenuItem(buff))
            {
                if (entity.has_component<MeshComponent>())
                {
                    entity.add_component<OrbitalComponent>(
                        entity.get_component<MeshComponent>().header);
                }
                else
                    entity.add_component<OrbitalComponent>();
            }

            ImGui::Spacing();
            ImGui::Separator();

            sprintf(buff, "%s Delete Entity", "\xef\x80\x8d");
            if (ImGui::MenuItem(buff))
            {
                entity_to_delete = entity;
            }
        }

        ImGui::EndPopup();
    }

    if (opened)
    {
        // list components
        list_component<MeshComponent>(entity, "Mesh Component", "\xef\x9a\xa6",
                                      [](Entity &e) {});
        list_component<TextureComponent>(
            entity, "Texture Component", "\xef\x80\xbe", [](Entity &e) {
                if (e.has_component<Material>())
                    e.get_component<Material>().uniforms_int["u_has_texture"] =
                        0;
            });
        list_component<Material>(entity, "Material Component", "\xef\x83\xab",
                                 [](Entity &e) {});
        list_component<CameraComponent>(entity, "Camera Component",
                                        "\xef\x80\xbd", [](Entity &e) {});
        list_component<NativeScriptComponent>(entity, "Native Script Component",
                                              "\xef\x92\x89", [](Entity &e) {});
        list_component<OrbitalComponent>(entity, "OrbitalComponent",
                                         "\xef\x94\x97", [](Entity &e) {});
        list_component<LightComponent>(entity, "LightComponent", "\xef\xaf\xa6",
                                       [](Entity &e) {});


        // list children
        for (Entity &child :
             entity.get_component<SceneGraphComponent>().children)
            draw_entity_node(child);

        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::draw_selected_properties(Entity &entity)
{
    ImGuiTreeNodeFlags flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;


    if (entity.has_component<TagComponent>())
    {
        // renameing entity
        string &tag = entity.get_component<TagComponent>().tag;
        memset(buff, 0, sizeof(buff));
        strcpy(buff, tag.c_str());
        if (ImGui::InputText("Tag", buff, sizeof(buff)))
            tag = string(buff);
    }

    if (entity.has_component<Transform>())
    {
        bool opened = ImGui::TreeNodeEx("Transform", flags);
        if (opened)
        {
            // TODO: convertions rad -> deg, and positions * multiplicant
            Transform &t = entity.get_component<Transform>();

            glm::vec3 rot_deg;
            rot_deg = (float)(180. / M_PI) * t.rotation;

            ImGui::DragFloat3("Position", glm::value_ptr(t.position), 0.1);
            ImGui::DragFloat3("Rotation", glm::value_ptr(rot_deg), 0.1);
            ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), 0.1);

            t.rotation = rot_deg * (float)(M_PI / 180.0);
            ImGui::TreePop();
        }
    }


    if (entity.has_component<OrbitalComponent>())
    {
        bool opened = ImGui::TreeNodeEx("OrbitalComponent", flags);
        if (opened)
        {
            Transform &t = entity.get_component<Transform>();

            for (int i = 0; i < 5; i++)
                ImGui::Spacing();

            OrbitalComponent &oc = entity.get_component<OrbitalComponent>();

            ImGui::DragFloat("rot. speed", &oc.rotation_speed, 0.1f);

            for (int i = 0; i < 5; i++)
                ImGui::Spacing();

            float rot_phase_deg = oc.rotation_phase * 180. / M_PI;
            if (ImGui::DragFloat("rot phase", &rot_phase_deg))
            {
                oc.rotation_phase = rot_phase_deg * M_PI / 180.;
                t.rotation = oc.xyz_from_lbg();
            }

            if (ImGui::InputDouble("P [h]", &oc.rot_period))
            {
                // 	TODO  zrobić coś z tym!
                oc.set_rot_phase_at_jd(*julian_day);
                t.rotation = oc.xyz_from_lbg();
            }

            for (int i = 0; i < 5; i++)
                ImGui::Spacing();

            vec3 euler_angles_deg;
            euler_angles_deg.x = oc.lambda * 180. / M_PI;
            euler_angles_deg.y = oc.beta * 180. / M_PI;
            euler_angles_deg.z = oc.gamma * 180. / M_PI;

            if (ImGui::DragFloat3("Euler lbg",
                                  glm::value_ptr(euler_angles_deg)))
            {
                oc.lambda = euler_angles_deg.x * M_PI / 180.;
                oc.beta = euler_angles_deg.y * M_PI / 180.;
                oc.gamma = euler_angles_deg.z * M_PI / 180.;

                t.rotation = oc.xyz_from_lbg();
            }

            if (ImGui::InputDouble("Rot. Epoch", &oc.jd_0))
            {
                oc.set_rot_phase_at_jd(*julian_day);
                t.rotation = oc.xyz_from_lbg();
            }
			ImGui::TreePop();
        }
    }


    if (entity.has_component<CameraComponent>())
    {
        bool opened = ImGui::TreeNodeEx("CameraComponent", flags);
        if (opened)
        {
            ImGui::Text("CameraComponent");

            CameraComponent &cc = entity.get_component<CameraComponent>();
            ImGui::PushItemWidth(150.);
            ImGui::DragFloat("speed", &cc.camera->speed);
            ImGui::PushItemWidth(150.);
            ImGui::DragFloat("rotation_speed", &cc.camera->rotation_speed);
            ImGui::PushItemWidth(150.);
            ImGui::DragFloat("z near", &cc.camera->view_box_z_near);
            ImGui::PushItemWidth(150.);
            ImGui::DragFloat("z far", &cc.camera->view_box_z_far);

            if (dynamic_pointer_cast<PerspectiveCamera>(cc.camera))
            {
                auto cam = dynamic_pointer_cast<PerspectiveCamera>(cc.camera);
                ImGui::PushItemWidth(150.);
                ImGui::DragFloat("FOV", &cam->fov);
            }
            if (dynamic_pointer_cast<OrthograficCamera>(cc.camera))
            {
                auto cam = dynamic_pointer_cast<OrthograficCamera>(cc.camera);
                ImGui::PushItemWidth(150.);
                ImGui::DragFloat("size_x", &cam->size_x);
            }


            ImGui::TreePop();
        }
    }



    if (entity.has_component<Material>())
    {
        bool opened = ImGui::TreeNodeEx("Material", flags);
        if (opened)
        {
            Material &m = entity.get_component<Material>();
            // 		for (pair<string, vec4> &uniform: m.uniforms_vec4)
            for (auto uniform = m.uniforms_vec3.begin();
                 uniform != m.uniforms_vec3.end(); uniform++)
            {
                ImGui::InputFloat3(uniform->first.c_str(),
                                   glm::value_ptr(uniform->second));
            }

            for (auto uniform = m.uniforms_vec4.begin();
                 uniform != m.uniforms_vec4.end(); uniform++)
            {
                ImGui::InputFloat4(uniform->first.c_str(),
                                   glm::value_ptr(uniform->second));
            }

            for (auto uniform = m.uniforms_int.begin();
                 uniform != m.uniforms_int.end(); uniform++)
            {
                ImGui::InputInt(uniform->first.c_str(), &uniform->second);
            }
            for (auto uniform = m.uniforms_float.begin();
                 uniform != m.uniforms_float.end(); uniform++)
            {
                ImGui::InputFloat(uniform->first.c_str(), &uniform->second);
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("Add uniform");

            sprintf(buff, "");
            if (ImGui::InputText("name", buff, sizeof(buff)))
                sprintf(u_name, buff);
            ImGui::Text("Add: ");
            ImGui::SameLine();
            if (ImGui::Button("int") && strcmp(u_name, "") != 0)
                m.uniforms_int[u_name] = 0;
            ImGui::SameLine();
            if (ImGui::Button("float") && strcmp(u_name, "") != 0)
                m.uniforms_float[u_name] = 0.;
            ImGui::SameLine();
            if (ImGui::Button("vec3") && strcmp(u_name, "") != 0)
                m.uniforms_vec3[u_name] = vec3(0);
            ImGui::SameLine();
            if (ImGui::Button("vec4") && strcmp(u_name, "") != 0)
                m.uniforms_vec4[u_name] = vec4(0);
            // 		ImGui::SameLine();
            // 		if (ImGui::Button("Add mat4") && strcmp(u_name, "") !=
            // 0) 			m.uniforms_mat4[u_name] = mat4(0);
            ImGui::TreePop();
        }
    }
}


void SceneHierarchyPanel::load_texture_from_file(Entity &entity)
{
    string filename = FileDialog::open_file("*.jpg *.png *.jpeg");
    if (filename.empty())
        return;

    INFO("Loading texture {}", filename);

    if (entity.has_component<TextureComponent>())
    {
        entity.remove_component<TextureComponent>();
    }
    entity.add_component<TextureComponent>(make_shared<ImgTexture>(filename));

    if (entity.has_component<Material>())
        entity.get_component<Material>().uniforms_int["u_has_texture"] = 1;
}
