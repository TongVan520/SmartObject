//
// Created by FireFlower on 2024/10/18.
//

#ifndef SMARTOBJECT_SMARTPOINTER_H
#define SMARTOBJECT_SMARTPOINTER_H

#include <exception>
#include "SmartObject.h"
#include <shared_mutex>
#include "SmartDefine.h"

namespace fireflower {
	///智能指针
	template<BaseOfSmartObject T>
	class SmartPointer {
	private:
		/// @名称 对象的原始指针
		T* p_object = nullptr;
		
		/// @名称 读写锁
		/// @描述 用于<b>高效</b>保障多线程中原始指针的状态
		mutable std::shared_mutex shrd_mtx;
	
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
		
		virtual SmartPointer<T>& operator=(const SmartPointer<T>& other);
		
		virtual ~SmartPointer() = default;
		
		inline T& operator*() const;
		
		inline T* operator->() const;
		
		/// @名称 获取原始指针
		/// @描述 线程安全，原子操作
		/// @返回值 原始指针
		[[nodiscard]] inline T* getPrimordialPointer() const;
		
		/// @名称 是否为空
		/// @描述 线程安全，原子操作
		/// @返回值 若所指向的对象已失效，则返回<code>true</code>，否则返回<code>false</code>
		[[nodiscard]] inline bool isNull() const;
		
		inline explicit operator bool() const;
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
	SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer<T>& other) {
		if (this->getPrimordialPointer() == other.getPrimordialPointer()) {
			return *this;
		}
		
		this->unbindObject();
		if (std::shared_lock<std::shared_mutex> other_read_locker(other.shrd_mtx); other) {
			this->bindObject(*other.p_object);
		}
		return *this;
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::bindObject(T& object) {
		std::unique_lock<std::shared_mutex> write_locker(this->shrd_mtx);
		this->p_object = &object;
		this->p_object->destructed.connect(&SmartPointer<T>::onDestructed, this);
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::unbindObject() {
		std::unique_lock<std::shared_mutex> write_locker(this->shrd_mtx);
		if (not this->p_object) {
			return;
		}
		this->p_object->destructed.disconnect(&SmartPointer<T>::onDestructed, this);
		this->p_object = nullptr;
	}
	
	template<BaseOfSmartObject T>
	T& SmartPointer<T>::operator*() const {
		//保证操作的原子性
		std::shared_lock<std::shared_mutex> read_locker(this->shrd_mtx);
		this->p_object != nullptr ?: throw SmartException("指针无效...");
		return *(this->p_object);
	}
	
	template<BaseOfSmartObject T>
	T* SmartPointer<T>::operator->() const {
		std::shared_lock<std::shared_mutex> read_locker(this->shrd_mtx);
		return this->p_object;
	}
	
	template<BaseOfSmartObject T>
	T* SmartPointer<T>::getPrimordialPointer() const {
		std::shared_lock<std::shared_mutex> read_locker(this->shrd_mtx);
		return this->p_object;
	}
	
	template<BaseOfSmartObject T>
	bool SmartPointer<T>::isNull() const {
		std::shared_lock<std::shared_mutex> read_locker(this->shrd_mtx);
		return not this->p_object;
	}
	
	template<BaseOfSmartObject T>
	SmartPointer<T>::operator bool() const {
		std::shared_lock<std::shared_mutex> read_locker(this->shrd_mtx);
		return this->p_object;
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::onDestructed() {
		std::unique_lock<std::shared_mutex> write_locker(this->shrd_mtx);
		this->p_object = nullptr;
	}
	
	template<BaseOfSmartObject T>
	SmartPointer<T> makeSmart(T* object) {
		return SmartPointer<T>(object);
	}

#define make_smart makeSmart

}

#endif //SMARTOBJECT_SMARTPOINTER_H
