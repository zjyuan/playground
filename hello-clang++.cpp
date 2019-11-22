#include <iostream>
#include <memory>

int main()
{
	std::cout << "hello, clang++" << std::endl;
	auto ptr = std::make_unique<int>(9);
	std::cout << "value stored in ptr: " << *ptr << std::endl;
}

// clang++ hello-clang++.cpp -o hello-clang++