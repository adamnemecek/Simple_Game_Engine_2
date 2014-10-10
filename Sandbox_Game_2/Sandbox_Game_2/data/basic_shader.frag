#version 440

smooth in vec4 theColor;

out vec4 outputColor;

void main()
{
	//float inv_r = 1.0 - theColor.r;
	//float inv_g = 1.0 - theColor.g;
	//float inv_b = 1.0 - theColor.b;
	////outputColor = theColor;
	//float lerpValue = gl_FragCoord.y / 500.0f;	
	////outputColor = mix(vec4(theColor.r, theColor.g, theColor.b, theColor.a), vec4(inv_r, inv_g, inv_b, theColor.a), 0.5);
	//outputColor = mix(vec4(theColor.r, theColor.g, theColor.b, theColor.a), vec4(0.2, 0.2, 0.2, 1.0), lerpValue);

   outputColor = theColor;
}
