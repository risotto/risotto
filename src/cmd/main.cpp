#include <lib/Risotto.h>

int main(int argc, char *argv[]) {
    auto path = argv[1];

    if (path == nullptr) {
        throw std::logic_error("path must be provided");
    }

    Risotto().runFile(path);

    return 0;
}
