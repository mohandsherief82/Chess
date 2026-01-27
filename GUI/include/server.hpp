
#pragma once

#include "interfaces.hpp"

namespace Chess
{
    class AIOpponent: public Concrete::Observer
    {
        public:
            void update() override;
    };
}
