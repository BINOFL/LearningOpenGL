#version 330 core

in  vec4 color;
in  vec2 uv0;
out vec4 fColor;

uniform sampler2D bodyTexture;

void main()
{
//    fColor = texture(bodyTexture, uv0);
    fColor = color;
//    fColor = vec4(1.0, 0.0, 0.0, 1.0);
}

