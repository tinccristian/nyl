#pragma once

#include "core.h"
#include "texture.h"
#include "component.h"
#include <unordered_map>
#include <string>

namespace nyl
{

    class NYL_API Animation {
    public:
        Animation();
        Animation(const std::string& name, TextureComponent* texture, unsigned int frameWidth, unsigned int frameHeight, unsigned int frameCount, float frameTime = 0.1f);
        void Update(float deltaTime);

        std::string name;
        std::shared_ptr<TextureComponent> texture;
        unsigned int frameWidth;
        unsigned int frameHeight;
        unsigned int frameCount;
        unsigned int currentFrame;
        float frameTime;
        float elapsedTime;
        bool isPlaying;
    };

    class NYL_API AnimatedComponent : public Component {
    public:
        AnimatedComponent();
        void AddAnimation(Animation animation);
        void SetCurrentAnimation(const std::string& name);
        void Update(float deltaTime);
        Animation* GetCurrentAnimation();
        std::shared_ptr<TextureComponent> GetCurrentTexture();

    private:
        std::unordered_map<std::string, Animation> animations;
        std::string currentAnimationName;
    };

}