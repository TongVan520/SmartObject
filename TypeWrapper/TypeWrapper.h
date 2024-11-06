//
// Created by FireFlower on 2024/11/3.
//

#ifndef SMARTOBJECT_TYPEWRAPPER_H
#define SMARTOBJECT_TYPEWRAPPER_H

#include <concepts>
#include <utility>
#include <type_traits>

namespace fireflower {
	
	/// @名称 非类类型
	/// @描述 <code>T</code>不是一个类
	template<class T>
	concept NonclassType = not std::is_class_v<T>;
	
	/// @名称 不可继承的类类型
	/// @描述 <code>T</code>是一个不可继承的类
	template<class T>
	concept ClassTypeButFinal = not NonclassType<T> and std::is_final_v<T>;
	
	/// @类名 类型包装器的母模板
	/// @描述 可以包装任意可继承的类类型
	template<class T>
	class TypeWrapper : public T {
	public:
		using T::T;
	};
	
	/// @名称 类型包装器的偏特化
	/// @描述 可以包装任意<b>非类</b>类型
	template<NonclassType T>
	class TypeWrapper<T> {
	private:
		T value;
	
	public:
		template<class... Args>
		TypeWrapper(Args&& ... args) : value(std::forward<Args>(args)...) {
		
		}
		
		operator T&() {
			return this->value;
		}
		
		operator const T&() const {
			return this->value;
		}
	};
	
	/// @名称 类型包装器的偏特化
	/// @描述 可以包装任意<b>不可继承的</b>类类型
	template<ClassTypeButFinal T>
	class TypeWrapper<T> final {
	private:
		T value;
	
	public:
		template<class... Args>
		TypeWrapper(Args&& ... args) : value(std::forward<Args>(args)...) {
		
		}
		
		T& unwrap() {
			return this->value;
		}
		
		const T& unwrap() const {
			return this->value;
		}
	};
	
}

#endif //SMARTOBJECT_TYPEWRAPPER_H
