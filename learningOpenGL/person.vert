#version 330 core

layout (location = 0) in  vec3 vPosition;
layout (location = 1) in  vec3 vColor;
layout (location = 2) in  vec2 aTexCoords;

out vec4 color;
out vec2 uv0;

// uniform mat4 model;
// uniform mat4 view;
uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 u_Color;

void main()
{
    uv0 = vec2(aTexCoords.x, 1.0 - aTexCoords.y);
//    color = vec4(vColor, 1.0);
    color = u_Color;
//    color = vec4(0.2, 0.2, 0.2, 1.0);
//    color = draw_color;
    // gl_Position = projection * view * model * vec4(vPosition, 1.0);
    gl_Position = projection * model_view * vec4(vPosition, 1.0);
}
