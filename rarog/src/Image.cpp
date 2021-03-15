#include "svpch.h"
#include "Image.h"

Image::Image(Entity &target, Entity &observer, int width, int height)
	: Observation(target, observer)
{
	this->width = width;
	this->height = height;
	texture_format = GL_RGB;

	data_r = new float[width * height];
	data_g = new float[width * height];
	data_b = new float[width * height];

	texture_specs.target = GL_TEXTURE_2D;
	texture_specs.level = 0;
	texture_specs.internal_format = GL_RGB32F;
	texture_specs.width = width;
	texture_specs.height = height;
	texture_specs.depth = 1;
	texture_specs.border = 0;
	texture_specs.format = texture_format;
	texture_specs.type = GL_FLOAT;

	texture = make_shared<Texture>(texture_specs);

	ghost_color = vec4(237 / 256., 84 / 256., 84 / 256., 0.2);
}


Image::~Image()
{
	delete[] data_r;
	delete[] data_g;
	delete[] data_b;
}


void Image::serialize(YAML::Emitter &out)
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

	out << YAML::Key << "type";
	out << YAML::BeginSeq <<  YAML::Value << "IMAGE" << YAML::EndSeq;

	out << YAML::EndMap;
}

FitsHeader Image::get_fits_header()
{
	FitsHeader header;
	push_basic_header_info(header);

	header.push("type", "image", "observation type");
	return header;
}

void Image::save_png(const char* filename)
{
    stbi_flip_vertically_on_write(1);
    int channels = 3;
//     if (texture_format == GL_RGBA)
//         channels = 4;
//     else if (texture_format == GL_RGB)
//         channels = 3;
//     else if (texture_format == GL_RED || texture_format == GL_BLUE ||
//              texture_format == GL_BLUE)
//         channels = 1;

    // TODO: match data type to the specs.type !!
    float *data = new float[width * height * channels];


//     texture->bind();
//     glGetTexImage(texture->specs.target, 0, texture_format, texture->specs.type, data);

    unsigned char *png_data =
        new unsigned char[width * height * channels];

    for (int i = 0; i < width * height; i++)
    {
        png_data[i*channels + 0] = (unsigned char)(data_r[i] * 255);
        png_data[i*channels + 1] = (unsigned char)(data_g[i] * 255);
        png_data[i*channels + 2] = (unsigned char)(data_b[i] * 255);
    }

    stbi_write_png(filename, width, height, channels, png_data,
                   width * channels);

    delete[] png_data;
}


void Image::save_fits_greyscale(const char *filename, const FitsHeader &header)

{
    int status = 0, bitpix, num_axis;
    long axis_dim[10], fpixel[2] = {1, 1};

    bitpix = -32; // (32 - int, 64 - long int, -32 - float, -64 - double)
    axis_dim[0] = width;
    axis_dim[1] = height;
    num_axis = 2;

    fitsfile *out;
    fits_create_file(&out, filename, &status);
    if (status != 0)
    {
        cout << "status: " << status << " coud not create file " << filename
             << "\n";
        return;
    }

    int channels = 1;
    if (texture_format == GL_RGBA)
        channels = 4;
    else if (texture_format == GL_RGB)
        channels = 3;
    else if (texture_format == GL_RED || texture_format == GL_BLUE ||
             texture_format == GL_BLUE)
        channels = 1;
    // TODO: match data type to the specs.type !!
//     float *data = new float[width * height * channels];

	float *fits_data = new float[width * height];

//     bind();
//     glGetTexImage(specs.target, 0, specs.format, specs.type, data);

    for (int i = 0; i < width * height; i++)
        fits_data[i] = data_r[i];

	fits_create_img(out, bitpix, num_axis, axis_dim, &status);
	fits_write_pix(out, TFLOAT, fpixel, axis_dim[0]*axis_dim[1],
			fits_data, &status);

	header.write(out);

// 	header.addHeaderToFile(out);

	fits_close_file(out, &status);

	delete[] fits_data;
}


void Image::update_data(float *dr, float *dg, float *db)
{
	for (int i = 0; i < width * height; i++)
	{
		data_r[i] = dr[i];
		data_g[i] = dg[i];
		data_b[i] = db[i];
	}
}


void Image::update_texture()
{
	texture->update(data_r, data_g, data_b);
}



AoImage::AoImage(Entity &target, Entity &observer, int width, int height)
	: Image(target, observer, width, height)
{
// 	TextureSpec specs;
//
// 	specs.target = GL_TEXTURE_2D;
// 	specs.level = 0;
// 	specs.internal_format = GL_RGB32F;
// 	specs.width = width;
// 	specs.height = height;
// 	specs.depth = 1;
// 	specs.border = 0;
// 	specs.format = texture_format;
// 	specs.type = GL_FLOAT;
//
// 	texture = make_shared<Texture>(specs);

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

FitsHeader AoImage::get_fits_header()
{
	FitsHeader header;
	push_basic_header_info(header);

	header.push("type", "Adaptive Optics", "observation type");
	return header;
}


void RadarImage::construct_delay_doppler(float *radial_vel_buffer,
                                         float *depth_buffer,
                                         float *normal_buffer,
                                         int buffer_width,
                                         int buffer_height)
{
    int delay_dopler_width = width;
    int delay_dopler_height = height;

    float *delay_doppler = new float[delay_dopler_width * delay_dopler_height];
    for (int i = 0; i < delay_dopler_width * delay_dopler_height; i++)
        delay_doppler[i] = 0.;

    for (int i = 0; i < buffer_width * buffer_height; i++)
    {
        if (depth_buffer[i] == 0.) // background
            continue;

        int vr = floor(delay_dopler_width * (radial_vel_buffer[i] - min_vr) /
                       (max_vr - min_vr));
        int d = floor(delay_dopler_height * (depth_buffer[i] - min_depth) /
                      (max_depth - min_depth));

        // 		if (i == 200*400 + 170)
        // 			TRACE("vr, d: {}, {}", vr, d);

        if (d < 0 || d >= delay_dopler_height)
            continue;
        if (vr < 0 || vr >= delay_dopler_width)
            continue;

        delay_doppler[d * delay_dopler_width + vr] += normal_buffer[i];
    }

    // normalize delay_doppler
    float dd_max = delay_doppler[0];
    for (int i = 0; i < delay_dopler_width * delay_dopler_height; i++)
        if (delay_doppler[i] > dd_max)
            dd_max = delay_doppler[i];
    for (int i = 0; i < delay_dopler_width * delay_dopler_height; i++)
        delay_doppler[i] /= dd_max;

	update_data(delay_doppler, delay_doppler, delay_doppler);

    delete[] delay_doppler;
}

void RadarImage::serialize(YAML::Emitter &out)
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


	out << YAML::Key << "type";
	out << YAML::BeginSeq <<  YAML::Value << "RADAR" << YAML::EndSeq;

	out << YAML::EndMap;
}

FitsHeader RadarImage::get_fits_header()
{
	vec2 tex_dim = texture->get_dimentions();
	double c = 299792458.0; // m/s
	double wavelen = 0.1259632; // 2.38 GHz (arecibo)

	FitsHeader header;
	push_basic_header_info(header);

	header.push("type", "Delay-Dopler", "observation type");
// 	header.push("min_d", min_depth, "depth at first pixel row");
// 	header.push("max_d", max_depth, "depth at last pixel row");
// 	header.push("min_vr", min_vr, "radial velocity at first pixel column");
// 	header.push("max_vr", max_vr, "radial velocity at last pixel column");

	// WCS

	header.push("WCSAXES", 2, "");
	header.push("WCSNAME", "delay-Doppler", "");

	header.push("CRPIX1", tex_dim[0]/2., "");
	header.push("CRVAL1", 0.0, "");
	header.push("CDELT1", 2.0 / wavelen * (max_vr - min_vr)/tex_dim[0], "");
	header.push("CTYPE1", "DOPPLER", "Doppler Shift");
	header.push("CUNIT1", "Hz", "");

	header.push("CRPIX2", 0, "");
	header.push("CRVAL2", 2.0 / c * 1e9 * min_depth, "");
	header.push("CDELT2", 2.0 / c * 1e9 * (max_depth - min_depth)/tex_dim[1], "");
	header.push("CTYPE2", "DELAY", "Round-Trip Time Delay");
	header.push("CUNIT2", "us", "");

	header.push("WAVELEN", wavelen, "radar wavelength");

	return header;

}
