#include <benchmark/benchmark.h>
#include <lib/Risotto.h>

#define FILE(name) (ddir(__FILE__) + "/" + (name))
#define INPUT(name) FILE(name ".rst")

int noopprintf(const char *, ...) {
    // noop
}

#define SETUP auto risotto = Risotto(); risotto.printfp = noopprintf

#define RST_BENCHMARH(name, ...) \
    static void BM_##name(benchmark::State& state) { \
        auto file = INPUT(#name); \
        for (auto _ : state) { \
            SETUP; \
            risotto.runFile(file, {__VA_ARGS__}); \
        } \
    } \
    BENCHMARK(BM_##name);

std::string ddir(const std::string &file_path) {
    std::string dir_path = file_path.substr(0, file_path.rfind('/'));
    return dir_path;
}

RST_BENCHMARH(bounce)
RST_BENCHMARH(list)
RST_BENCHMARH(mandelbrot, "500")
RST_BENCHMARH(permute)
RST_BENCHMARH(sieve)
RST_BENCHMARH(storage)
RST_BENCHMARH(fib)

// Run the benchmark
BENCHMARK_MAIN();
