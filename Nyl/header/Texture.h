#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>

namespace Nyl
{
	class Texture
	{
	public:
		unsigned int ID;
		unsigned int width,height;
		unsigned int object_format;
		unsigned int image_format;
		unsigned int wrap_s;
		unsigned int wrap_t;
		unsigned int filter_min;
		unsigned int filter_max;

		Texture();

		void Generate(unsigned int width, unsigned int height, unsigned char* data);
		// Binds a texture
		void Bind() const;

	};
}
#endif