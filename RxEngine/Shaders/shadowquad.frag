#version 330


out vec4 out_Color;
uniform vec4 shadowcolor;

void main()
{
    out_Color = shadowcolor;
}
