#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// ir�ny f�nyforr�s: f�ny ir�nya
uniform vec3 light_dir = vec3(-1,-1,-1);

// f�nytulajdons�gok: ambiens, diff�z, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D texImage;
uniform samplerCube texSkybox;

uniform vec3 camPos;

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;


	vec3 camToFrag = normalize(vs_out_pos - camPos);
	vec3 reflected = reflect(camToFrag, normalize(vs_out_norm));
	vec4 skyboxColor = texture(texSkybox, reflected);


	
	fs_out_col = mix(vec4(ambient + diffuse, 1) * texture(texImage, vs_out_tex), skyboxColor, 0.2);
}