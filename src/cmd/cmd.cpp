#include <lib/Risotto.h>

#include "../../lib/cxxopts/include/cxxopts.hpp"

int main(int argc, char *argv[]) {
    cxxopts::Options options(argv[0], "Risotto - YAPL ¯\\_(ツ)_/¯");

    options.add_options()
            ("f,file", "File name", cxxopts::value<std::string>())
            ("h,help", "Help");

    options.add_options("Debug")
            ("timings", "Print timings", cxxopts::value<bool>()->default_value("false"))
            ("tokens", "Print tokens", cxxopts::value<bool>()->default_value("false"))
            ("ast", "Print AST", cxxopts::value<bool>()->default_value("false"))
            ("disassemble", "Disassemble bytecode", cxxopts::value<bool>()->default_value("false"));

#ifdef DEBUG_TRACE_EXECUTION
    options.add_options("Debug")
            ("trace-execution", "Print trace execution", cxxopts::value<bool>()->default_value("false"));
#endif

    options.parse_positional({"file"});
    options.positional_help("[file]")
            .show_positional_help();

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

    auto showTimings = result["timings"].as<bool>();
    if (showTimings) {
        flags |= RisottoFlags::PrintTimings;
    }

    auto showTokens = result["tokens"].as<bool>();
    if (showTokens) {
        flags |= RisottoFlags::PrintTokens;
    }

    auto showAST = result["ast"].as<bool>();
    if (showAST) {
        flags |= RisottoFlags::PrintAST;
    }

    auto showDisassemble = result["disassemble"].as<bool>();
    if (showDisassemble) {
        flags |= RisottoFlags::PrintDisassembled;
    }

#ifdef DEBUG_TRACE_EXECUTION
    auto showTraceExecution= result["trace-execution"].as<bool>();
    if (showTraceExecution) {
        flags |= RisottoFlags::PrintTraceExecution;
    }
#endif

    Risotto(flags).runFile(path);

    return 0;
}
