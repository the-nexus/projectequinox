uniform sampler2D tex;
uniform float planetRadius;
uniform float planetHeight;

varying vec3 normal, lightDir, eyeVec;
varying vec4 initColor;
varying float dist;

void main()
{
	// calcul de la composante ambiante
	vec4 couleur = vec4(1.0, 1.0, 1.0, 1.0);
  
	// vecteur normal
	vec3 N = normal;
	if(!gl_FrontFacing)
	  N = -N;

	// direction de la lumière
	// calcul du vecteur de la surface vers la source lumineuse
	// normaliser le vecteur de la surface vers la source lumineuse
	vec3 L = normalize( lightDir );

	// produit scalaire pour le calcul de la réflexion diffuse
	// normale . direction de la lumière
	float NdotL = max(dot( N, L ), 0.0);
	couleur *= gl_FrontLightModelProduct.sceneColor + gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	// calcul de l'éclairage seulement si le produit scalaire est positif
	if (NdotL >= 0.0)
	{
		// calcul de la composante diffuse
		couleur += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * NdotL;
		float specular = 0.0;
		vec3 E = normalize( eyeVec );
		
	  
		// calcul de la composante spéculaire
		vec3 R = -reflect( L, N ); // réflexion de L par rapport à N
		// produit scalaire pour la réflexion spéculaire
		float NdotHV = max( dot( R, E ), 0.0 );
		specular = pow(NdotHV, gl_FrontMaterial.shininess);
		couleur += gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
		couleur +=  gl_FrontLightModelProduct.sceneColor + gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	}
	if(false){
		vec4 texCouleur = texture2D(tex, gl_TexCoord[0].st);
		couleur *= texCouleur;
	}
   
	vec3 finalColor = vec3(dist);
	
	
  
	gl_FragColor = couleur * vec4(clamp((finalColor * 0.5) + 0.5, 0.0, 1.0), 1.0) * initColor;
}
