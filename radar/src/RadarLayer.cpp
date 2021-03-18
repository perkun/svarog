#include "svpch.h"
#include "RadarLayer.h"


RadarLayer::RadarLayer(int dd_size) : dd_size(dd_size)
{
    auto window = Application::get_window();
    framebuffer = new Framebuffer(window->width, window->height,
                                  COLOR_ATTACHMENT | DEPTH_ATTACHMENT);

	frame_width = dd_size * 5;
	frame_height = dd_size * 5;

    pixel_buffer_r = new float[frame_width * frame_height];
    pixel_buffer_normal = new float[frame_width * frame_height];
    pixel_buffer_depth = new float[frame_width * frame_height];


    scene.on_resize(frame_width, frame_height);
    framebuffer->resize(frame_width, frame_height);
}


RadarLayer::~RadarLayer()
{
	delete framebuffer;

    delete[] pixel_buffer_r;
    delete[] pixel_buffer_depth;
    delete[] pixel_buffer_normal;
}

void RadarLayer::on_attach()
{
    model = scene.create_entity("model");
	model.add_component<Material>("radar");
	scene.target = model;

	observer = scene.create_entity("observer");
	Transform &ot = observer.get_component<Transform>();
    CameraComponent &cp = observer.add_component<CameraComponent>(
        make_shared<OrthograficCamera>(0.1, 1.0, 0.1, 10.));
	ot.position = vec3(0.5, 0.5, 0.);

	scene.observer = observer;

	scene.root_entity.add_child(model);
	scene.root_entity.add_child(observer);
}


void RadarLayer::load_model(string filename)
{
	model.remove_component<MeshComponent>();
	model.remove_component<OrbitalComponent>();

    MeshComponent &mc =
        model.add_component<MeshComponent>(filename);
    OrbitalComponent &oc = model.add_component<OrbitalComponent>(mc.header);

    Transform &mt = model.get_component<Transform>();
    mt.rotation = oc.xyz_from_lbg();
	mt.position = vec3(0., 3., 0.);

}


void RadarLayer::on_detach()
{

}


void RadarLayer::on_update(double time_delta)
{
	framebuffer->bind();
	framebuffer->clear();

	scene.on_update_runtime(time_delta);
}



void RadarLayer::make_radar_image(ImageSeries *radar_images, double jd)
{
    Entity target = this->model;
    Entity observer = this->observer;

    Material &mat = target.get_component<Material>();
    mat.shader = Application::shaders["radar"]; // just in case
    mat.uniforms_float["u_factor"] = 1.0;

    CameraComponent &camera_comp = observer.get_component<CameraComponent>();

    // create view box that fits the target perfectly
    Transform &tt = target.get_component<Transform>();
    Transform &ot = observer.get_component<Transform>();
    MeshComponent &mc = target.get_component<MeshComponent>();

    float max_scale = glm::max(tt.scale.x, tt.scale.y);
    max_scale = glm::max(max_scale, tt.scale.z);

    float near =
        glm::length(tt.position - ot.position) - (max_scale * mc.r_max);
    float far = glm::length(tt.position - ot.position) + (max_scale * mc.r_max);

//     TRACE("near: {}, far: {}, rmax: {}", near, far, mc.r_max);
    camera_comp.camera =
        make_shared<OrthograficCamera>(max_scale * mc.r_max, 1., near, far);

	// order makes a difference!
    camera_comp.camera->position = ot.position;
    camera_comp.camera->update_target(tt.position);


    Application::set_bg_color(vec4(0, 0, 0, 0));

    on_update(0.);

    framebuffer->bind();
    glReadPixels(0, 0, frame_width, frame_height, GL_RED, GL_FLOAT,
                 pixel_buffer_r);
    glReadPixels(0, 0, frame_width, frame_height, GL_GREEN, GL_FLOAT,
                 pixel_buffer_normal);
    glReadPixels(0, 0, frame_width, frame_height, GL_BLUE, GL_FLOAT,
                 pixel_buffer_depth);

    RadarImage *rimg = new RadarImage(target, observer, dd_size, dd_size, near,
                                      far, -1, 1, false);
    rimg->construct_delay_doppler(pixel_buffer_r, pixel_buffer_depth,
                                  pixel_buffer_normal, frame_width,
                                  frame_height);
	// julian day!
	rimg->julian_day = jd;
    radar_images->push(rimg);
}
