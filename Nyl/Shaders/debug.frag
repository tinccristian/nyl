// Fragment shader
#version 330 core
out vec4 FragColor;
uniform vec3 debugColor;
void main()
{
    FragColor = vec4(debugColor, 1.0);
}