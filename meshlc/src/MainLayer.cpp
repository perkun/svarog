#include "svpch.h"
#include "MainLayer.h"
#include "Components.h"
#include "Framebuffer.h"
#include "Batch.h"
// #include "ImGuizmo.h"
#include "Math.h"
// #include "ModelController.h"
#include "AsteroidController.h"
#include "Renderer.h"
#include "Utils/FileDialog.h"
// #include "ObservatoryPanel.h"
#include "Utils/ObjHeader.h"
#include "Image.h"
#include <glm/gtc/type_ptr.hpp>


unsigned int Scene::num_scenes = 0;

MainLayer::MainLayer(Args args)
{
    this->args = args;
    TRACE("MainLayer constructed");

}

MainLayer::~MainLayer()
{

}


void MainLayer::on_attach()
{
    Renderer::set_line_width(5);
    auto window = Application::get_window();


    FramebufferSpecification ms_fb_specs;
    ms_fb_specs.attachments = {FramebufferTextureFormat::RGBA32F,
                               FramebufferTextureFormat::RED_INTEGER,
                               FramebufferTextureFormat::DEPTH32FSTENCIL8};
    ms_fb_specs.width = window->width;
    ms_fb_specs.height = window->height;
    ms_fb_specs.samples = 4;

    ms_framebuffer = new Framebuffer(ms_fb_specs);

    FramebufferSpecification fb_specs;
    fb_specs.attachments = {FramebufferTextureFormat::RGBA32F,
                            FramebufferTextureFormat::RED_INTEGER,
                            FramebufferTextureFormat::DEPTH32FSTENCIL8};
    fb_specs.width = window->width;
    fb_specs.height = window->height;
    fb_specs.samples = 1;

    framebuffer = new Framebuffer(fb_specs);

	if (args["num-points"])
		lc_num_points = args.get_value<int>("num-points");


    Entity model = scene.create_entity("Model");
    if (args["model"])
        model.add_component<MeshComponent>(
            args.get_positional<string>("model"));
    else
        model.add_component<MeshComponent>(ModelType::CUBE);

    Material &m = model.add_component<Material>("tex_sha_ls");
    m.uniforms_int["u_has_texture"] = 0;

    if (args["texture"])
    {
        model.add_component<TextureComponent>(
            make_shared<ImgTexture>(args.get_value<string>("texture")));
        m.uniforms_int["u_has_texture"] = 1;
    }


    ObjHeader &header = model.get_component<MeshComponent>().header;

    if (args["header"])
    {
        // if header is empty, everything is set to 0;
        // jd is set to now, and rotation phase is set
        model.add_component<OrbitalComponent>(header);
    }
    else
        // default lambda = 0; beta = 90
        model.add_component<OrbitalComponent>();

	OrbitalComponent &oc = model.get_component<OrbitalComponent>();

    if (args["spin"])
    {
        vector<double> spin = args.get_vec_values<double>("spin");

		oc.lambda = spin[0] * M_PI / 180.;
		oc.beta = spin[1] * M_PI / 180.;
		oc.gamma = spin[2] * M_PI / 180.;
    }


	if (args["jd0"])
		oc.jd_0 = args.get_value<double>("jd0");
	if (args["period"])
		oc.rot_period = args.get_value<double>("period");
	if (args["jd"])
		oc.set_rot_phase_at_jd(args.get_value<double>("jd"));


    vector<double> mp = args.get_vec_values<double>("model-pos");
    init_model_pos = vec3(mp[0], mp[1], mp[2]) * 100.f;

    Transform &mt = model.get_component<Transform>();
    mt.position = init_model_pos;
    mt.rotation = model.get_component<OrbitalComponent>().xyz_from_lbg();


    MeshComponent &mmc = model.get_component<MeshComponent>();

    // normalize model
    mt.scale = vec3(1.0 / mmc.r_max);
    TRACE("r_max: {}", mmc.r_max);

    model.add_component<NativeScriptComponent>().bind<AsteroidController>();



    vector<double> cp = args.get_vec_values<double>("observer-pos");
	vec3 init_cam_pos = vec3(cp[0], cp[1], cp[2]) * 100.f;

	float distance_model_obs = glm::length(init_cam_pos - init_model_pos);

    Entity runtime_observer = scene.create_entity("Observer");
    CameraComponent &rocp = runtime_observer.add_component<CameraComponent>(
        make_shared<OrthograficCamera>(cam_size_x, 1.0, distance_model_obs - 2, distance_model_obs + 2));


    rocp.camera->position = init_cam_pos;
    rocp.camera->update_target(init_model_pos);

    Transform &rot = runtime_observer.get_component<Transform>();


    Entity light = scene.create_entity("Light");
    light.add_component<LightComponent>();

    FramebufferSpecification light_fb_specs;
    light_fb_specs.attachments = {FramebufferTextureFormat::RGBA32F,
                                  FramebufferTextureFormat::DEPTH32FSTENCIL8};
    light_fb_specs.width = 2048;
    light_fb_specs.height = 2048;

    light.add_component<FramebufferComponent>(
        make_shared<Framebuffer>(light_fb_specs));

	float distance_model_light = glm::length(init_model_pos);
    light.add_component<CameraComponent>(
        make_shared<OrthograficCamera>(2.5, 1., distance_model_light -2, distance_model_light + 2));
    light.get_component<Transform>().scale = vec3(0.1);
	light.get_component<SceneStatus>().render = false;


    scene.root_entity.add_child(model);
    scene.root_entity.add_child(light);
    scene.root_entity.add_child(runtime_observer);

    scene.observer = runtime_observer;
    scene.target = model;
    scene.light = light;

    lightcurves = new LightcurveSeries;


    TRACE("making lc, {}", lc_num_points);
    make_lightcurve(lightcurves, lc_num_points);
    Lightcurve *lc = (Lightcurve *)lightcurves->get_current_obs();
	TRACE("size: {}", lc->size());
    TRACE("saving lc");
    lc->save_mag(args.get_value<string>("out"));

    Application::stop();
}


void MainLayer::on_detach()
{
    delete ms_framebuffer;
    delete framebuffer;
	delete lightcurves;
}


void MainLayer::on_update(double ts)
{

    ASSERT(scene.light.has_component<FramebufferComponent>(),
           "Scene light doesn't have Framebuffer Component");
    ASSERT(scene.light.has_component<CameraComponent>(),
           "Scene light doesn't have Camera Component");

    SceneStatus &lss = scene.light.get_component<SceneStatus>();
    bool tmp_render = lss.render;
    lss.render = false;

    Transform &lt = scene.light.get_component<Transform>();
    auto lc = scene.light.get_component<CameraComponent>().camera;

    lc->position = lt.position;
    if (scene.target)
        lc->update_target(scene.target.get_component<Transform>().position);
    lc->aspect = 1;

    auto fb = scene.light.get_component<FramebufferComponent>().framebuffer;
    fb->bind();
    fb->clear();

    Entity tmp_scene_observer = scene.observer;
    scene.observer = scene.light;

    scene.on_update_shadow();

    fb->bind_depth_texture(1);

    lss.render = tmp_render;
    scene.observer = tmp_scene_observer;


    ms_framebuffer->bind();
    ms_framebuffer->clear();
    ms_framebuffer->clear_attachment(1, -1); // ent_id "bg" to -1

    scene.on_update_runtime(ts);

    Framebuffer::blit(ms_framebuffer, framebuffer, 0, 0);
    Framebuffer::blit(ms_framebuffer, framebuffer, 1, 1);

    Renderer::bind_default_framebuffer();

	scene.update_scripts(ts);
}


void MainLayer::on_imgui_render()
{
    ImGui::Begin("Scene");

    ImVec2 vps = ImGui::GetContentRegionAvail();
    if (vps.x != viewport_panel_size.x || vps.y != viewport_panel_size.y)
    {
        viewport_panel_size.x = vps.x;
        viewport_panel_size.y = vps.y;

        auto cam = dynamic_pointer_cast<OrthograficCamera>(
            scene.observer.get_component<CameraComponent>().camera);
        float aspect = viewport_panel_size.x / viewport_panel_size.y;
        if (aspect > 1.) // wide screen
            cam->fov = cam_size_x * aspect;

        scene.on_resize(viewport_panel_size.x, viewport_panel_size.y);
        ms_framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        framebuffer->resize(viewport_panel_size.x, viewport_panel_size.y);
        Renderer::bind_default_framebuffer();
    }

    long int tex_id = framebuffer->get_color_attachment_id(0);
    ImGui::Image((void *)tex_id, ImVec2(vps.x, vps.y), ImVec2(0, 1),
                 ImVec2(1, 0));

    ImGui::End();
}


void MainLayer::on_event(Event &e)
{

	EventDispacher dispatcher(e);
    dispatcher.dispatch<KeyReleasedEvent>(
        bind(&MainLayer::on_key_released_event, this, placeholders::_1));
}

void MainLayer::on_key_released_event(KeyReleasedEvent &event)
{
    int key_code = event.get_key_code();

    if (key_code == GLFW_KEY_Q)
        Application::stop();
    else if (key_code == GLFW_KEY_L)
    {
        TRACE("making lc");
        make_lightcurve(lightcurves, lc_num_points);
        Lightcurve *lc = (Lightcurve *)lightcurves->get_current_obs();
        TRACE("saving lc");
        lc->save_mag("lc.dat");
    }
}

void MainLayer::make_lightcurve(LightcurveSeries *lightcurves, int num_points)
{
	Entity &target = scene.target;
	Entity &observer = scene.observer;
    Transform &tt = target.get_component<Transform>();
	if (!target.has_component<OrbitalComponent>())
	{
		cout << "target does not have orbital component" << endl;
		return;
	}

    OrbitalComponent &oc = target.get_component<OrbitalComponent>();

    int width = 256;
    int height = 256;
    float *pixel_buffer = new float[width * height];
    Lightcurve *lc = new Lightcurve(target, observer, num_points);

    observer.get_component<CameraComponent>().camera->update_target(
        tt.position);


    Application::set_bg_color(vec4(0., 0., 0., 1.));
    scene.on_resize(width, height);
    ms_framebuffer->resize(width, height);
    framebuffer->resize(width, height);

	TRACE("rot speed: {}", oc.rotation_speed);
	TRACE("time delta: {}",2 * M_PI / (double)num_points / oc.rotation_speed);


    for (int i = 0; i < num_points; i++)
    {
        if (oc.rotation_speed != 0)
            on_update(2 * M_PI / (double)num_points / oc.rotation_speed);
        else
            on_update(2 * M_PI / num_points);

        framebuffer->bind();
        glReadPixels(0, 0, width, height, GL_RED, GL_FLOAT, pixel_buffer);

        double flux = 0.0;
        for (int j = 0; j < width * height; j++)
            flux += pixel_buffer[j];
// 		printf("%.16lf\n", flux);
        lc->push_flux((double)i/num_points * 2*M_PI, flux);
    }

//     lc->julian_day = time_panel.julian_day;


    lightcurves->push(lc);

    delete[] pixel_buffer;
}
