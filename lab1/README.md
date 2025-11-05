# About
現代 CPU/GPU 透過 SIMD instructions，能將同一個指令同時套用到多筆資料，大幅提升 throughput。sv128 是教學用 C++ library，以 128-bit vector width 模擬常見向量指令（例如同時載入/儲存 4 個 float，或一次對 4 個 float 做加乘運算），不必深入組合語言即可練習向量化思考。
# Goal
Complete the following vector operations using sv128 library functions:
```
void vector_add(float* result, const float* a, const float* b, int size);
void vector_mul(float* result, const float* a, const float* b, int size);
void vector_saxpy(float* y, const float* x, float alpha, int size);
```
- 功能說明：
    - `vector_add`：逐元素計算 `result[i] = a[i] + b[i]`
    - `vector_mul`：逐元素計算 `result[i] = a[i] * b[i]`
    - `vector_saxpy`：計算 **SAXPY**：`y[i] = alpha * x[i] + y[i]`（就地更新 `y`）