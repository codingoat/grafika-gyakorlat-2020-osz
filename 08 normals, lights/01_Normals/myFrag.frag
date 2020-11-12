#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.2, 0.2, 0.2);

uniform vec3 Ld = vec3(0.4, 0.6, 0.6);
uniform vec3 Ls = vec3(1, 0, 0);

uniform vec3 Lpd = vec3(0, 0.6, 0);
uniform vec3 Lps = vec3(0, 1, 0);
uniform vec3 light_point_pos = vec3(5, 0, 0);

// anyagtulajdonságok: ambiens, diffúz, ...
uniform vec3 Ka = vec3(0.2, 0.4, 0.6);
uniform vec3 Kd = vec3(0.2, 0.4, 0.6);
uniform vec3 Ks = vec3(0.6, 0.8, 1);

uniform sampler2D texImage;

uniform vec3 cam_pos;

vec3 calcDir()
{
	//
	// diffúz szín számítása
	//	
	/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/

	vec3 light_dir_reverse = normalize(-light_dir);
	float diffuse_str = clamp(dot(light_dir_reverse, normalize(vs_out_norm)), 0, 1);
	vec3 diffuse = Ld * Kd * diffuse_str;

	//
	// fényfoltképzõ szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/

	vec3 light_reflected = normalize( reflect(light_dir, vs_out_norm));
	vec3 to_cam = normalize(cam_pos - vs_out_pos);
	float specular_str = pow( clamp(dot(light_reflected, to_cam), 0, 1), 50);
	
	vec3 specular = Ls * Ks * specular_str;

	return diffuse + specular;
}

vec3 calcPoint()
{
	vec3 light_dir_reverse = normalize(light_point_pos - vs_out_pos);
	float diffuse_str = clamp(dot(light_dir_reverse, normalize(vs_out_norm)), 0, 1);
	vec3 diffuse = Lpd * Kd * diffuse_str;


	vec3 light_reflected = normalize( reflect(-light_dir_reverse , vs_out_norm));
	vec3 to_cam = normalize(cam_pos - vs_out_pos);
	float specular_str = pow( clamp(dot(light_reflected, to_cam), 0, 1), 50);
	
	vec3 specular = Lps * Ks * specular_str;

	return diffuse + specular;
}


void main()
{	
	//
	// ambiens szín számítása
	//

	vec3 ambient = La * Ka;


	
	//
	// a fragment végsõ színének meghatározása
	//

	//fs_out_col = vec4(ambient + diffuse + specular, 1);

	// felületi normális
	// fs_out_col = vec4(vs_out_norm, 1);



	// textúrával
	//vec4 textureColor = texture(texImage, vs_out_tex);
	//fs_out_col = vec4(ambient + diffuse + specular, 1) * textureColor;
	//fs_out_col = texture(texImage, vs_out_tex);

	fs_out_col = vec4(ambient + calcDir() + calcPoint(), 1);
}

// Feladatok

// 1) Fényszámítás
// - ambiens
// - diffúz
// - spekuláris

// 2) Textúra
