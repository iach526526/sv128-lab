
// main function for testing. Delete this when completed function development.
#include <iostream>
using namespace std;
// ===== Functions to be implemented by students (in lab1.cpp) =====
extern void vector_add(float* result, const float* a, const float* b, int size);
extern void vector_mul(float* result, const float* a, const float* b, int size);
extern void vector_saxpy(float* y, const float* x, float alpha, int size);

void print_content(const float *array)
{
	int MAX_INDEX = sizeof(array)-1; // +1 to let length correct(start from 0)
	for (int i = 0; i < MAX_INDEX; i++)
	{
		std::cout << array[i] << " ";
	}
	std::cout << "\n";
}

int main()
{
	const int size = 7;
	cout<<"process started\n"<<"size is "<<size<<"\n";
	float a[size] = { 2, 3, 4, 5, 6, 7,  10};
	float b[size] = { 2, 4, 3, 1, 3, 2, 11};
	float c[size];

	cout<<"x=";
	print_content(b);
	cout<<"y=";
	print_content(a);
	//call calculator function
	// vector_add(c, a, b, size);
	// print_content(c);
	vector_saxpy(a, b,2.0 ,0);
	print_content(a);
	print_content(b);
	return 0;
}
