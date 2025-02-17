#version 460

layout(location = 0) in vec3 vertices;

uniform int uniformVar;
uniform int uniformVar2;

layout(std140) uniform uniformBlock{
    int val1;
    int val2;
};

void main(){
    gl_Position = vec4(vertices, uniformVar * uniformVar2 * val2 * val1);
}