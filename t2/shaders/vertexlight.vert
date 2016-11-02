#version 130

uniform mat4 mvp;
in vec4 vtx;
in vec4 normal;
uniform vec4 eye;
uniform vec4 light;

varying vec4 v_Color;

void main(void)
{
	vec4 v = normalize(eye - vtx);
	vec4 l = normalize(light - vtx);
	vec4 r = 2 * dot(l, normal) * normal - l;

	vec4 Ia = 0.1 * vec4(1, 1, 1, 1); // ambient
	vec4 Is = 0.2 * pow(dot(r, v), 1) * vec4(1, 1, 1, 1); // specular
	vec4 Id = 0.7 * dot(l, normal) * vec4(1, 0, 0, 1); // diffuse
	vec4 Ip = Ia + Is + Id;

	gl_Position = mvp * vtx;
	v_Color = Ip;
}

