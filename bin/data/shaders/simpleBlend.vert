#version 120
varying vec4 Vertex_UV;
void main()
{
  // gl_Position = gxl3d_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	// Vertex_UV = gl_MultiTexCoord0;
}