#ifndef RESOURCE_INITIALIZER_H
#define RESOURCE_INITIALIZER_H

#include "../Graphics/Texture.h"

class ResourceInitializer{
public:
	static Texture* getErrorTexture();
	static void initializeEntities();
private:
	static Texture* errorTex;
};

#endif