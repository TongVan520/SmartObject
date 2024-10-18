//
// Created by FireFlower on 2024/10/18.
//

#include "SmartObject.h"

namespace fireflower {
	SmartObject::~SmartObject() {
		this->destructed();
	}
} // fireflower