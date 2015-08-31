uniform int useTex;
uniform sampler2D heightmap;
uniform float height;

varying vec4 color;
varying vec4 position;

void main(){
	color = gl_Color.rgba;
	position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
