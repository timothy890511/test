#version 410 core

uniform sampler2D tex;

out vec4 color;

in VS_OUT
{
	vec2 texcoord;
} fs_in;

void main()
{
	// TODO 6: Do Red-Blue Stereo here
	float offset = 0.005;
	vec4 Left_Color = texture(tex, fs_in.texcoord + vec2(-offset, 0));
	vec4 Right_Color = texture(tex, fs_in.texcoord + vec2(offset, 0));

	float Color_R = Left_Color.x*0.299 + Left_Color.y*0.587 + Left_Color.z*0.114;
	float Color_G = Right_Color.y;
	float Color_B = Right_Color.z;
	color = vec4(Color_R,Color_G,Color_B,1.0);
}