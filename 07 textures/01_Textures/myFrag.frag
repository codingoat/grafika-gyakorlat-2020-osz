#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

uniform sampler2D texImageLoaded;
uniform sampler2D texImageGenerated;

uniform float time;

void main()
{
	// magassagtol fuggo mintavetelezes
	/*
	if(vs_out_pos.y > 1) 
		fs_out_col = texture(texImageLoaded, vs_out_tex0);
	else
		fs_out_col = texture(texImageGenerated, vs_out_tex0);
	*/

	// fs_out_col = vec4(sin(time), 0, 0, 0);

	fs_out_col = mix( texture(texImageLoaded, vs_out_tex0), texture(texImageGenerated, vs_out_tex0), (sin(time) + 1) * 0.5f);
	
}

// procedurális textúra...
