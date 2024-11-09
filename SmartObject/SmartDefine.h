//
// Created by FireFlower on 2024/10/23.
//

#ifndef SMARTOBJECT_SMARTDEFINE_H
#define SMARTOBJECT_SMARTDEFINE_H

#include <type_traits>

namespace fireflower {
	class SmartObject;
	
	/// @名称 继承自<code>SmartObject</code>
	template<class T>
	concept BaseOfSmartObject = std::is_base_of_v<SmartObject, T>;
	
	/// @名称 拥有构造函数
	template<class T>
	concept HasConstructor = std::is_constructible_v<T>;
	
	// /// @名称 拥有复制构造函数
	// template<class T>
	// concept HasCopyConstructor = requires() {
	// 	std::is_copy_constructible_v<T>;
	// };
	//
	// /// @名称 拥有移动构造函数
	// template<class T>
	// concept HasMoveConstructor = requires() {
	// 	std::is_move_constructible_v<T>;
	// };
	
	/// @名称 可与指定类型赋值
	template<class T, class U>
	concept CanAssignWith = std::is_assignable_v<T, U>;
	
	/// @名称 可复制赋值
	template<class T>
	concept CanCopyAssign = std::is_copy_assignable_v<T>;
	
	/// @名称 可移动赋值
	template<class T>
	concept CanMoveAssign = std::is_move_assignable_v<T>;

#define SmartException(errorStr) std::runtime_error( \
    (std::string)"日期 / 时间：" + __DATE__ + " / " + __TIME__ + \
    "\n文件：" + __FILE__ + "\t行：" + std::to_string(__LINE__) + \
    "\n函数：" + __FUNCTION__ + "\n错误信息：" + errorStr \
)
}

#endif //SMARTOBJECT_SMARTDEFINE_H
