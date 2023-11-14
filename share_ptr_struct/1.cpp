#include <iostream>
#include <memory>
#include <vector>

struct User {
    int user_id;
    std::string user_name;

    User(int id, std::string name) : user_id(id), user_name(name) {}
};

std::shared_ptr<User> g_user = std::make_shared<User>(9, "Zhanglang");

//void print_user(std::shared_ptr<User>& user) {
//void print_user(const std::shared_ptr<User>& user) {
void print_user(std::shared_ptr<User> user) {
	std::cout << __FUNCTION__ << " counter: " << user.use_count() << ", name: " << user->user_name << std::endl;
}

void print_user_use_ptr(std::shared_ptr<User>* user_ptr) {
	std::cout << __FUNCTION__ << " counter: " << user_ptr->use_count() << ", name: " << (*user_ptr)->user_name << std::endl;
}

//std::shared_ptr<User>& create_user() {
std::shared_ptr<User> create_user() {
	return g_user;
}

int main() {
	// 创建一个std::vector，存放std::shared_ptr<User>
	std::vector<std::shared_ptr<User>> users;
	users.resize(3);

	// 向std::vector中添加一些User
	//users.push_back(std::make_shared<User>(1, "Alice"));
	//users.push_back(std::make_shared<User>(2, "Bob"));
	//users.push_back(std::make_shared<User>(3, "Charlie"));
	users[0] = std::make_shared<User>(1, "Alice");
	users[1] = std::make_shared<User>(2, "Bob");
	users[2] = std::make_shared<User>(3, "Charlie");

	// 遍历std::vector中的User
	// for (const auto& user : users) {
	// 	std::cout << "User ID: " << user->user_id << ", User Name: " << user->user_name << std::endl;
	// }

	//auto user = std::make_shared<User>(8, "Zhangjie");
	//auto user = create_user();
	//std::cout << __FUNCTION__ << " counter: " << user.use_count() << ", name: " << user->user_name << std::endl;
	//print_user(user);
	//print_user(create_user());
	print_user_use_ptr(&create_user());
	//print_user_use_ptr(&user);
	//std::cout << __FUNCTION__ << " counter: " << user.use_count() << ", name: " << user->user_name << std::endl;

	return 0;
}
