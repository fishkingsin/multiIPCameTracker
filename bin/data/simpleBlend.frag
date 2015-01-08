//simpleBlend.frag
uniform sampler2DRect Tex0; 
uniform float alphaL;
uniform float alphaR;
uniform float sizeL;
uniform float sizeR;
void main()
{
	vec4 c = vec4(1.0,1.0,1.0,1.0);
	
		if(gl_TexCoord[0].x < sizeL )
		{
			c = vec4(1.0,1.0,1.0,(gl_TexCoord[0].x/sizeL)*alphaL);
			
		}
		else if( gl_TexCoord[0].x > sizeR )
		{
			c = vec4(1.0,1.0,1.0,alphaR);
		}
	

	gl_FragColor = texture2DRect(Tex0, gl_TexCoord[0].st)*c;
}