#include "SharedPtr.hpp"

struct Yoink {
	Yoink(SharedPtr<int> ptr): p(ptr) {}
	~Yoink() {}
	SharedPtr<int> p;
};

int main() {
	SharedPtr<int> p;

	Yoink copy1(p);
	SharedPtr<int> copy2(p);
	SharedPtr<int> copy3 = copy2;
}
