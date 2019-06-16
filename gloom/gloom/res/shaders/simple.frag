#version 450 core

in layout(location = 0) vec3 normal_in;
in layout(location = 1) vec3 v;
in layout(location = 2) vec2 texCoords;
in layout(location = 3) vec3 texCoords3D;
in layout(location = 4) vec3 vp;
in layout(location = 12) mat2x3 lightPositions;


uniform layout(location = 7) int is3D;
uniform layout(binding = 0) sampler2D tex1;
uniform layout(binding = 1) sampler3D tex2;

out vec4 color;

void main()
{
	if ( is3D == 1) {
		float steps = 100;
		float stepSize = 0.01;
		float alpha = 0;
		vec4 tempColor = vec4(0);

		// find ray for ray march
		vec3 ray_direction = normalize(vp);
		vec3 step = stepSize * ray_direction;
		step = vec3(-step.x, -step.y, step.z);
		
		vec3 tempCoords = texCoords3D;
		for (int i = 0; i < steps; i++) {
			// find values in perlin noise
			tempColor = texture(tex2, tempCoords).rgba;
			if ( tempColor[3] > 0.3) { // treshold to remove noise
				alpha += pow(2,((tempColor[3]*tempColor[3]))/20)-1; // tranfer funciton for how much alpha that should be added
			}
			// continue march
			if (step.y >= 0) {
				step.y = - step.y;
			}
			tempCoords -= step;
			
			
			if (tempCoords.x > 1 || tempCoords.x < 0) {
				break;
			}
			if (tempCoords.y > 1 || tempCoords.y < 0) {
				break;
			}
			if (tempCoords.z > 1 || tempCoords.z < 0) {
				break;
			}
		}
		color = vec4(1,1,1,alpha);
	} else {
		//constants
		float shininess = 100.0f;
		float lightReach = 0.003f;

		// vectors
		vec3 eye_direction = normalize(-v);
		vec3 light_direction, reflected_light_dir;
		vec4 diff_comp, spec_comp;
		vec2 light_intensity = vec2(1.1, 0.5); // how much light each light contributes

		// colors
		color = vec4(0, 0, 0, 1);
		vec4 ambient = vec4(0);
		vec4 emission = vec4(0); 
		vec4 specular_color = vec4(1); // white
		vec4 light_color = vec4(0);
		vec4 textureColour = vec4(1, 1, 1, 1);
		textureColour = texture(tex1, texCoords).rgba;

		vec3 normal = normalize(normal_in);

		for (int i = 0; i < 2; i++) {
				vec3 surfaceLightVector = lightPositions[i] - v;
				float lightDistance = length(surfaceLightVector);
				light_direction = normalize(surfaceLightVector);
				reflected_light_dir = normalize(reflect(-light_direction, normal));

				// diffuse part
				float diffuse_intensity = max(dot(normal, light_direction), 0.0f);
				diff_comp = vec4(textureColour.rgb*diffuse_intensity, 1.0);

				// specular part
				spec_comp = clamp(pow(max(dot(reflected_light_dir, eye_direction), 0.0f), shininess)*specular_color, 0.0f, 0.05f); // clamps down the shinyness

				light_color += ((diff_comp + spec_comp) / (lightReach * lightDistance))*light_intensity[i];
			}
		color +=  ambient + emission + light_color;
	}
}
