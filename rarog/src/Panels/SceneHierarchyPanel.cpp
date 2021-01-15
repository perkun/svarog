#include "svpch.h"
#include "SceneHierarchyPanel.h"
#include "Components.h"
#include "CameraController.h"
#include "ModelController.h"
#include "AsteroidController.h"
#include <glm/gtc/type_ptr.hpp>

SceneHierarchyPanel::SceneHierarchyPanel(Scene *s)
{
	scene = s;
}


SceneHierarchyPanel::~SceneHierarchyPanel()
{
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
		scene->selected_entity = Entity();
	}


	ImGui::End();


	ImGui::Begin("Properties");
	if (scene->selected_entity && scene->selected_entity.get_parent())
	{
		 draw_selected_properties(scene->selected_entity);
	}
	ImGui::End();



	// deleting
	if (entity_to_delete)
	{
		if (scene->selected_entity == entity_to_delete)
			scene->selected_entity = Entity();
		entity_to_delete.destroy();
		entity_to_delete = Entity();
	}

}


void SceneHierarchyPanel::draw_entity_node(Entity &entity)
{
    string &tag = entity.get_component<TagComponent>().tag;

    ImGuiTreeNodeFlags flags =
        ((scene->selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
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
        scene->selected_entity = entity;
    }

    // left click popup
    if (ImGui::BeginPopupContextItem())
    {
		ImGui::Checkbox("Render", &entity.get_component<SceneStatus>().render);
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
			e.add_component<Material>(Application::shaders["line_shader"]);
			Batch axes;
			axes.push_back(IndexedLine(vec3(0.), vec3(2., 0., 0), vec4(1., 0., 0., 1.)));
			axes.push_back(IndexedLine(vec3(0.), vec3(0., 2., 0), vec4(0., 1., 0., 1.)));
			axes.push_back(IndexedLine(vec3(0.), vec3(0., 0., 2), vec4(0., 0., 1., 1.)));
			e.add_component<MeshComponent>(make_shared<VertexArrayObject>(axes.indexed_model));
            entity.add_child(e);
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
                    entity.add_component<MeshComponent>(
                        make_shared<VertexArrayObject>(IndexedCube()));
                }
                if (ImGui::MenuItem("Ico Sphere"))
                {
                    entity.add_component<MeshComponent>(
                        make_shared<VertexArrayObject>(IndexedIcoSphere()));
                }
                if (ImGui::MenuItem("Quad"))
                {
                    entity.add_component<MeshComponent>(
                        make_shared<VertexArrayObject>(IndexedQuad()));
                }
                if (ImGui::MenuItem("Obj file"))
                {
                    string filename = FileDialog::open_file("*.OBJ *.obj");
                    if (!filename.empty())
                        entity.add_component<MeshComponent>(
                            make_shared<VertexArrayObject>(IndexedModelObj(
                                filename, NormalIndexing::PER_VERTEX)));
                }
                if (ImGui::MenuItem("Shp file"))
                {
                    string filename = FileDialog::open_file("*.SHP *.shp");
                    if (!filename.empty())
                        entity.add_component<MeshComponent>(
                            make_shared<VertexArrayObject>(IndexedModelShp(
                                filename, NormalIndexing::PER_VERTEX)));
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
                        entity.add_component<Material>(element.second);
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
                if (ImGui::MenuItem("Asteroid Controller"))
                {
                    entity.add_component<NativeScriptComponent>()
                        .bind<AsteroidController>();
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
                        make_shared<PerspectiveCamera>(
                            radians(25.0), 1.0, 0.01, 50.0));
                }
                if (ImGui::MenuItem("Orthografic Camera"))
                {
                    entity.add_component<CameraComponent>(
                        make_shared<OrthograficCamera>(3., 1.0, 0.1, 20.));
                }
                ImGui::EndMenu();
            }

            sprintf(buff, "%s  Add Orbital Component", "\xef\x94\x97");
            if (!entity.has_component<OrbitalComponent>() &&
                ImGui::MenuItem(buff))
            {
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
				e.get_component<Material>().uniforms_int["u_has_texture"] = 0;
				});
		list_component<Material>(entity, "Material Component", "\xef\x83\xab",
				[](Entity &e) {});
		list_component<CameraComponent>(entity, "Camera Component",
				"\xef\x80\xbd", [](Entity &e) {});
		list_component<NativeScriptComponent>(entity, "Native Script Component",
				"\xef\x92\x89", [](Entity &e) {});
		list_component<OrbitalComponent>(entity, "OrbitalComponent",
				"\xef\x94\x97", [](Entity &e) {});


		// list children
		for (Entity &child :
				entity.get_component<SceneGraphComponent>().children)
			draw_entity_node(child);

		ImGui::TreePop();
	}
}

void SceneHierarchyPanel::draw_selected_properties(Entity &entity)
{
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
		// TODO: convertions rad -> deg, and positions * multiplicant
		Transform &t = entity.get_component<Transform>();

		glm::vec3 rot_deg;
		rot_deg = (float)(180./M_PI) * t.rotation ;

		ImGui::DragFloat3("Position", glm::value_ptr(t.position), 0.1);
		ImGui::DragFloat3("Rotation", glm::value_ptr(rot_deg), 0.1);
		ImGui::DragFloat3("Scale", glm::value_ptr(t.scale), 0.1);

		t.rotation = rot_deg * (float)(M_PI / 180.0);


		if (entity.has_component<OrbitalComponent>())
		{
			static double julian_day = Time::julian_day_now();
			static Time::CalendarDate date = Time::jd_to_date(Time::julian_day_now());

			for (int i = 0; i < 5; i++)
				ImGui::Spacing();

			OrbitalComponent &oc = entity.get_component<OrbitalComponent>();

			ImGui::DragFloat("rot. speed", &oc.rotation_speed, 0.1f);

			for (int i = 0; i < 5; i++)
				ImGui::Spacing();

			float rot_phase_deg = oc.rotation_phase * 180./M_PI;
			if (ImGui::DragFloat("rot phase", &rot_phase_deg))
			{
				oc.rotation_phase = rot_phase_deg * M_PI/180.;
				t.rotation = oc.xyz_from_lbg();
			}

			if (ImGui::InputDouble("P [h]", &oc.rot_period))
			{
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			for (int i = 0; i < 5; i++)
				ImGui::Spacing();

			vec3 euler_angles_deg;
			euler_angles_deg.x = oc.lambda * 180./M_PI;
			euler_angles_deg.y = oc.beta * 180./M_PI;
			euler_angles_deg.z = oc.gamma * 180./M_PI;

			if (ImGui::DragFloat3("Euler lbg", glm::value_ptr(euler_angles_deg)))
			{
				oc.lambda = euler_angles_deg.x * M_PI/180.;
				oc.beta = euler_angles_deg.y * M_PI/180.;
				oc.gamma = euler_angles_deg.z * M_PI/180.;

				t.rotation = oc.xyz_from_lbg();
			}

			for (int i = 0; i < 5; i++)
				ImGui::Spacing();

			if (ImGui::Button("Reset"))
			{
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}


			// set JD
			ImGui::SetNextItemWidth(150);
			if (ImGui::InputDouble("JD", &julian_day))
			{
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			ImGui::SetNextItemWidth(150);
			if (ImGui::InputInt("Year", &date.year))
			{
				date.normalize();
				julian_day = Time::julian_day(date);
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			ImGui::SetNextItemWidth(150);
			if (ImGui::InputInt("Month", &date.month))
			{
				date.normalize();
				julian_day = Time::julian_day(date);
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			ImGui::SetNextItemWidth(150);
			if (ImGui::InputInt("Day", &date.day))
			{
				date.normalize();
				julian_day = Time::julian_day(date);
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			ImGui::SetNextItemWidth(150);
			if (ImGui::InputInt("Hour", &date.hour))
			{
				date.normalize();
				julian_day = Time::julian_day(date);
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			ImGui::SetNextItemWidth(150);
			if (ImGui::InputInt("Min.", &date.minute))
			{
				date.normalize();
				julian_day = Time::julian_day(date);
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

			ImGui::SetNextItemWidth(150);
			if (ImGui::InputFloat("Sec.", &date.second))
			{
				date.normalize();
				julian_day = Time::julian_day(date);
				oc.calculate_rot_phase(julian_day);
				t.rotation = oc.xyz_from_lbg();
			}

// 			if (ImGui::Button("Reset"))
// 			{
// 				oc.calculate_rot_phase(julian_day);
// 				t.rotation = oc.xyz_from_lbg();
// 			}
		}
	}

	ImGui::Separator();


	if (entity.has_component<Material>())
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
			ImGui::InputInt(uniform->first.c_str(),
				&uniform->second);
		}
		for (auto uniform = m.uniforms_float.begin();
			 uniform != m.uniforms_float.end(); uniform++)
		{
			ImGui::InputFloat(uniform->first.c_str(),
				&uniform->second);
		}

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
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
// 		if (ImGui::Button("Add mat4") && strcmp(u_name, "") != 0)
// 			m.uniforms_mat4[u_name] = mat4(0);
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
