#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.2, 0.2, 0.2);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);
uniform vec3 Ls = vec3(1.2, 1.2, 1.2);

uniform sampler2D texImage;

uniform vec3 cam_pos;

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	float cosa = clamp(dot(normal, to_light), 0, 1);
	vec3 diffuse = cosa*Ld;

	vec3 light_reflected = normalize( reflect(light_dir, vs_out_norm));
	vec3 to_cam = normalize(cam_pos - vs_out_pos);
	float specular_str = pow( clamp(dot(light_reflected, to_cam), 0, 1), 50);
	vec3 specular = Ls * specular_str;

	
	fs_out_col = vec4(ambient + diffuse + specular, 1) * texture(texImage, vs_out_tex);
}