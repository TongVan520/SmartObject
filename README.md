# SmartObject 智能对象库

可以解决的问题：

|#|问题|
|-:|-|
|1||

## 智能对象`SmartObject`

一个`SmartPointer`类的辅助类。

## 智能指针`SmartPointer`

可以指向任何由[智能对象](#智能对象smartobject)类的实例的智能指针。
它不同于标准库中的`std::unique_ptr`和`std::shared_ptr`，与`std::weak_ptr`有些许相似，下面我们来看看它们的区别：  

|指针|指针类型示例|可指向的数据|是否影响数据`生命周期`|
|-|:-:|:-:|:-:|
|原始指针|`T*`|`栈`和`堆`|否|
|唯一指针|`std::unique_ptr<T>`|`堆`|是|
|共享指针|`std::shared_ptr<T>`|`堆`|是|
|弱指针|`std::weak_ptr<T>`|`堆`|否|
|`SmartPointer`指针|`SmartPointer<T>`|`栈`和`堆`|否|

### `std::unique_ptr`

只能指向存储在**堆**内存上的数据。  
`std::unique_ptr`所指向的数据的`生命周期`是由指针本身来控制的，当指针被销毁时，数据也会被销毁。
有点像`Rust`中的**所有权**规则。

### `std::shared_ptr`

只能指向存储在**堆**内存上的数据。  
`std::shared_ptr`所指向的数据的`生命周期`是由`引用计数`来控制的，当`引用计数`为0时，数据才会被销毁。

### `std::weak_ptr`

只能指向存储在**堆**内存上的数据。  
`std::weak_ptr`**不会**影响被指向的数据的`生命周期`，如同`原始指针`那般自由。只不过`std::weak_ptr`会提供`expired`接口供程序员判断该指针所指向的数据是否已失效（**包括**`野指针`）。  
这是`原始指针`做不到的地方，它无法判断自己是否为`野指针`。  
其原理是根据`引用计数`是否为0来判断被指向的数据是否已被释放。因此指向**有效**数据的`std::weak_ptr`一定只能通过`std::shared_ptr`来创建。

### `SmartPointer`

既能指向存储在**栈**内存上的数据，也能指向存储在**堆**内存上的数据。  
`SmartPointer`**不会**影响被指向的数据的`生命周期`，如同`原始指针`那般自由。只不过`SmartPointer`会提供`isNull`接口供程序员判断该指针所指向的数据是否已失效（**包括**`野指针`）。  
这是与`std::weak_ptr`的相似之处。  
虽然`SmartPointer`可以指向存储在**堆**内存上的数据，但它**不会**影响被指向的数据的`生命周期`，因此它**不会**自动销毁被指向的数据。  

## 线程安全

考虑以下示例代码：

```cpp
class Student : public SmartObject {
private:
	string nameStr;
	size_t age = 0;

public:
	virtual ~Student() {
		cout << this << "::" << __FUNCTION__ << endl;
	}
	
	const string& getName() const {
		return nameStr;
	}
	
	void setName(const string& nameStr) {
		this->nameStr = nameStr;
	}
	
	size_t getAge() const {
		return age;
	}
	
	void setAge(size_t age) {
		this->age = age;
	}
};

void mainForThread(SmartPointer<Student> p_student) {
	cout << "子线程id:" << this_thread::get_id() << endl;
	
	while (p_student) {
		// ==========================↓在以下区域delete则抛异常↓==========================
		cout << "======子线程======" << endl;
		// 由于Student类线程不安全，所以无法保证其对象操作的原子性
		// cout << "姓名：" << p_student->getName() << endl;
		// cout << "年龄：" << p_student->getAge() << endl;
		
		// 有概率抛异常，情况为：子线程在while判断后，打印student信息前，主线程delete
		try {
			cout << "姓名：" << (*p_student).getName() << endl;
			cout << "年龄：" << (*p_student).getAge() << endl;
			// ======================↑在以上区域delete则抛异常↑==========================
		}
		catch (const exception& excp) {
			cerr << excp.what() << endl;
		}
	}
	
	cout << "内存已释放..." << endl;
}

void main() {
	cout << "多线程测试开始" << endl;
	cout << "主线程id:" << this_thread::get_id() << endl;
	
	Student* p_student = new Student;
	p_student->setName("田所浩二");
	p_student->setAge(24);
	
	thread thrd(mainForThread, SmartPointer<Student>(p_student));
	
	this_thread::sleep_for(chrono::milliseconds(1000));
	
	delete p_student;
	thrd.join();
	cout << "多线程测试结束" << endl;
}
```

以下是运行两次的输出：  

第一次

```python
多线程测试开始
主线程id:1
子线程id:2
======子线程======
姓名：田所浩二
年龄：24

(此处省略重复输出的内容...)

======子线程======
姓名：田所浩二
年龄：24
======子线程======
姓名：0x1001d00::~Student田所浩二

年龄：24
======子线程======
姓名：田所浩二
年龄：内存已释放...
多线程测试结束
日期 / 时间：Oct 23 2024 / 19:00:19
文件：D:/Application Projects/CLion/SmartObject/SmartObject/SmartPointer.h	行：122
函数：operator*
错误信息：指针无效...
```

第二次

```python
多线程测试开始
主线程id:1
子线程id:2
======子线程======
姓名：田所浩二
年龄：24

(此处省略重复输出的内容...)

======子线程======
姓名：田所浩二
年龄：24
======子线程======
姓名：田所浩二
0x10e1d00年龄：::24~Student
======子线程======
姓名：田所浩二

年龄：24
内存已释放...
多线程测试结束
```

## 实现思路

`原始指针`最大的痛点在于：
>`原始指针`无法判断自己是否为`野指针`。

而造成`野指针`的原因只有一个：
>指针所指向的数据已被释放，但指针本身并不知情。

因此我们可以在数据的`析构函数`上做手脚，在自己被销毁时**通知**所有指向它的指针。  

熟悉`Qt`的开发者们就知道这种功能只需要一个`信号`就可以解决。

### 性能

要实现保障了功能的最小性能开销，就得手动实现一个简单的`信号槽`功能，也许都称不上`信号槽`，只能算一些简单的`回调`功能。  
但为了保证线程安全，还是选择了现成的[sigslot](https://github.com/palacaze/sigslot)信号槽库。

### 线程安全

由于`sigslot`提供的功能是线程安全的，因此只需要保证`SmartPointer`中修改原始指针的代码是线程安全的即可。  

只需用一个`互斥锁`保护`原始指针`即可。
考虑到普通的`互斥锁`会在一个线程读取时会导致需要读取它的其他线程发生阻塞，因此可以将`互斥锁`替换为`读写锁`。

## 待解决问题

|##|问题|描述|
|:-:|:-:|:-|
|1|基本类型|目前为止`SmartPointer`仅支持继承自`SmartObject`的类。由于`SmartPointer`的功能依赖于`SmartObject`，可以考虑引入一个继承自`SmartObject`的模板类。|
