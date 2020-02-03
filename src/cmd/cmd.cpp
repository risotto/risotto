#include <lib/Risotto.h>

#include "../../lib/cxxopts/include/cxxopts.hpp"

#define DEBUG_CMD_GROUP "Debug"

int main(int argc, char *argv[]) {
    cxxopts::Options options(argv[0], "Risotto - YAPL ¯\\_(ツ)_/¯");

    options.add_options()
            ("file", "File name", cxxopts::value<std::string>())
            ("args", "Arguments", cxxopts::value<std::vector<std::string>>()->default_value(""))
            ("h,help", "Show this screen");

    options.add_options(DEBUG_CMD_GROUP)
            ("timings", "Print timings", cxxopts::value<bool>()->default_value("false"))
            ("tokens", "Print tokens", cxxopts::value<bool>()->default_value("false"))
            ("ast", "Print AST", cxxopts::value<bool>()->default_value("false"))
            ("disassemble", "Disassemble bytecode", cxxopts::value<bool>()->default_value("false"));

#ifdef DEBUG_TRACE_EXECUTION
    options.add_options(DEBUG_CMD_GROUP)
            ("trace-execution", "Print trace execution", cxxopts::value<bool>()->default_value("false"));
#endif

#ifdef BENCHMARK_TIMINGS
    options.add_options(DEBUG_CMD_GROUP)
            ("benchmark-execution", "Benchmark execution", cxxopts::value<bool>()->default_value("false"));
#endif

    options.parse_positional({"file", "args"});
    options.positional_help("file [arguments]");

    auto result = options.parse(argc, argv);

    if (result["help"].count() > 0) {
        std::cout << options.help();
        return 0;
    }

    auto path = result["file"].as<std::string>();

    if (path.empty()) {
        throw std::logic_error("path must be provided");
    }

    auto flags = (unsigned int) RisottoFlags::None;

    auto printTimings = result["timings"].as<bool>();
    if (printTimings) {
        flags |= RisottoFlags::PrintTimings;
    }

    auto printTokens = result["tokens"].as<bool>();
    if (printTokens) {
        flags |= RisottoFlags::PrintTokens;
    }

    auto printAST = result["ast"].as<bool>();
    if (printAST) {
        flags |= RisottoFlags::PrintAST;
    }

    auto printDisassembled = result["disassemble"].as<bool>();
    if (printDisassembled) {
        flags |= RisottoFlags::PrintDisassembled;
    }

#ifdef DEBUG_TRACE_EXECUTION
    auto traceExecution = result["trace-execution"].as<bool>();
    if (traceExecution) {
        flags |= RisottoFlags::PrintTraceExecution;
    }
#endif

#ifdef BENCHMARK_TIMINGS
    auto benchmarkExecution = result["benchmark-execution"].as<bool>();
    if (benchmarkExecution) {
        flags |= RisottoFlags::PrintBenchmarkExecution;
    }
#endif

    auto args = result["args"].as<std::vector<std::string>>();

    Risotto(flags).runFile(path, args);

    return 0;
}
