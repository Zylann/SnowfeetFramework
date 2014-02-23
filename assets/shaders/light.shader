#vertex

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}

#fragment

//uniform sampler2D texture;
uniform sampler2D filterMap;
uniform vec2 lightPos;

void main()
{
	float resolution = 256.0;
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	//---

	float invRes = 1.0 / resolution;

	// Distance from the fragment to the light in pixels
	float distancePixels = distance(lightPos, gl_FragCoord.xy);

	// Samples actually needed to do the full raycast
	int neededSamples = int(distancePixels);
	//float invSamples = 1.0 / float(samples);

	// Fixed number of samples the shader will execute (for performance)
	int FIXED_SAMPLES = 200;
	float fixedInvSamples = 1.0 / float(FIXED_SAMPLES);

	// Raycast parameters in [0,1] space
    vec2 rayStart = lightPos * invRes;
	vec2 rayEnd = gl_FragCoord.xy * invRes;
	vec2 rayStep = normalize(rayEnd - rayStart) * invRes;

	vec2 rayPos = rayStart;
	int crossedSamples = 0;

	vec3 filteredLightColor = lightColor;// * (clamp(1.0 - 0.005*distancePixels, 0.0, 1.0));

	// Note: this loops a fixed number of times because it enables the compiler
	// to unroll the code, which makes the shader a lot faster.
	// Still, we need to check a variable amount of samples, so the step() function 
	// is used to disable light filtering above a certain number of loops.
	for(int i = 0; i < FIXED_SAMPLES; ++i)
	{
		// k==1 on the raycast segment, 0 if out
	    float k = 1.0 - step(float(neededSamples), float(crossedSamples));

	    // Get color filter
	    vec3 filterColor = texture2D(filterMap, vec2(rayPos.x, 1.0-rayPos.y)).rgb;

	    // Filter our "photon", or not if we got enough samples
	    filteredLightColor = mix(filteredLightColor, filteredLightColor*filterColor, k);

	    // Advance the ray
		rayPos += rayStep;

		++crossedSamples;
	}

    gl_FragColor = gl_Color * vec4(filteredLightColor, 1.0);
}

