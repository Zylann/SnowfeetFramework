//uniform sampler2D texture;
uniform float time;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	//vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	float c = rand(vec2(gl_FragCoord.x+time, gl_FragCoord.y));
	gl_FragColor = vec4(c, c, c, 1.0);
}

