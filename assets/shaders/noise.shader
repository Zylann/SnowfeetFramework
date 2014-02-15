// This is a special merged shader file format.

#vertex

// All usual vertex shader code goes here

void main()
{
	// transform the vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// transform the texture coordinates
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	// forward the vertex color
	gl_FrontColor = gl_Color;
}

#fragment

// All usual fragment shader code goes here

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

