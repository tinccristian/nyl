#pragma once

#include "system.h"
#include "shader.h"

namespace nyl
{
    class NYL_API ShaderSystem : public System
    {
    public:
        ShaderSystem() = default;
        ~ShaderSystem() override = default;
    };
}
