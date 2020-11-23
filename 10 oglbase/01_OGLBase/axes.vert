#version 330 core

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_col;

// shader külsõ paraméterei
uniform mat4 MVP;

vec4 positions[6] = vec4[6](
	vec4(0, 0, 0, 1),
	vec4(5, 0, 0, 1),

	vec4(0, 0, 0, 1),
	vec4(0, 5, 0, 1),

	vec4(0, 0, 0, 1),
	vec4(0, 0, 5, 1)
);

vec3 colors[3] = vec3[3](
	vec3(1, 0, 0),
	vec3(0, 1, 0),
	vec3(0, 0, 1)
);

void main()
{
	gl_Position = MVP * positions[gl_VertexID];
	vs_out_col = colors[gl_VertexID / 2];
}