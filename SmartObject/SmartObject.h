//
// Created by FireFlower on 2024/10/18.
//

#ifndef SMARTOBJECT_SMARTOBJECT_H
#define SMARTOBJECT_SMARTOBJECT_H

#include <sigslot/signal.hpp>

namespace fireflower {
	using namespace sigslot;
	
	class SmartObject;
	
	template<class T>
	concept BaseOfSmartObject = requires() {
		std::is_base_of_v<SmartObject, T>;
	};
	
	///智能对象
	class SmartObject {
		template<BaseOfSmartObject T>
		friend
		class SmartPointer;
	
	private:
		sigslot::signal<> destructed;
	
	public:
		virtual ~SmartObject();
	};
} // fireflower

#endif //SMARTOBJECT_SMARTOBJECT_H
