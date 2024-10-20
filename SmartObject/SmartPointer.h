//
// Created by FireFlower on 2024/10/18.
//

#ifndef SMARTOBJECT_SMARTPOINTER_H
#define SMARTOBJECT_SMARTPOINTER_H

#include <type_traits>
#include <exception>
#include "SmartObject.h"

namespace fireflower {
	///智能指针
	template<BaseOfSmartObject T>
	class SmartPointer {
	private:
		T* p_object = nullptr;
	
	private:
		virtual void onDestructed();
		
		void bindObject(SmartObject& object);
		
		void unbindObject(SmartObject& object);
	
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
	SmartPointer<T>::SmartPointer(const SmartPointer& other) : SmartPointer(other.p_object) {
	
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::unbindObject(SmartObject& object) {
		object.destructed.disconnect(&SmartPointer<T>::onDestructed, this);
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::bindObject(SmartObject& object) {
		object.destructed.connect(&SmartPointer<T>::onDestructed, this);
	}
	
	template<BaseOfSmartObject T>
	SmartPointer<T>& SmartPointer<T>::operator=(T* new_p_object) {
		if (not this->isNull()) {
			this->unbindObject(*this->p_object);
		}
		
		this->p_object = new_p_object;
		if (not this->isNull()) {
			this->bindObject(*this->p_object);
		}
		
		return *this;
	}
	
	template<BaseOfSmartObject T>
	SmartPointer<T>::SmartPointer(T* init_p_object) : p_object(init_p_object) {
		if (not this->isNull()) {
			this->bindObject(*this->p_object);
		}
	}
	
	template<BaseOfSmartObject T>
	T* SmartPointer<T>::operator->() const {
		return this->p_object;
	}
	
	template<BaseOfSmartObject T>
	T& SmartPointer<T>::operator*() const {
		not this->isNull() ?: throw std::runtime_error("指针无效...");
		return *(this->p_object);
	}
	
	template<BaseOfSmartObject T>
	bool SmartPointer<T>::isNull() const {
		return not this->p_object;
	}
	
	template<BaseOfSmartObject T>
	void SmartPointer<T>::onDestructed() {
		this->p_object = nullptr;
	}
}

#endif //SMARTOBJECT_SMARTPOINTER_H
