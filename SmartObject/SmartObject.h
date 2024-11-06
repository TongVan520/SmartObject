//
// Created by FireFlower on 2024/10/18.
//

#ifndef SMARTOBJECT_SMARTOBJECT_H
#define SMARTOBJECT_SMARTOBJECT_H

#include <sigslot/signal.hpp>
#include "SmartDefine.h"
#include <type_traits>
#include "../TypeWrapper/TypeWrapper.h"

namespace fireflower {
	using namespace sigslot;
	
	/// @类名 智能对象
	/// @描述 智能指针的辅助类
	class SmartObject {
		template<BaseOfSmartObject T>
		friend
		class SmartPointer;
	
	private:
		sigslot::signal<> destructed;
	
	public:
		virtual ~SmartObject();
	};
	
	/// @类名 智能包装器
	/// @描述 可将满足以下所有条件的一切类型包装成智能对象：\n
	/// - 可构造的类型\n
	/// - <b>不</b>继承自<code>SmartObject</code>的类型\n
	template<class T> requires (HasConstructor<T> and not BaseOfSmartObject<T>)
	class SmartWrapper : public SmartObject, public TypeWrapper<T> {
	public:
		using TypeWrapper<T>::TypeWrapper;
	};
} // fireflower

#endif //SMARTOBJECT_SMARTOBJECT_H
