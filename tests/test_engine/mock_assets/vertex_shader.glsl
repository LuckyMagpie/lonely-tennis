#version 330 core

layout(location = 0) in vec3 vertex_data;
layout(location = 1) in vec2 uv_data;

out vec2 uv;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertex_data, 1);

    uv = uv_data;
}
