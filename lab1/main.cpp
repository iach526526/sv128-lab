// main.cpp
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <cstring>
#include <string>
#include <iomanip>
#include <sv128/sv128.h>        // Make sure this header is found via -I./include
#include <sv128/sv_logger.h>    // Make sure this header is found via -I./include

// ===== Functions to be implemented by students (in lab1.cpp) =====
extern void vector_add(float* result, const float* a, const float* b, int size);
extern void vector_mul(float* result, const float* a, const float* b, int size);
extern void vector_saxpy(float* y, const float* x, float alpha, int size);

// ===== Scalar reference implementations (used as ground truth) =====
static void scalar_add(std::vector<float>& out,
                       const std::vector<float>& a,
                       const std::vector<float>& b) {
    const int n = static_cast<int>(out.size());
    for (int i = 0; i < n; ++i) out[i] = a[i] + b[i];
}

static void scalar_mul(std::vector<float>& out,
                       const std::vector<float>& a,
                       const std::vector<float>& b) {
    const int n = static_cast<int>(out.size());
    for (int i = 0; i < n; ++i) out[i] = a[i] * b[i];
}

static void scalar_saxpy(std::vector<float>& y,
                         const std::vector<float>& x,
                         float alpha) {
    const int n = static_cast<int>(y.size());
    for (int i = 0; i < n; ++i) y[i] = alpha * x[i] + y[i];
}

// ===== Comparison utilities =====
struct Diff {
    int index;
    float expected;
    float got;
};

static bool almost_equal(float a, float b, float eps = 1e-5f) {
    // Allow small floating-point rounding errors
    return std::fabs(a - b) <= eps * (1.0f + std::max(std::fabs(a), std::fabs(b)));
}

static bool compare_arrays(const std::vector<float>& expected,
                           const std::vector<float>& got,
                           std::vector<Diff>& diffs,
                           float eps = 1e-5f,
                           int max_report = 5) {
    bool ok = true;
    for (int i = 0; i < static_cast<int>(expected.size()); ++i) {
        if (!almost_equal(expected[i], got[i], eps)) {
            if (static_cast<int>(diffs.size()) < max_report) {
                diffs.push_back({i, expected[i], got[i]});
            }
            ok = false;
        }
    }
    return ok;
}

static void print_result(const std::string& name, bool pass,
                         const std::vector<Diff>& diffs,
                         long long instructions, long long utilized_lanes) {
    std::cout << std::left << std::setw(45) << name;
    if (pass) {
        double utilization = (instructions > 0) ?
            (100.0 * utilized_lanes / (instructions * VECTOR_WIDTH)) : 0.0;

        std::cout << "[PASS] "
                  << "Inst: " << std::setw(5) << instructions
                  << " Lanes: " << std::setw(7) << utilized_lanes
                  << " Util: " << std::fixed << std::setprecision(2)
                  << utilization << "%" << std::endl;
    } else {
        std::cout << "[FAIL] " << " (showing up to "
                  << diffs.size() << " diffs)\n";
        for (const auto& d : diffs) {
            std::cout << "  idx " << d.index
                      << "  expected=" << std::setprecision(8) << d.expected
                      << "  got=" << std::setprecision(8) << d.got << "\n";
        }
    }
}

// ===== Test data generation =====
static void make_inputs(int n,
                        std::vector<float>& a,
                        std::vector<float>& b,
                        std::vector<float>& x,
                        std::vector<float>& y) {
    a.resize(n); b.resize(n); x.resize(n); y.resize(n);
    std::mt19937 rng(12345);  // Fixed seed for reproducibility
    std::uniform_real_distribution<float> distA(-5.0f, 5.0f);
    std::uniform_real_distribution<float> distB(-3.0f, 3.0f);
    std::uniform_real_distribution<float> distX(-2.0f, 2.0f);
    std::uniform_real_distribution<float> distY(-1.0f, 1.0f);
    for (int i = 0; i < n; ++i) {
        a[i] = distA(rng);
        b[i] = distB(rng);
        x[i] = distX(rng);
        y[i] = distY(rng);
    }
}

// ===== Individual test cases =====
static bool test_add(int n) {
    std::vector<float> a, b, x, y;
    make_inputs(n, a, b, x, y);
    std::vector<float> got(n, 0), ref(n, 0);

    // Reference implementation (scalar)
    scalar_add(ref, a, b);

    // Instrument the function call
    sv_logger_init();
    vector_add(got.data(), a.data(), b.data(), n);
    long long instructions = sv_logger_get_total_instructions();
    long long utilized_lanes = sv_logger_get_utilized_lanes();

    std::vector<Diff> diffs;
    bool pass = compare_arrays(ref, got, diffs);
    print_result("vector_add (n=" + std::to_string(n) + ")", pass, diffs, instructions, utilized_lanes);
    return pass;
}

static bool test_mul(int n) {
    std::vector<float> a, b, x, y;
    make_inputs(n, a, b, x, y);
    std::vector<float> got(n, 0), ref(n, 0);

    scalar_mul(ref, a, b);

    // Instrument the function call
    sv_logger_init();
    vector_mul(got.data(), a.data(), b.data(), n);
    long long instructions = sv_logger_get_total_instructions();
    long long utilized_lanes = sv_logger_get_utilized_lanes();

    std::vector<Diff> diffs;
    bool pass = compare_arrays(ref, got, diffs);
    print_result("vector_mul (n=" + std::to_string(n) + ")", pass, diffs, instructions, utilized_lanes);
    return pass;
}

static bool test_saxpy(int n, float alpha) {
    std::vector<float> a, b, x, y;
    make_inputs(n, a, b, x, y);
    std::vector<float> y_ref = y;  // Reference copy (modified by scalar version)
    std::vector<float> y_got = y;  // Tested copy (modified by student function)

    scalar_saxpy(y_ref, x, alpha);

    // Instrument the function call
    sv_logger_init();
    vector_saxpy(y_got.data(), x.data(), alpha, n);
    long long instructions = sv_logger_get_total_instructions();
    long long utilized_lanes = sv_logger_get_utilized_lanes();

    std::vector<Diff> diffs;
    bool pass = compare_arrays(y_ref, y_got, diffs);
    print_result("vector_saxpy (n=" + std::to_string(n) + ", alpha=" + std::to_string(alpha) + ")", pass, diffs, instructions, utilized_lanes);
    return pass;
}

// ===== Main function =====
int main() {
    int total = 0, passed = 0;

    // 1) Sizes that are multiples of 4
    for (int n : {4, 8, 16, 32}) {
        total += 3;
        passed += test_add(n) ? 1 : 0;
        passed += test_mul(n) ? 1 : 0;
        passed += test_saxpy(n, 2.0f) ? 1 : 0;
    }

    // 2) Sizes that are not multiples of 4
    for (int n : {5, 7, 10, 13, 29}) {
        total += 3;
        passed += test_add(n) ? 1 : 0;
        passed += test_mul(n) ? 1 : 0;
        passed += test_saxpy(n, -0.5f) ? 1 : 0;
    }

    // 3) Sizes smaller than 4
    for (int n : {0, 1, 2, 3}) {
        total += 3;
        passed += test_add(n) ? 1 : 0;
        passed += test_mul(n) ? 1 : 0;
        passed += test_saxpy(n, 1.25f) ? 1 : 0;
    }

    // 4) Mixed random test sizes (fixed set)
    for (int n : {11, 24, 33, 41}) {
        total += 3;
        passed += test_add(n) ? 1 : 0;
        passed += test_mul(n) ? 1 : 0;
        passed += test_saxpy(n, 0.75f) ? 1 : 0;
    }

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
