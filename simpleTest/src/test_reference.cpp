#include <stdio.h>
#include <stdlib.h>
#include <string>

template<class T>
class sharedPtr
{
public:
	sharedPtr(T t) : p(NULL) {
		if(p == NULL) {
			try	{
				p = new T(t);
			}
			catch(std::bad_alloc e) {
				printf("%s\n", e.what());
			}
		}
	};

	~sharedPtr() {
		if(p) delete p;
	};

	inline T* get() {
		return p;
	};

private:
	T* p;
};

void test(sharedPtr<int> &ref)
{
	puts("\ntest");
	sharedPtr<int> &_r = ref;
	// sharedPtr<int> _r = ref;		이거하면 주금
	printf("ref: %d\n", *(_r.get()));
	printf("ref: %p\n", &_r);
}

int main() {

	int *b = NULL;
	try {
		b = new int(1);
	}
	catch(std::bad_alloc e) {
		printf("%s\n", e.what());
	}

	int* &c = b;

	printf("b: %d\n", *b);
	printf("c: %d\n", *c);
	printf("b: %p\n", b);
	printf("c: %p\n", c);
	*c = 2;
	printf("c = 2\n");
	printf("b: %d\n", *b);
	printf("c: %d\n", *c);
	printf("b: %p\n", b);
	printf("c: %p\n", c);

	delete b;
	b = NULL;
	// printf("b: %d\n", *b);	이거하면 주금
	// printf("c: %d\n", *c);	이거하면 주금
	printf("b: %p\n", b);
	printf("c: %p\n", c);
	// delete c;	이거하면 주금

	sharedPtr<int> _p(10);
	sharedPtr<int> &_r = _p;
	printf("p: %d\n", *(_p.get()));
	printf("r: %d\n", *(_r.get()));
	printf("p: %p\n", &_p);
	printf("r: %p\n", &_r);

	*(_r.get()) = 20;
	printf("r = 20\n");
	printf("p: %d\n", *(_p.get()));
	printf("r: %d\n", *(_r.get()));
	printf("p: %p\n", &_p);
	printf("r: %p\n", &_r);
	
	test(_r);
	printf("p: %d\n", *(_p.get()));
	printf("r: %d\n", *(_r.get()));
	printf("p: %p\n", &_p);
	printf("r: %p\n", &_r);

	return 0;
}
