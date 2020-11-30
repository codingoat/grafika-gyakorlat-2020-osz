#version 330 core

// VBO-ból érkezõ változók
in vec2 vs_in_uv;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform float time;

vec3 GetPos(float u, float v)
{
	return vec3(u, sin(u*v*100 + time) / 8, -v);
}

vec3 GetNorm(float u, float v)
{
	// Numerikusan (nem kell ismerni a képletet, elég a pozícióét)
	vec3 du = GetPos(u+0.01, v)-GetPos(u-0.01, v);
	vec3 dv = GetPos(u, v+0.01)-GetPos(u, v-0.01);

	return normalize(cross(du, dv));
}

void main()
{
	vec3 position = GetPos(vs_in_uv.x, vs_in_uv.y);
	gl_Position = MVP * vec4( position, 1 );
	
	vs_out_pos = (world * vec4(position, 1)).xyz;
	vs_out_norm = (worldIT * vec4(GetNorm(vs_in_uv.x, vs_in_uv.y), 0)).xyz;
}