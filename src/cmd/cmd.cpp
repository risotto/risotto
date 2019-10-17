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

    auto showTimings = result["timings"].as<bool>();
    auto showTokens = result["tokens"].as<bool>();
    auto showAST = result["ast"].as<bool>();
    auto showDisassemble = result["disassemble"].as<bool>();

    auto flags = (unsigned int) RisottoFlags::None;

    if (showTimings) {
        flags |= RisottoFlags::PrintTimings;
    }

    if (showTokens) {
        flags |= RisottoFlags::PrintTokens;
    }

    if (showAST) {
        flags |= RisottoFlags::PrintAST;
    }

    if (showDisassemble) {
        flags |= RisottoFlags::PrintDisassembled;
    }

    Risotto(flags).runFile(path);

    return 0;
}
