#include"Texture.h"

namespace Nyl 
{
	Texture::Texture()
		:width(0)             , height(0),
		object_format(GL_RGB) , image_format(GL_RGB),
		wrap_s(GL_REPEAT)     , wrap_t(GL_REPEAT),
		filter_min(GL_NEAREST) , filter_max(GL_NEAREST)
	{
		glGenTextures(1, &this->ID);
	}

	void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data)
	{
		this->width = width;
		this->height = height;
		// create Texture
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, this->object_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
		// set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     this->wrap_s);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     this->wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}

}