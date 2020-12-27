#version 330 core
out vec4 FragColor;
// 传出一个vec4的颜色
in vec2 TexCoords;
// 传入一个vec2的位置坐标
uniform sampler2D texture_diffuse1;
// 纹理信息
void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}
