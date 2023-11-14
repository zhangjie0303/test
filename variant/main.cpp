#include <iostream>
#include <list>
#include <variant>
//#include <any>

struct Student {
	int id;
	std::string name;
};

struct Teacher {
	int id;
	std::string school;
};


using VARIANT = std::variant<int, float, double, std::string, Student, Teacher* >;
//using VARIANT = std::variant<int, float, double, char>;

int main() {
    std::list<VARIANT> variantList;
    variantList.push_back(10);
    variantList.push_back(3.14f);
    variantList.push_back(3.14159);
    variantList.push_back("abcd");
    //variantList.push_back(uint64_t(1888)); // build error!
    //variantList.push_back('a'); // print 97
	
	Student stu;
	stu.id = 1;
	stu.name = "wang";
	variantList.push_back(stu);

	Teacher teacher;
	teacher.id = 888;
	teacher.school = "beijindaxue";
	variantList.push_back(&teacher);

	for (auto it = variantList.begin(); it != variantList.end(); ++it) {
		VARIANT* ptr = &(*it);
		if(const auto intPtr (std::get_if<int>(ptr)); intPtr) {
			std::cout << "type is int, value: " << *intPtr << std::endl;
		} else if (const auto floatPtr (std::get_if<float>(ptr)); floatPtr) {
			std::cout << "type is float, value: " << *floatPtr << std::endl;
		} else if (const auto doublePtr (std::get_if<double>(ptr)); doublePtr) {
			std::cout << "type is double, value: " << *doublePtr << std::endl;
		} else if (auto* stringPtr = std::get_if<std::string>(ptr); stringPtr) {
			std::cout << "type is string, value: " << *stringPtr << std::endl;
		//} else if (auto* stuPtr = std::get_if<Student>(ptr); stuPtr) {
		} else if (auto stuPtr = std::get_if<Student>(ptr); stuPtr) {
			std::cout << "type is student, value: " << stuPtr->name << std::endl;
		//} else if (auto* teacherPtr = std::get_if<Teacher*>(ptr); teacherPtr) {
		} else if (auto teacherPtr = std::get_if<Teacher*>(ptr); teacherPtr) {
			std::cout << "type is teacher, value: " << (*teacherPtr)->school << std::endl;
		} else {
			std::cout << "unknow type!" << std::endl;
		}
	}

    // std::list<std::any> anyList;
    // anyList.push_back(10);
    // anyList.push_back(3.14f);
    // anyList.push_back(3.14159);
    // anyList.push_back('a');

    return 0;
}
