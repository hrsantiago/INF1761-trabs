#version 130

uniform mat4 mvp;
in vec4 vtx;
in vec4 normal;

varying vec4 v_Position; 
varying vec4 v_Normal; 

void main(void)
{
	v_Normal = normal;
	v_Position = vtx;
	gl_Position = mvp * vtx;
}
