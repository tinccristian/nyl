// IGAME.h

#pragma once

namespace Nyl {

    class IGAME
    {
    public:
        virtual ~IGAME() = default;

        // Declare Init and Update functions to be implemented by Antares
        virtual int Init() = 0;
        virtual void Update() = 0;
        virtual void Cleanup() = 0;
    };

} // namespace Nyl
