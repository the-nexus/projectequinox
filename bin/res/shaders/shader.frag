uniform int useTex;
uniform sampler2D heightmap;
uniform float height;

varying vec4 color;
varying vec4 position;

void main()
{
	vec4 col = color;
	if(position.y > 0.0){
		col = vec4(0.45, 0.65, 0.1, 1.0);
	}
	else if(position.y > -0.5){
		col = vec4(0.8, 0.8, 0.5, 1.0);
	}
	else {
		col = vec4(0.2, 0.45, 0.95, 1.0);
	}
	
	if(useTex == 1){
		col = col * texture2D(heightmap, gl_TexCoord[0].st);
	}
    gl_FragColor = col;
}
