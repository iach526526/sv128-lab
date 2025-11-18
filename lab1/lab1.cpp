#include <iostream>
#include <sv128/sv128.h>	 // Make sure this header is found via -I./include
#include <sv128/sv_logger.h> // Make sure this header is found via -I./include

using namespace std;
void vector_add(float *result, const float *a, const float *b, int size);
void vector_mul(float *result, const float *a, const float *b, int size);
void vector_saxpy(float *y, const float *x, float alpha, int size);

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
	int i = 0;
	sv_float4 passthru=sv_set_float(10.0f, 20.0f, 30.0f, 40.0f);
	sv_float4 valpha=sv_set_float(alpha, alpha, alpha, alpha);
	for (; i + 3 < size; i += 4)
	{
		sv_float4 vx = sv_load_float(passthru, x + i, sv_mask_all_true());

		sv_float4 summand = sv_float_mul(vx, valpha, sv_mask_all_true());
		sv_float4 vy = sv_load_float(passthru, y + i, sv_mask_all_true());

		// y[i] = alpha * x[i] + y[i]
		sv_float4 vr = sv_float_add(summand, vy, sv_mask_all_true());
		sv_store_float(y + i, vr, sv_mask_all_true());
	}


	if(size %4 ==0) return;// all elements have been processed
	int remaining = size - i;
	sv_mask tail_mask = sv_init_ones(remaining);// remaining = 1..3 : [T,F,F,F] / [T,T,F,F] / [T,T,T,F]
	auto tail_x = sv_load_float(passthru, x + i, tail_mask);
	sv_float4 summand = sv_float_mul(tail_x, valpha, tail_mask);
	// cout<<"summand:"<<summand<<"\n";
	auto tail_y = sv_load_float(passthru, y + i, tail_mask);
	// cout<<"tail y:"<<tail_y<<"\n";
	sv_float4 temp = sv_float_add(summand, tail_y, tail_mask);
	// cout<<"temp:"<<temp<<"\n";
	// std::cout<<"tail a:"<<tail_a<<"\n"<<"tail b:"<<tail_b<<"\n"<<temp<<"\n";
	sv_store_float(y + i, temp, tail_mask);
	return;
}
