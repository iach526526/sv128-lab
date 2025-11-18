#include <sv128/sv128.h>	 // Make sure this header is found via -I./include
#define all_t sv_mask_all_true()
#include <iostream>
using namespace std;
/**
 * @brief Computes the dot product of two vectors.
 * * Calculates sum(a[i] * b[i] for i in 0..size-1)
 *
 * @param a The first input array
 * @param b The second input array
 * @param size The length of the arrays
 * @return float The resulting dot product
 */
float vector_dot_product(const float* a, const float* b, int size)
{
    sv_float4 passthru = sv_set_float(10.0f,20.0f,30.0f,40.0f);
    sv_float4 vsum = sv_set_float(0.0f,0.0f,0.0f,0.0f);
    int i=0;
    for(;i+3<size;i+=4){
        sv_float4 va=sv_load_float(passthru,a + i,all_t);
        sv_float4 vb=sv_load_float(passthru,b + i,all_t);
        sv_float4 product = sv_float_mul(va,vb,all_t);
        
        vsum = sv_float_add(vsum,product,all_t);
    }
    // maxSize - i 是剩下未做的元素數量
    sv_mask least_mask=sv_init_ones(size-i);
    // cout<<"least mask:"<<least_mask<<"\n";
    // dependence do least lane
    
    sv_float4 va=sv_load_float(passthru,a + i,least_mask);
    sv_float4 vb=sv_load_float(passthru,b + i,least_mask);
    sv_float4 tail_product = sv_float_mul(va,vb,least_mask);
    vsum = sv_float_add(vsum,tail_product,least_mask);
    
    // cout<<vsum<<"\n";
    float sum_lane[4];
    sv_store_float(sum_lane,vsum,all_t);
    return sum_lane[0]+sum_lane[1]+sum_lane[2]+sum_lane[3];
}

/**
 * @brief Blends elements from 'if_true' and 'if_false' arrays based on a condition array.
 *
 * Element-wise computation:
 * result[i] = (cond[i] > 0) ? if_true[i] : if_false[i];
 *
 * @param result Output array
 * @param cond Condition array
 * @param if_true Array selected when 'cond[i] > 0' is true
 * @param if_false Array selected when 'cond[i] > 0' is false
 * @param size The length of the arrays
 */
void vector_blend(float* result, const float* cond, const float* if_true, const float* if_false, int size) {
    sv_float4 passthru = sv_set_float(10.0f,20.0f,30.0f,40.0f);
    sv_float4 all_zero_vectors = sv_set_float(0.0f, 0.0f, 0.0f, 0.0f);
    
    int i=0;
    for(;i+3<size;i+=4){
        sv_float4 vcon = sv_load_float(passthru,cond + i,all_t);
        // the mask where cond[i] > 0
        sv_mask cond_mask = sv_float_gt(vcon,all_zero_vectors);
        sv_mask cond_mask_invserse = sv_mask_not(cond_mask);

        // all if_true vector
        sv_float4 vtrue = sv_load_float(passthru,if_true + i,all_t);
        // all if_false vector
        sv_float4 vfalse = sv_load_float(passthru,if_false + i,all_t);


        // select from vtrue and vfalse based on cd_mask
        sv_store_float(result + i, vtrue , cond_mask);
        sv_store_float(result + i, vfalse , cond_mask_invserse);
        

    }
    for (; i < size; i++) {
        result[i] = (cond[i] > 0) ? if_true[i] : if_false[i]; // 纯量 loop 處理剩下的元素
    }

}
// int main() {
//     int size = 10; // 示例大小
//     float cond[] = {1.0f, -2.0f, 0.0f, 4.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
//     float if_true[] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
//     float if_false[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
//     float result[10] = {0};

//     vector_blend(result, cond, if_true, if_false, size);

//     // 打印结果
//     for (int i = 0; i < size; i++) {
//         printf("result[%d] = %f\n", i, result[i]);
//     }

//     return 0;
// }
// int main(){
//     float a[3] = {1.0f, 2.0f, 3.0f};
//     float b[] = {4.0f, 5.0f, 6.0f};
//     int size = 3;
//     float dot_product = vector_dot_product(a, b, size);
//     cout << "Dot Product: " << dot_product << endl;
// }