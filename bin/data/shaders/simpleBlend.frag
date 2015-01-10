#version 120
uniform sampler2DRect tex0;
uniform float width;
uniform float height;
uniform float alphaL;
uniform float alphaR;
uniform float sizeL;
uniform float sizeR;
const float PI = 3.1415926535;
const float HLAF_PI = PI*0.5;
// varying vec4 Vertex_UV;
uniform float BarrelPower;
// uniform vec2 radialdist;
// uniform vec2 tangentdist;
// uniform vec2 focal;
// uniform vec2 center;
vec2 Distort(vec2 p)
{
		float theta  = atan(p.y, p.x);
		float radius = length(p);
		radius = pow(radius, BarrelPower);
		p.x = radius * cos(theta);
		p.y = radius * sin(theta);
		return 0.5 * (p + 1.0);
}

void main(void)
{

	vec2 xy = 2.0 * (gl_TexCoord[0].xy / vec2(width,height))- 1.0;
	vec2 uv;
	float d = length(xy);
	// if (d < 1.0)
	{
		uv = Distort(xy)*vec2(width,height);
	}
	// else
	{
		// uv = gl_TexCoord[0].xy;
	}
	vec4 c = vec4(1.0,1.0,1.0,1.0);
	
		if(gl_TexCoord[0].x < sizeL )
		{
			
			c = vec4(1.0,1.0,1.0,sin((gl_TexCoord[0].x/sizeL)*HLAF_PI*alphaL)*alphaL);
			
		}
		else if( gl_TexCoord[0].x > width-sizeR )
		{
			float a = sin(((width-gl_TexCoord[0].x)/sizeR)*HLAF_PI*alphaL)*alphaR;
			c = vec4(1.0,1.0,1.0,a);
		}
	gl_FragColor = texture2DRect(tex0, uv)*c;
}
