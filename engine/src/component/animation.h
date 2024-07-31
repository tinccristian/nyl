#pragma once

#include "core.h"
#include "texture.h"
#include "component.h"

namespace nyl
{

class NYL_API Animation {
public:
    Animation();
    Animation(unsigned int frameWidth, unsigned int frameHeight, unsigned int frameCount, float frameTime = 0.1f);
    void Update(float deltaTime);
    
    unsigned int frameWidth;
    unsigned int frameHeight;
    unsigned int frameCount;
    unsigned int currentFrame;
    float frameTime;
    float elapsedTime;
    bool isPlaying;
};

class NYL_API AnimationComponent: public Component {
public:
    AnimationComponent(TextureComponent* texture);
    void Update(float deltaTime);
    
    TextureComponent* texture;
    Animation animation;
};
}