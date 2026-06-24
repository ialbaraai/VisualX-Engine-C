#version 330 core

out vec4 fragmentColor;
uniform float ourColor;

void main()
{
    fragmentColor = vec4(0.53f * ourColor, 0.95f * ourColor, 0.21f * ourColor, 1.0f);
}