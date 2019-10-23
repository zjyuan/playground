#include <iostream>
#include <memory>

class Sample
{
private:
	class PImpl;
	std::shared_ptr<PImpl> impl_;
public:
	Sample():impl_(std::make_shared<PImpl>()){}
	void Print();
};

class Sample::PImpl
{
public:
	PImpl() = default;
	~PImpl(){ std::cout << "PImpl dtor" << std::endl; }
	inline void Print() { std::cout << "Sample::PImpl:Print" << std::endl; }
};

void Sample::Print()
{
	impl_->Print();
}

int main()
{
	Sample s;
	s.Print();
}

//g++ bridge_pattern.cpp -o bridge_pattern -std=c++11