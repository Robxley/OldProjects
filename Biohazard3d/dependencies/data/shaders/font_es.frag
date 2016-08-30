
precision mediump float;

varying vec2 texCoord;

uniform sampler2D texture0;
uniform vec4 uColor;

void main()
{
    vec4 color_t = texture2D( texture0, texCoord );
	color_t.r *= uColor.r;
	color_t.g *= uColor.g;
	color_t.b *= uColor.b;
	//color_t.a *= uColor.a;
    gl_FragColor = color_t;
}
