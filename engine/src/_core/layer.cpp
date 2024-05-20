#include "layer.h"
#include <string>

namespace nyl{

    Layer::Layer(const std::string& debugName)
        :m_debugName(debugName){}

    Layer::~Layer(){}
}