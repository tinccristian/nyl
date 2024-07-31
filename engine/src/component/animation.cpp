#include "animation.h"

namespace nyl
{

    Animation::Animation()
        : frameWidth(0), frameHeight(0), frameCount(1),
        currentFrame(0), frameTime(0.1f), elapsedTime(0.0f),
        isPlaying(false){}
    Animation::Animation(unsigned int frameWidth, unsigned int frameHeight, unsigned int frameCount, float frameTime)
        : frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount),currentFrame(0), frameTime(frameTime), elapsedTime(0.0f),isPlaying(true){}

    void Animation::Update(float deltaTime)
    {
        if (isPlaying) {
            elapsedTime += deltaTime;
            if (elapsedTime >= frameTime) {
                currentFrame = (currentFrame + 1) % frameCount;
                elapsedTime = 0.0f;
            }
        }
    }

    AnimationComponent::AnimationComponent(TextureComponent* texture)
        : texture(texture)
    {
    }

    void AnimationComponent::Update(float deltaTime)
    {
        animation.Update(deltaTime);
    }

}