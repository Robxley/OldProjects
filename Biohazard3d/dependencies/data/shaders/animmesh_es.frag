precision mediump float;

varying vec3 color;
varying vec2 texCoord;



uniform sampler2D textureSampler;
 
void main()
{
    vec4 color_t = texture2D( textureSampler, texCoord );
    gl_FragColor = color_t;

}
