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
    
    float t = float(iteration) / float(maxIteration);
    float rt = 9 * (1 - t) * t*t*t;
    float gt = 15 * (1 - t) * (1 - t) * t*t;
    float bt = 8.5f * (1 - t) * (1 - t) * (1 - t) * t;

    fragColor = vec4(rt, gt, bt, 1.0f);
}    