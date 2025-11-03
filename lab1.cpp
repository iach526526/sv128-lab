#include <iostream>
#include <sv128/sv128.h>	 // Make sure this header is found via -I./include
#include <sv128/sv_logger.h> // Make sure this header is found via -I./include
using namespace std;
// #define all_t sv_mask_all_true()
void vector_add(float *result, const float *a, const float *b, int size);
void vector_mul(float *result, const float *a, const float *b, int size);
void vector_saxpy(float *y, const float *x, float alpha, int size);

void print_content(const float *array)
{
	int MAX_INDEX = sizeof(array)-1; // +1 to let length correct(start from 0)
	for (int i = 0; i < MAX_INDEX; i++)
	{
		std::cout << array[i] << " ";
	}
	std::cout << "\n";
}

// float array[4] = {1.5f, 2.5f, 3.5f, 4.5f};
// sv_float4 passthru = sv_set_float(10.0f, 20.0f, 30.0f, 40.0f);
// sv_mask mask = sv_init_ones(3);  // [T, T, T, F]
// sv_float4 vec = sv_load_float(passthru, array, mask);  // [1.5, 2.5, 3.5, 40.0]
void vector_add(float *result, const float *a, const float *b, int size)
{
	sv_float4 passthru = sv_set_float(10.0f, 20.0f, 30.0f, 40.0f);
	int i = 0;

	for (; i + 3 < size; i += 4)
	{
		sv_float4 va = sv_load_float(passthru, a + i, sv_mask_all_true());
		sv_float4 vb = sv_load_float(passthru, b + i, sv_mask_all_true());
		sv_float4 vr = sv_float_add(va, vb, sv_mask_all_true());
		sv_store_float(result + i, vr, sv_mask_all_true());
	}
	// count how many elements have not operated yet
	
	if(size %4 ==0) return;// all elements have been processed
	int remaining = size - i;
	sv_mask tail_mask = sv_init_ones(remaining);// remaining = 1..3 : [T,F,F,F] / [T,T,F,F] / [T,T,T,F]
	cout<<tail_mask<<"\n";
	// sv_mask tail_mask = sv_mask_not(temp_mask);// reverse to get [F,F,F..F,T,T] T count is size%4
	auto tail_a = sv_load_float(passthru, a + i, tail_mask);
	auto tail_b = sv_load_float(passthru, b + i, tail_mask);
	sv_float4 temp = sv_float_add(tail_a, tail_b, tail_mask);
	// std::cout<<"tail a:"<<tail_a<<"\n"<<"tail b:"<<tail_b<<"\n"<<temp<<"\n";
	sv_store_float(result + i, temp, tail_mask);
	return;
}

void vector_mul(float *result, const float *a, const float *b, int size)
{

	sv_float4 passthru = sv_set_float(10.0f, 20.0f, 30.0f, 40.0f);
	int i = 0;

	for (; i + 3 < size; i += 4)
	{
		sv_float4 va = sv_load_float(passthru, a + i, sv_mask_all_true());
		sv_float4 vb = sv_load_float(passthru, b + i, sv_mask_all_true());
		sv_float4 vr = sv_float_mul(va, vb, sv_mask_all_true());
		sv_store_float(result + i, vr, sv_mask_all_true());
	}
	// count how many elements have not operated yet
	
	if(size %4 ==0) return;// all elements have been processed
	int remaining = size - i;
	sv_mask tail_mask = sv_init_ones(remaining);// remaining = 1..3 : [T,F,F,F] / [T,T,F,F] / [T,T,T,F]
	cout<<tail_mask<<"\n";
	// sv_mask tail_mask = sv_mask_not(temp_mask);// reverse to get [F,F,F..F,T,T] T count is size%4
	auto tail_a = sv_load_float(passthru, a + i, tail_mask);
	auto tail_b = sv_load_float(passthru, b + i, tail_mask);
	sv_float4 temp = sv_float_mul(tail_a, tail_b, tail_mask);
	// std::cout<<"tail a:"<<tail_a<<"\n"<<"tail b:"<<tail_b<<"\n"<<temp<<"\n";
	sv_store_float(result + i, temp, tail_mask);
	return;
}

// y[i] = alpha * x[i] + y[i]
void vector_saxpy(float *y, const float *x, float alpha, int size)
{
	return;
}

// main function for testing. Delete this when completed function development.
int main()
{
	const int size = 7;
	cout<<"process started\n"<<"size is "<<size<<"\n";
	float a[size] = { 2, 3, 4, 5, 6, 7,  10};
	float b[size] = { 2, 2, 2, 2, 2, 2, 11};
	float c[size];

	// vector_add(c, a, b, size);
	// print_content(c);
	vector_mul(c, a, b, size);
	print_content(c);
	return 0;
}
