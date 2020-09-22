#version 130

// bemeneti változó - most a vertex shader-bõl (vagyis ottani out)
in vec4 vs_out_col;
in vec2 vs_out_pos;

// kimeneti változó - a fragment színe
out vec4 fs_out_col;

uniform vec2 move;
uniform float zoom;

void main()
{
	// vertexbol adott szin
	fs_out_col = vs_out_col;
	// csak feher
	fs_out_col = vec4(1, 1, 1, 1);
	// pozicio alapjan szin
	fs_out_col = vec4(vs_out_pos, 0, 1);


	//kor, korgyuru
	float r = 0.5;
	float rw = 0.1;
	vec2 pos = vec2(vs_out_pos.x * (640 / float(480)), vs_out_pos.y);

	if(length(pos) < r && length(pos) > r-rw)
	{
		fs_out_col = vec4(1, 1, 1, 1);
	}
	else
	{
		fs_out_col = vec4(0, 0, 0, 1);
	}


	vec2 z = vec2(0, 0);
	vec2 c = (pos + move) * zoom;
	int iter = 100;

	int i=0;
	for(; i<iter; i++)
	{
		z = vec2(pow(z.x, 2) - pow(z.y, 2), 2 * z.x * z.y) + c;
		if(length(z) > 2) break;
	}

	if(i == iter) fs_out_col = vec4(0, 0, 0, 1);
	else 
	{
		float brightness = i / float(iter);
		fs_out_col = vec4(vec3(brightness), 1);
	}
}