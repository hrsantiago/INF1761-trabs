#version 130

uniform vec4 eye;
uniform vec3 lights[10];
uniform int nLights;

out vec4 fragmentColor;
varying vec4 v_Position;
varying vec4 v_Normal; 

void main(void)
{
	vec4 v = normalize(eye - v_Position);

	vec4 Ia = 0.1 * vec4(0, 1, 0, 1); // ambient
	vec4 Ip = Ia;
	for(int i = 0; i < nLights; ++i) {
		vec4 l = normalize(vec4(lights[i], 1) - v_Position);
		vec4 r = 2 * dot(l, v_Normal) * v_Normal - l;

		vec4 Is = 0.5 * pow(max(dot(r, v), 0), 40) * vec4(1, 1, 1, 1); // specular
		vec4 Id = 1 * max(dot(l, v_Normal), 0) * vec4(0, 1, 0, 1); // diffuse
		Ip += Is + Id;
	}

	fragmentColor = Ip;
}
