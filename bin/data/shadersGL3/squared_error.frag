// fragment shader

#version 150

out vec4 outputColor;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

void main()
{

	vec4 tex0Sample = texture(tex0, gl_FragCoord.xy);
	vec4 tex1Sample = texture(tex1, gl_FragCoord.xy);

	// Assuming the source is actually grayscale so only consider red channel.
	float err = tex0Sample.r - tex1Sample.r;
	float sqErr = err * err;
	
    outputColor = vec4(sqErr, sqErr, sqErr, 1.0);

}