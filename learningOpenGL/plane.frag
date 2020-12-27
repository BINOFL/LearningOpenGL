#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
//    // ambient
//    vec3 ambient = light.ambient * texture(texture1, TexCoords).rgb;
    vec3 ambient = 0.5 * texture(texture1, TexCoords).rgb;
//
    vec4 empty = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 planeColor = vec4(ambient, 0.5);
    FragColor = mix(empty, planeColor, 1.0);
    
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
