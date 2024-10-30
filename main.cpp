#include <iostream>
#include "SmartObject/SmartPointer.h"
#include <thread>

using namespace std;
using namespace fireflower;

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

namespace test {
	void testNormalPointer() {
		Student student;
		student.setName("田所浩二");
		student.setAge(24);
		
		SmartPointer<Student> p = &student;
		cout << "姓名：" << p->getName() << endl;
		cout << "年龄：" << p->getAge() << endl;
	}
	
	void testEmptyPointer() {
		SmartPointer<Student> p;
		
		{
			Student student;
			student.setName("田所浩二");
			student.setAge(24);
			
			p = &student;
			cout << "姓名：" << p->getName() << endl;
			cout << "年龄：" << p->getAge() << endl;
		}
		
		if (not p.isNull()) {
			cout << "姓名：" << p->getName() << endl;
			cout << "年龄：" << p->getAge() << endl;
		}
		else {
			cout << "内存已释放..." << endl;
		}
	}
	
	void testNormalStar() {
		Student student;
		student.setName("田所浩二");
		student.setAge(24);
		
		SmartPointer<Student> p = &student;
		
		cout << "姓名：" << (*p).getName() << endl;
		cout << "年龄：" << (*p).getAge() << endl;
	}
	
	void testEmptyStar() {
		SmartPointer<Student> p;
		
		{
			Student student;
			student.setName("田所浩二");
			student.setAge(24);
			
			p = &student;
			cout << "姓名：" << (*p).getName() << endl;
			cout << "年龄：" << (*p).getAge() << endl;
		}
		
		if (not p.isNull()) {
			cout << "姓名：" << (*p).getName() << endl;
			cout << "年龄：" << (*p).getAge() << endl;
		}
		else {
			cout << "内存已释放..." << endl;
		}
		
		try {
			cout << "姓名：" << (*p).getName() << endl;
			cout << "年龄：" << (*p).getAge() << endl;
		}
		catch (const exception& excp) {
			cerr << excp.what() << endl;
		}
	}
	
	void testNormalChange() {
		Student student;
		student.setName("田所浩二");
		student.setAge(24);
		
		SmartPointer<Student> p = &student;
		cout << "姓名：" << p->getName() << endl;
		cout << "年龄：" << p->getAge() << endl;
		
		cout << "======变身！======" << endl;
		
		Student senpai;
		senpai.setName("野兽先辈");
		senpai.setAge(114514);
		
		p = &senpai;
		cout << "姓名：" << p->getName() << endl;
		cout << "年龄：" << p->getAge() << endl;
	}
	
	void testEmptyChange() {
		SmartPointer<Student> p;
		
		{
			Student senpai;
			senpai.setName("野兽先辈");
			senpai.setAge(114514);
			
			{
				Student student;
				student.setName("田所浩二");
				student.setAge(24);
				
				p = &student;
				cout << "姓名：" << p->getName() << endl;
				cout << "年龄：" << p->getAge() << endl;
			}
			
			if (not p.isNull()) {
				cout << "姓名：" << p->getName() << endl;
				cout << "年龄：" << p->getAge() << endl;
			}
			else {
				cout << "内存已释放..." << endl;
			}
			
			cout << "======变身！======" << endl;
			
			p = &senpai;
			cout << "姓名：" << p->getName() << endl;
			cout << "年龄：" << p->getAge() << endl;
		}
		
		if (not p.isNull()) {
			cout << "姓名：" << p->getName() << endl;
			cout << "年龄：" << p->getAge() << endl;
		}
		else {
			cout << "内存已释放..." << endl;
		}
	}
	
	namespace multithread {
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
	}
	
	namespace wrapper {
		void testNormalWrapper() {
			using namespace fireflower;
			
			int normal_number = 114;
			SmartWrapper<int> number = 0;
			cout << number << endl;
			
			number = 10;
			normal_number = number;
			number = normal_number;
			
			for (SmartWrapper<int> i = 0; i < number; i++) {
				cout << i << endl;
			}
			
			string textStr = "114514";
			SmartWrapper<string> textStrWrp = "1919810";
			
			textStrWrp = textStr;
			cout << static_cast<string>(textStrWrp) << endl;
			// textStr = textStrWrp;
			// cout << textStr << endl;
			
			// 思维栈：智能包装器的实现 -> 隐式类型转换规则 -> 值类别
			
			class Test {
			public:
				int n;
				
			public:
				Test() = default;
				
				Test(const Test&) = default;
				
				Test(Test&&) = default;
				
				Test& operator=(const Test&) = default;
				
				Test& operator=(Test&&) = default;
			};
			
			SmartWrapper<Test> t1, t2;
			// t1 = std::move(t2);
			// t1.operator=(std::move(t2));
			t1 = std::move(t2);
			// t1.n;
			
			SmartWrapper<string> str = "114514";
			auto ptr = make_smart(str);
			SmartPointer<SmartWrapper<string>> pointer = &str;
			cout << pointer << endl;
			cout << str.getPrimordialReference() << endl;
		}
	}
}

void showSize() {
	cout << "sizeof(sigslot::signal<>) == " << sizeof(sigslot::signal<>) << endl;
	cout << endl;
	cout << "sizeof(SmartObject) == " << sizeof(SmartObject) << endl;
	cout << endl;
	cout << "sizeof(string) == " << sizeof(string) << endl;
	cout << "sizeof(size_t) == " << sizeof(size_t) << endl;
	cout << "sizeof(Student) == " << sizeof(Student) << endl;
	cout << endl;
	cout << "======指针大小比较======" << endl;
	cout << "sizeof(Student*) == " << sizeof(Student*) << endl;
	cout << "sizeof(unique_ptr<Student>) == " << sizeof(unique_ptr<Student>) << endl;
	cout << "sizeof(shared_ptr<Student>) == " << sizeof(shared_ptr<Student>) << endl;
	cout << "sizeof(weak_ptr<Student>) == " << sizeof(weak_ptr<Student>) << endl;
	cout << "sizeof(SmartPointer<Student>) == " << sizeof(SmartPointer<Student>) << endl;
}

int main() {
	cout << "Hello World!" << endl;
	
	using namespace test;
	// testNormalPointer();
	// testEmptyPointer();
	// testNormalStar();
	// testEmptyStar();
	//
	// testNormalChange();
	// testEmptyChange();
	
	// throw runtime_error("Test Exception");
	// throw SmartException("Test Exception");
	
	// multithread::main();
	
	wrapper::testNormalWrapper();
	
	// showSize();
	
	return 0;
}
