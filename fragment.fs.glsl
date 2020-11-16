#version 410

layout(location = 0) out vec4 fragColor;

in VS_OUT {
	vec2 texcoord;
	flat int flag;
} fs_in;

uniform sampler2D us2dtex[2];

void main()
{
	// TODO 7: Sample texture
	if (fs_in.flag == 0) {
		fragColor = vec4(texture(us2dtex[0], fs_in.texcoord).rgb, 1.0);
	}
	else {
		fragColor = vec4(texture(us2dtex[1], fs_in.texcoord).rgb, 1.0);
	}
}