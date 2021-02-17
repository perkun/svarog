#include "svpch.h"
#include "RadarImage.h"



void RadarImage::construct_delay_doppler(float *radial_vel_buffer,
                                          float *depth_bufer,
                                          float *normal_buffer, int width,
                                          int height)
{
    int radar_width = texture_specs.width;
    int radar_height = texture_specs.height;

    // find min and max vr and d
    float d_max = depth_bufer[0];
    float d_min = depth_bufer[0];
    float vr_min = radial_vel_buffer[0];
    float vr_max = radial_vel_buffer[0];

    for (int i = 0; i < width * height; i++)
    {
        if (radial_vel_buffer[i] > vr_max)
            vr_max = radial_vel_buffer[i];
        if (radial_vel_buffer[i] < vr_min)
            vr_min = radial_vel_buffer[i];

        if (depth_bufer[i] > d_max)
            d_max = depth_bufer[i];
        if (depth_bufer[i] < d_min)
            d_min = depth_bufer[i];
    }


    TRACE("vr min/max, d min/max: {}, {} | {}, {}", vr_min, vr_max, d_min,
          d_max);

    vr_min = -1.;
    vr_max = 1.0;

    d_min = 0.;
    d_max = 1.;


    float *delay_doppler = new float[radar_width * radar_height];
    for (int i = 0; i < radar_width * radar_height; i++)
        delay_doppler[i] = 0.;

    for (int i = 0; i < width * height; i++)
    {
        if (depth_bufer[i] == 0.) // background
            continue;

        int vr = floor(radar_width * (radial_vel_buffer[i] - vr_min) /
                       (vr_max - vr_min));
        int d =
            floor(radar_height * (depth_bufer[i] - d_min) / (d_max - d_min));

        // 		if (i == 200*400 + 170)
        // 			TRACE("vr, d: {}, {}", vr, d);

        if (d < 0 || d >= radar_height)
            continue;
        if (vr < 0 || vr >= radar_width)
            continue;

        delay_doppler[d * radar_width + vr] += normal_buffer[i];
    }

    // normalize delay_doppler
    float dd_max = delay_doppler[0];
    for (int i = 0; i < radar_width * radar_height; i++)
        if (delay_doppler[i] > dd_max)
            dd_max = delay_doppler[i];
    for (int i = 0; i < radar_width * radar_height; i++)
        delay_doppler[i] /= dd_max;

    texture->update(delay_doppler, delay_doppler, delay_doppler);

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
