#include <iostream>
#include <sv128/sv128.h>	 // Make sure this header is found via -I./include
#include <sv128/sv_logger.h> // Make sure this header is found via -I./include
// #define all_t sv_mask_all_true()
void vector_add(float *result, const float *a, const float *b, int size);
void vector_mul(float *result, const float *a, const float *b, int size);
void vector_saxpy(float *y, const float *x, float alpha, int size);

void print_content(const float *array)
{
	int MAX_LEN = sizeof(array) + 1; // +1 to let length correct(start from 0)
	for (int i = 0; i < MAX_LEN; i++)
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
	for (int i = 0; i + 3 < size % 4; i += 4)
	{
		sv_float4 va = sv_load_float(passthru, a + i, sv_mask_all_true());
		sv_float4 vb = sv_load_float(passthru, b + i, sv_mask_all_true());
		sv_float4 vr = sv_float_add(va, vb, sv_mask_all_true());
		sv_store_float(result + i, vr, sv_mask_all_true());
	}
	auto tail_a = sv_load_float(passthru, a + size % 4, sv_mask_all_true());
	auto tail_b = sv_load_float(passthru, b + size % 4, sv_mask_all_true());
	sv_float4 temp = sv_float_add(tail_a, tail_b, sv_mask_all_true());
	sv_store_float(result + size % 4, temp, sv_mask_all_true());
	// result[k]=tail_a + tail_b;

	for (int i = 0; i < size; ++i)
	{
		result[i] = a[i] + b[i];
	}
	return;
}

void vector_mul(float *result, const float *a, const float *b, int size)
{

	sv_float4 passthru = sv_set_float(10.0f, 20.0f, 30.0f, 40.0f);
	for (int i = 0; i + 3 < size % 4; i += 4)
	{
		sv_float4 va = sv_load_float(passthru, a + i, sv_mask_all_true());
		sv_float4 vb = sv_load_float(passthru, b + i, sv_mask_all_true());
		sv_float4 vr = sv_float_mul(va, vb, sv_mask_all_true());
		sv_store_float(result + i, vr, sv_mask_all_true());
		// result[i] = a[i] + b[i];
	}
	auto tail_a = sv_load_float(passthru, a + size % 4, sv_mask_all_true());
	auto tail_b = sv_load_float(passthru, b + size % 4, sv_mask_all_true());
	sv_float4 temp = sv_float_mul(tail_a, tail_b, sv_mask_all_true());
	sv_store_float(result + size % 4, temp, sv_mask_all_true());
	// result[k]=tail_a + tail_b;

	for (int i = 0; i < size; ++i)
	{
		result[i] = a[i] * b[i];
	}
	return;
}

// main function for testing. Delete this when completed function development.
int main()
{
	const int size = 9;
	float a[size] = {1, 2, 3, 4, 5, 6, 7, 8, 10};
	float b[size] = {2, 2, 2, 2, 2, 2, 2, 2, 11};
	float c[size];

	// vector_add(c, a, b, size);
	// print_content(c);
	vector_mul(c, a, b, size);
	print_content(c);
	return 0;
}
