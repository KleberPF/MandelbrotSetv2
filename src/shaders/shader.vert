#version 460 core
    
layout (location = 0) in vec3 aPos;

out vec3 coords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    coords = vec3(aPos.x, aPos.y, aPos.z);
}  