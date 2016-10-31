#version 410

uniform mat4 mvp;
in vec4 vtx;

void main(void)
{
    gl_Position = mvp * vtx;
}

