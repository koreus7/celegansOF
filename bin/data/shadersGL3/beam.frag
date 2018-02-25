// fragment shader

#version 150

out vec4 outputColor;

#define PI 3.14159265359


uniform float beamWidth;
uniform float beamLength;

void main()
{

	float size = 256.0;

	vec2 centeredPos = gl_FragCoord.xy - size/2;

	float per = clamp(centeredPos.x/(beamWidth * 0.5), -1.0, 1.0);

	float g =  0.5 * (cos(PI*per) + 1);

	if( abs(centeredPos.y) > beamLength/2.0 ) {
		
		g = 0.0;

	}

    outputColor = vec4(g,g,g,1.0);
}