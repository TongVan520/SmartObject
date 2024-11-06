#include <iostream>
#include "SmartObject/SmartPointer.h"
#include <thread>
#include <memory>
#include "TypeWrapper/TypeWrapper.h"

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
			// 类类型
			SmartWrapper<string> textStrWrp = "114514";
			textStrWrp.append("1919810");
			cout << textStrWrp << endl;        // 1145141919810
			
			// 非类类型
			SmartWrapper<size_t> size = textStrWrp.size();
			cout << "size == " << size << endl;
			
			for (auto& c: textStrWrp) {
				c++;
			}
			cout << textStrWrp << endl;        // 2256252:2:921
			
			// SmartWrapper<Student> studentWrp;	// 报错，原因：不能包装继承自`SmartObject`类的类型
		}
		
		void testWrapperWithPointer() {
			SmartWrapper<string> textStrWrp = "114514";
			auto textStrPtr = make_smart(&textStrWrp);
			textStrPtr->append("1919810");
			cout << *textStrPtr << endl;
		}
		
		void testWrapperWithEmptyPointer() {
			using namespace fireflower;
			SmartPointer<SmartWrapper<string>> p;
			
			{
				SmartWrapper<string> textStrWrp = "114514";
				
				p = make_smart(&textStrWrp);
				p->append("1919810");
				
				cout << "*p == " << *p << endl;
			}
			
			if (p) {
				cout << "*p == " << *p << endl;
			}
			else {
				cout << "内存已释放..." << endl;
			}
		}
	}
}

void showSize() {
	cout << "sizeof(sigslot::signal<>) == " << sizeof(sigslot::signal<>) << endl;
	cout << endl;
	cout << "sizeof(SmartObject) == " << sizeof(SmartObject) << endl;
	cout << endl;
	cout << "sizeof(string) == " << sizeof(string) << endl;
	cout << "sizeof(SmartWrapper<string>) == " << sizeof(SmartWrapper<string>) << endl;
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
	
	using namespace wrapper;
	// testNormalWrapper();
	// testWrapperWithPointer();
	// testWrapperWithEmptyPointer();
	
	// showSize();
	
	return 0;
}
