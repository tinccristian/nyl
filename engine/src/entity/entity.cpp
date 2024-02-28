#include "entity.h"

int Entity::nextId = 0;
std::queue<int> Entity::freeIds;