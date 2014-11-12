#version 430 core

in vec4 fMyColor;
out vec4 outColor;
in vec2 Mapping;

uniform vec3 CameraPos;

vec4 circle(vec4 color)
{

    float lensqr = dot(Mapping, Mapping);

    if(lensqr > 1.0)
        discard;
    return (1.0f - lensqr) * color;
}

void main()
{
	vec4 color = circle(vec4(fMyColor.xyz, 1.0f));
	outColor = color;
}