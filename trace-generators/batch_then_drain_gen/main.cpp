
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char** argv) {

    const std::string profile = "batch_then_drain";

    std::filesystem::path out_root = std::filesystem::current_path() / "traces";

    if (argc >= 2) {

        std::filesystem::path p = argv[1];

        if (p.extension() == ".trace") {

            std::cerr << "ERROR: first arg must be an output directory, not a .trace file.\n";
            return 2;
        }

        out_root = p;

    }

    const auto out_dir = out_root / profile;

    std::error_code ec;
    std::filesystem::create_directories(out_dir, ec);

    if (ec)
    {

        std::cerr << "mkdir " << out_dir << " : " << ec.message() << "\n";
        return 3;

    }

    std::cout << "exe = " << argv[0] << "\n";
    std::cout << "cwd = " << std::filesystem::current_path().string() << "\n";
    std::cout << "out = " << out_dir.string() << "\n";

    std::vector<std::size_t> Ns; Ns.reserve(11);

    for (int e = 10; e <= 20; ++e)
        Ns.push_back(std::size_t(1) << e);

    const std::vector<uint32_t> seeds = {101, 202, 303, 404, 505};

    constexpr int KEY_MIN = 1;
    constexpr int KEY_MAX = (1 << 20);

    for (auto N : Ns) {

        for (auto seed : seeds) {

            std::mt19937 rng(seed);
            std::uniform_int_distribution<int> dist(KEY_MIN, KEY_MAX);

            const auto path = out_dir / (profile + "_N_" + std::to_string(N) + "_S_" + std::to_string(seed) + ".trace");
            std::ofstream out(path);

            if (!out)
            {

                std::cerr << "open-for-write failed: " << path << "\n";

                return 1;
            }

            out << profile << " " << N << " " << seed << "\n";

            unsigned id = 0;

            for (std::size_t i = 0; i < N; ++i)
                out << "I " << dist(rng) << " " << id++ << "\n";

            for (std::size_t i = 0; i < N; ++i) {

                out << "F\n";
                out << "D\n";

            }

            out.close();
            std::cout << "wrote " << path.string() << "\n";
        }
    }
    return 0;
}

