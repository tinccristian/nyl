#include "animation.h"

namespace nyl
{
    Animation::Animation()
        : name(""), texture(nullptr), frameWidth(0), frameHeight(0), frameCount(1),
        currentFrame(0), frameTime(0.1f), elapsedTime(0.0f),
        isPlaying(false) {}

    Animation::Animation(const std::string& name, TextureComponent* texture, unsigned int frameWidth, unsigned int frameHeight, unsigned int frameCount, float frameTime)
        : name(name), texture(texture), frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount),
        currentFrame(0), frameTime(frameTime), elapsedTime(0.0f), isPlaying(true) {}

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

    AnimatedComponent::AnimatedComponent()
        : currentAnimationName("")
    {
    }

    void AnimatedComponent::AddAnimation(Animation animation)
    {
        animations[animation.name] = animation;
        if (currentAnimationName.empty()) {
            currentAnimationName = animation.name;
        }
    }

    void AnimatedComponent::SetCurrentAnimation(const std::string& name)
    {
        if (animations.find(name) != animations.end()) {
            currentAnimationName = name;
        }
    }

    void AnimatedComponent::Update(float deltaTime)
    {
        if (!currentAnimationName.empty()) {
            animations[currentAnimationName].Update(deltaTime);
        }
    }

    Animation* AnimatedComponent::GetCurrentAnimation()
    {
        if (!currentAnimationName.empty()) {
            return &animations[currentAnimationName];
        }
        return nullptr;
    }

    std::shared_ptr<TextureComponent> AnimatedComponent::GetCurrentTexture()
    {
        Animation* currentAnimation = GetCurrentAnimation();
        return currentAnimation ? std::shared_ptr<TextureComponent>(currentAnimation->texture) : nullptr;
    }

}