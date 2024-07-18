#pragma once

#include<glad/glad.h>

#include "component.h"
#include "core.h"

namespace nyl
{
    class NYL_API TextureComponent : public Component
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
        float frameWidth;
        float frameHeight;
        unsigned int frameCount;
        int currentFrame;
        float frameTime;
        float elapsedTime;
        bool isAnimated;
        TextureComponent();

        void Generate(unsigned int width, unsigned int height, unsigned char* data);
        void Update(float deltaTime);
        void Bind() const;

    };
}