#version 410 core

layout(location = 0) out vec4 fragColor;

uniform int u1iflag;

in vec3 vv3color;

void main()
{
	// TODO: Use the 'uniform int u1iflag' to switch the effect mode
	if (u1iflag == 0)
		fragColor = vec4(vv3color, 1.0);
	else
		fragColor = vec4(vec3(1.0, 1.0, 1.0) - vv3color, 1.0);
}

