#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{
//    vec3 uv = vec3(TexCoords.x, 1.0 - TexCoords.y, TexCoords.z);
    color = texture(skybox, TexCoords);
}
