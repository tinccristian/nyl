#pragma once

#include "system.h"
#include "component_shader.h"


class NYL_API ShaderSystem : public System {
public:
    ShaderSystem();
    virtual void update() override;
    virtual ~ShaderSystem() = default;
};