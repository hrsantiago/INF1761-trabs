#version 130

uniform mat4 mvp;
in vec4 vtx;
in vec4 normal;
uniform vec4 eye;
uniform vec3 lights[10];
uniform int nLights;

varying vec4 v_Color;

void main(void)
{
	vec4 v = normalize(eye - vtx);
	vec4 Ia = 0.1 * vec4(1, 0, 0, 1); // ambient
	vec4 Ip = Ia;

	for(int i = 0; i < nLights; ++i) {
		vec4 l = normalize(vec4(lights[i], 1) - vtx);
		vec4 r = 2 * dot(l, normal) * normal - l;

		vec4 Is = 0.5 * pow(max(dot(r, v), 0), 40) * vec4(1, 1, 1, 1); // specular
		vec4 Id = 1 * max(dot(l, normal), 0) * vec4(1, 0, 0, 1); // diffuse
		Ip += Is + Id;
	}

	gl_Position = mvp * vtx;
	v_Color = Ip;
}
