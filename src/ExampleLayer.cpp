#include "ExampleLayer.h"


ExampleLayer::ExampleLayer() : Layer("ExampleLayer")
{

    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_1] = [](ExampleLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_alpha = true;
	};
//
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_2] = [](ExampleLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_beta = true;
	};
//
    mouse_button_pressed_map[GLFW_MOUSE_BUTTON_3] = [](ExampleLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_gamma = true;
	};


    mouse_button_released_map[GLFW_MOUSE_BUTTON_1] = [](ExampleLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_alpha = false;
	};
//
    mouse_button_released_map[GLFW_MOUSE_BUTTON_2] = [](ExampleLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_beta = false;
	};
//
    mouse_button_released_map[GLFW_MOUSE_BUTTON_3] = [](ExampleLayer *layer) {
		Transform *trans = layer->active_scene->get_active_drawable_transform();
		if (trans == NULL)
			return;
		trans->change_gamma = false;
	};


    mouse_cursor_action = [](ExampleLayer *layer, vec2 cursor_shift) {
        // free floating camera movement
        // 		Camera *camera = app->active_scene->get_active_camera();
        // 		camera->pitch(cursor_shift.y);
        // 		camera->yaw(cursor_shift.x);
//
        // rotating meshes
        Transform *trans = layer->active_scene->get_active_drawable_transform();
        if (trans == NULL)
            return;
//
        if (trans->change_alpha)
            trans->alpha += cursor_shift.x / 200.;
        if (trans->change_beta)
            trans->beta += cursor_shift.y / 300.;
        if (trans->change_gamma)
            trans->gamma += cursor_shift.x / 300.;
    };

	key_released_map[GLFW_KEY_SPACE] = [](ExampleLayer *layer) {
		SceneStatus &sc = layer->arrow.get_component<SceneStatus>();
		sc.render = !sc.render;
		cout << "space released" << endl;
	};

}


ExampleLayer::~ExampleLayer()
{
	delete active_scene;
// 	plane_mesh.destroy();
// 	tex_1.destroy();
// 	tex_2.destroy();
// 	basic_shader.destroy();
}


void ExampleLayer::on_attach()
{
	Window *window = Application::get_window();
    Camera camera(vec3(0., -3., 0.), vec3(0., 0., 0.), radians(45.0),
                  window->width / (float)window->height, 0.1, 100.0);

    camera.speed = 1.;

    Shader texture_shader("../src/shaders/texture_shader.vs",
                          "../src/shaders/texture_shader.fs");
    Shader color_shader("../src/shaders/color_shader.vs",
                        "../src/shaders/color_shader.fs");

        // Meshes
        Mesh metis_mesh("../data/model.obj", IndexModel::PER_VERTEX);
    Mesh plane_mesh("../data/plane.obj", IndexModel::PER_FACE);
    Mesh arrow_mesh("../data/arrow.obj", IndexModel::PER_FACE);
	arrow_mesh.color = vec4(157/256., 52/256., 255/256., 1.0);

    // Textures
    Texture tex_1("../data/makeeva2.png");
    Texture tex_2("../data/forest.jpg");
//     Texture tex_1("../data/cyber.png");
//     Texture tex_2("../data/horsehead.jpeg");

    // Create scene
    active_scene = new Scene();

    // Entities
    scene_camera = active_scene->create_entity();
    scene_camera.add_component<Camera>(camera);
    scene_camera.add_component<SceneStatus>(SceneStatus(true));

    plane = active_scene->create_entity();
    plane.add_component<Mesh>(plane_mesh);
    plane.add_component<Texture>(tex_1);
    plane.add_component<Shader>(texture_shader);
	plane.add_component<SceneStatus>(false, false);

    Transform &tr = plane.get_component<Transform>();
    tr.beta = 1;
    tr.position = vec3(0.0, 0., 0.);

    metis = active_scene->create_entity("Metis ent");
    metis.add_component<Mesh>(metis_mesh);
    metis.add_component<Texture>(tex_2);
    metis.add_component<Shader>(color_shader);
    metis.add_component<SceneStatus>(true);
    Transform &metis_trans = metis.get_component<Transform>();
    metis_trans.position = vec3(0.0, 0.0, 0.0);

    arrow = active_scene->create_entity("Arrow");
    arrow.add_component<Mesh>(arrow_mesh);
    arrow.add_component<Shader>(color_shader);
    arrow.add_component<SceneStatus>(false, true);
    Transform &arrow_tr = arrow.get_component<Transform>();
    arrow_tr.scale = vec3(1.f, 1.f, 2.f);
    // 	arrow.add_component<Texture>(tex_1);

    Transform &rt = active_scene->root_entity.get_component<Transform>();
    rt.position = vec3(0., 0., 0.);

    active_scene->root_entity.add_child(&plane);
    active_scene->root_entity.add_child(&metis);
    metis.add_child(&arrow);

	// FRAMEBUFFER
	FramebufferSpecification fb_spec;
	fb_spec.width = Application::get_window()->width;
	fb_spec.height = Application::get_window()->height;

	framefuffer = new Framebuffer(fb_spec);
	tex_tmp = new Texture("../data/harley_quinn2.png");
}

void ExampleLayer::on_detach()
{
	delete tex_tmp;
	delete framefuffer;
}



void ExampleLayer::on_update(double ts)
{
	if (active_scene == NULL)
	{
		cout << "No active Scene set, not rendering" << endl;
		return;
	}

	if (active_scene->get_active_camera() == NULL)
	{
		cout << "No active Camera set, not rendering" << endl;
		return;
	}

	framefuffer->bind();
	framefuffer->clear();

	active_scene->draw_root();

	framefuffer->unbind();

	active_scene->draw_root();
	active_scene->get_active_camera()->move(ts);
}


void ExampleLayer::on_imgui_render()
{
    Transform &tr = metis.get_component<Transform>();

    ImGui::SetNextWindowBgAlpha(0.0);
    ImGui::SetNextWindowPos(ImVec2(0., 0.));
    ImGui::SetNextWindowSize(ImVec2(300, 350));

    ImGui::Begin("Settings", NULL,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoTitleBar);

    ImGui::Checkbox("Spin axis", &arrow.get_component<SceneStatus>().render);

    ImGui::Text("Position");
    ImGui::SliderFloat("pos x", &(tr.position[0]), -1.0, 1.0, "X");
    ImGui::SliderFloat("pos y", &(tr.position[1]), -1.0, 1.0, "Y");
    ImGui::SliderFloat("pos z", &(tr.position[2]), -1.0, 1.0, "Z");

    ImGui::SliderFloat("Alpha", &(tr.alpha), 0., M_PI * 2, "a = %.3f");
    ImGui::SliderFloat("Beta", &(tr.beta), -M_PI, M_PI, "b = %.3f");
    ImGui::SliderFloat("Ggamma", &(tr.gamma), 0., M_PI * 2, "g = %.3f");

    ImGui::Text("Colors");
    ImGui::ColorEdit4("metis color",
                      (float *)&metis.get_component<Mesh>().color);
    ImGui::ColorEdit4("arrow color",
                      (float *)&arrow.get_component<Mesh>().color);

    ImGui::End();


	ImGui::Begin("FB texture");
// 	long int tex_id = framefuffer->get_color_attachment_id();
	long int tex_id = framefuffer->get_depth_attachment_id();
// 	long int tex_id = tex_tmp->get_texture_id();
	ImGui::Image((void*)tex_id, ImVec2(320., 180.), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
    //     bool show = true;
    //     ImGui::ShowDemoWindow(&show);
}

// EVENTS
void ExampleLayer::on_event(Event &event)
{
    EventDispacher dispatcher(event);

    dispatcher.dispatch<WindowResizeEvent>(
        bind(&ExampleLayer::on_window_resize_event, this, placeholders::_1));

    dispatcher.dispatch<KeyPressedEvent>(
        bind(&ExampleLayer::on_key_pressed_event, this, placeholders::_1));

    dispatcher.dispatch<KeyReleasedEvent>(
        bind(&ExampleLayer::on_key_released_event, this, placeholders::_1));


	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
    dispatcher.dispatch<MouseButtonPressedEvent>(bind(
        &ExampleLayer::on_mouse_button_pressed_event, this, placeholders::_1));

    dispatcher.dispatch<MouseButtonReleasedEvent>(bind(
        &ExampleLayer::on_mouse_button_released_event, this, placeholders::_1));

    dispatcher.dispatch<MouseMovedEvent>(
        bind(&ExampleLayer::on_curosr_moved_event, this, placeholders::_1));

    dispatcher.dispatch<MouseScrolledEvent>(
        bind(&ExampleLayer::on_mouse_scrolled_event, this, placeholders::_1));
	}
}


void ExampleLayer::on_mouse_button_released_event(MouseButtonReleasedEvent &event)
{
    int button_code = event.get_button_code();

    if (mouse_button_released_map.find(button_code) !=
        mouse_button_released_map.end())
        mouse_button_released_map[button_code](this);
}


void ExampleLayer::on_mouse_button_pressed_event(MouseButtonPressedEvent &event)
{
    int button_code = event.get_button_code();

    if (mouse_button_pressed_map.find(button_code) !=
        mouse_button_pressed_map.end())
        mouse_button_pressed_map[button_code](this);

    // 	cout << "on_mouse_button_pressed_event    ";
    // 	event.print_type();
}



void ExampleLayer::on_window_resize_event(WindowResizeEvent &event)
{
    ivec2 size = event.get_size();
    active_scene->on_resize(size.x, size.y);
	framefuffer->resize(size.x, size.y);
}



void ExampleLayer::on_key_pressed_event(KeyPressedEvent &event)
{
	int key_code = event.get_key_code();
	int repeat_count = event.get_repeat_count();

    if (key_pressed_map.find(key_code) != key_pressed_map.end())
		key_pressed_map[key_code](this);
}
//
void ExampleLayer::on_key_released_event(KeyReleasedEvent &event)
{
// 	cout << "on key released event" << endl;
	int key_code = event.get_key_code();
    if (key_released_map.find(key_code) != key_released_map.end())
		key_released_map[key_code](this);
}


void ExampleLayer::on_mouse_scrolled_event(MouseScrolledEvent &event)
{
}
//
void ExampleLayer::on_curosr_moved_event(MouseMovedEvent &event)
{
	vec2 cursor_shift;
	vec2 old_cursor_pos = Application::get_cursor_pos();
	vec2 new_cursor_pos = event.get_cursor_pos();

// 	if (new_cursor_pos.x < 250 && new_cursor_pos.y < 350)
// 		return;

	Application::set_cursor_pos(new_cursor_pos);

	cursor_shift = new_cursor_pos - old_cursor_pos;

	if (mouse_cursor_action != NULL)
		mouse_cursor_action(this, cursor_shift);
}

