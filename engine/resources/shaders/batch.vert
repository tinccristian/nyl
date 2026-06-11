#version 330 core
layout (location = 0) in vec2 aPos;   // world-space position
layout (location = 1) in vec2 aUV;    // texture coordinates
layout (location = 2) in vec4 aColor; // tint (rgba)

out vec2 vUV;
out vec4 vColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vUV = aUV;
    vColor = aColor;
    gl_Position = projection * view * vec4(aPos, 0.0, 1.0);
}
