#version 330 core

out vec4 fragmentColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform sampler2D texture1;

void main()
{
    fragmentColor = texture2D(texture1, texCoord);
}