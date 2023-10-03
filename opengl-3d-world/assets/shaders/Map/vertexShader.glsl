#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

out vec2 FragTexCoord;
out float Height;
out vec3 FragPos;

void main() {
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
    FragTexCoord = a_TexCoord.xy;
    Height = a_Position.y;
    FragPos = vec3(gl_Position);
}