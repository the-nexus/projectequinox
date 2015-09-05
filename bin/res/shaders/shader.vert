uniform vec3 vecCamera;

varying vec3 normal, lightDir, eyeVec;
varying vec4 initColor;


void main( void )
{
   
   normal = normalize(gl_NormalMatrix * gl_Normal);
   
   vec3 ecPosition = vec3( gl_ModelViewMatrix * gl_Vertex );
   
   if(false){
      lightDir = vec3( gl_LightSource[0].position.xyz - ecPosition );
   } else{
      lightDir = vec3(gl_LightSource[0].position.xyz);
   }
   
   
   eyeVec = -vecCamera; // vecteur qui pointe vers le (0,0,0), c'est-à-dire vers l'oeil
   
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
   initColor = gl_Color;
   gl_Position = ftransform();
   
}
