#include <iostream>
#include "SmartObject/SmartPointer.h"

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

#define MyException(errorStr) runtime_error( \
    (string)"日期 / 时间：" + __DATE__ + " / " + __TIME__ + \
    "\n文件：" + __FILE__ + "\t行：" + to_string(__LINE__) + \
    "\n函数：" + __FUNCTION__ + "\n错误信息：" + errorStr \
)

int main() {
	cout << "Hello World!" << endl;

//	testNormalPointer();
//	testEmptyPointer();
//	testNormalStar();
//	testEmptyStar();

//	testNormalChange();
//	testEmptyChange();

//	throw runtime_error("Test Exception");
	throw MyException("Test Exception");
	
	return 0;
}
