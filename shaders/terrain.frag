#version 450

// Per object texture sampler
layout(set = 1, binding = 1) uniform sampler2D tex_sampler_object;

// In
layout(location = 0) in vec3 i_color;
layout(location = 1) in vec2 i_tex_coord;
// Put
layout(location = 0) out vec4 out_color;

void main() {
    //out_color = vec4(i_tex_coord,0.0, 1.0);
	//out_color = texture(tex_sampler_object, i_tex_coord);
	out_color = vec4(i_color * texture(tex_sampler_object, i_tex_coord).rgb, 1.0);
}