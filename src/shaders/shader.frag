#version 460 core

in vec3 coords;

out vec4 fragColor;

uniform float zoomMultiplier;
uniform float xoffset;
uniform float yoffset;

void main()
{
    vec3 color;
    int iteration = 0, maxIteration = 100;
    double x = 0, y = 0;
    while ((x * x + y * y) <= 4 && iteration < maxIteration)
    {
        double xTemp = x * x - y * y + (xoffset + coords.x / zoomMultiplier) * 2.5;
        y = 2 * x * y + (yoffset + coords.y / zoomMultiplier) * 1.40625;
        x = xTemp;
        ++iteration;
    }
    if (iteration == maxIteration)
    {
        color = vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        color = vec3(0.6f, 0.2f, 0.6f);
    }

    fragColor = vec4(color, 1.0f);
}    