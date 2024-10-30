//
// Created by FireFlower on 2024/10/18.
//

#ifndef SMARTOBJECT_SMARTOBJECT_H
#define SMARTOBJECT_SMARTOBJECT_H

#include <sigslot/signal.hpp>
#include "SmartDefine.h"

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
	/// @描述 可将一切类型包装成智能对象
	template<HasConstructor T>
	class SmartWrapper : public SmartObject {
	private:
		T object;
	
	public:
		template<class... Args>
		SmartWrapper(Args&& ... args);
		
		SmartWrapper(const SmartWrapper& other) = delete;
		
		SmartWrapper(SmartWrapper&& other) requires HasMoveConstructor<T>;
		
		SmartWrapper& operator=(const SmartWrapper& other) requires CanCopyAssign<T>;
		
		SmartWrapper& operator=(SmartWrapper&& other) requires CanMoveAssign<T>;
		
		virtual ~SmartWrapper() = default;
		
		inline operator T&();
		
		inline operator const T&() const;
		
		/// @名称 获取原始对象的指针
		/// @描述 虽然是个对象但请以指针的方式调用原始对象的方法。因为无法重载<code>.</code>运算符！
		/// @返回值 原始对象的指针
		inline T* operator->();
		
		/// @名称 获取原始对象的常指针
		/// @描述 虽然是个对象但请以指针的方式调用原始对象的方法。因为无法重载<code>.</code>运算符！
		/// @返回值 原始对象的常指针
		inline const T* operator->() const;
		
		/// @名称 获取原始引用
		/// @返回值 原始引用
		[[nodiscard]] inline const T& getPrimordialReference() const;
	};
	
	template<HasConstructor T>
	SmartWrapper<T>::SmartWrapper(SmartWrapper&& other) requires HasMoveConstructor<T>
			: object(std::move(other.object)) {
		
	}
	
	template<HasConstructor T>
	SmartWrapper<T>& SmartWrapper<T>::operator=(const SmartWrapper& other) requires CanCopyAssign<T> {
		this->object = other.object;
		return *this;
	}
	
	template<HasConstructor T>
	SmartWrapper<T>& SmartWrapper<T>::operator=(SmartWrapper&& other) requires CanMoveAssign<T> {
		this->object = std::move(other.object);
		return *this;
	}
	
	template<HasConstructor T>
	SmartWrapper<T>::operator const T&() const {
		return this->object;
	}
	
	template<HasConstructor T>
	SmartWrapper<T>::operator T&() {
		return this->object;
	}
	
	template<HasConstructor T>
	template<class... Args>
	SmartWrapper<T>::SmartWrapper(Args&& ... args) : object(args...) {
	
	}
	
	template<HasConstructor T>
	T* SmartWrapper<T>::operator->() {
		return &this->object;
	}
	
	template<HasConstructor T>
	const T* SmartWrapper<T>::operator->() const {
		return &this->object;
	}
	
	template<HasConstructor T>
	const T& SmartWrapper<T>::getPrimordialReference() const {
		return this->object;
	}
} // fireflower

#endif //SMARTOBJECT_SMARTOBJECT_H
