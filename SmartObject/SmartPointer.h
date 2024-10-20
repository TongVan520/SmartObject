//
// Created by FireFlower on 2024/10/18.
//

#ifndef SMARTOBJECT_SMARTPOINTER_H
#define SMARTOBJECT_SMARTPOINTER_H

#include <type_traits>
#include <exception>
#include "SmartObject.h"
#include <mutex>

namespace fireflower {

#define SmartException(errorStr) std::runtime_error( \
    (std::string)"日期 / 时间：" + __DATE__ + " / " + __TIME__ + \
    "\n文件：" + __FILE__ + "\t行：" + std::to_string(__LINE__) + \
    "\n函数：" + __FUNCTION__ + "\n错误信息：" + errorStr \
)
	
	///智能指针
	template<BaseOfSmartObject T>
	class SmartPointer {
	private:
		/// @名称 对象的原始指针
		T* p_object = nullptr;
		
		/// @名称 互斥锁
		/// @描述 用于保障多线程中原始指针的状态
		mutable std::mutex mtx;
	
	private:
		virtual void onDestructed();
		
		/// @名称 绑定对象
		/// @描述 线程安全，原子操作
		/// @参数名称 object
		/// @参数描述 要绑定的对象
		void bindObject(T& object);
		
		/// @名称 解绑对象
		/// @描述 线程安全，原子操作\n
		/// <b>注意：若对象无效则什么也不做</b>
		void unbindObject();
	
	public:
		SmartPointer(T* init_p_object = nullptr);
		
		SmartPointer(const SmartPointer& other);
		
		SmartPointer(SmartPointer&& other) = default;
		
		virtual ~SmartPointer() = default;
		
		virtual SmartPointer<T>& operator=(T* new_p_object);
		
		[[nodiscard]] inline bool isNull() const;
		
		inline T& operator*() const;
		
		inline T* operator->() const;
	};
	
	template<BaseOfSmartObject T>
	SmartPointer<T>::SmartPointer(T* init_p_object) {
		if (init_p_object) {
			this->bindObject(*init_p_object);
		}
	}
	
	template<BaseOfSmartObject T>
	SmartPointer<T>::SmartPointer(const SmartPointer& other) : SmartPointer(other.p_object) {
	
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::bindObject(T& object) {
		std::lock_guard<std::mutex> locker(this->mtx);
		this->p_object = &object;
		this->p_object->destructed.connect(&SmartPointer<T>::onDestructed, this);
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::unbindObject() {
		std::lock_guard<std::mutex> locker(this->mtx);
		if (not this->p_object) {
			return;
		}
		this->p_object->destructed.disconnect(&SmartPointer<T>::onDestructed, this);
		this->p_object = nullptr;
	}
	
	template<BaseOfSmartObject T>
	SmartPointer<T>& SmartPointer<T>::operator=(T* new_p_object) {
		this->unbindObject();
		if (new_p_object) {
			this->bindObject(*new_p_object);
		}
		return *this;
	}
	
	template<BaseOfSmartObject T>
	T* SmartPointer<T>::operator->() const {
		std::lock_guard<std::mutex> locker(this->mtx);
		return this->p_object;
	}
	
	template<BaseOfSmartObject T>
	T& SmartPointer<T>::operator*() const {
		//保证操作的原子性
		std::lock_guard<std::mutex> locker(this->mtx);
		this->p_object != nullptr ?: throw SmartException("指针无效...");
		return *(this->p_object);
	}
	
	template<BaseOfSmartObject T>
	bool SmartPointer<T>::isNull() const {
		std::lock_guard<std::mutex> locker(this->mtx);
		return not this->p_object;
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::onDestructed() {
		std::lock_guard<std::mutex> locker(this->mtx);
		this->p_object = nullptr;
	}
}

#endif //SMARTOBJECT_SMARTPOINTER_H
