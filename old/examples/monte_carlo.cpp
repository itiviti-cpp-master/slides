#include <array>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>

const unsigned long min_num_of_run = 1;
const unsigned long max_num_of_run = 1000000000;

void run_simulation(const unsigned num_of_runs, std::function<unsigned()> roll_dice)
{
    for (unsigned difficulty = 4; difficulty <= 10; ++difficulty) {
        for (unsigned ability = 1; ability <= 10; ++ability) {
            double botch_probability = 0;
            double failure_probability = 0;
            std::array<double, 10> success_probability; // distinct probability value for each successes number
            success_probability.fill(0);

            // run experiments
            for (unsigned i = 0; i < num_of_runs; ++i) {
                unsigned ones = 0;
                unsigned successes = 0;
                // roll required number of dices
                for (unsigned j = 0; j < ability; ++j) {
                    const auto x = roll_dice();
                    if (x == 1) {
                        ++ones;
                    }
                    else if (x >= difficulty) {
                        ++successes;
                    }
                }
                // outcome determination
                if (successes > 0) {
                    // each 'one' remove one 'success'
                    if (successes >= ones) {
                        successes -= ones;
                    }
                    else {
                        successes = 0;
                    }
                    // if there was at least one 'success' - it's not a failure
                    if (successes > 0) {
                        success_probability[successes - 1] += 1;
                    }
                    else {
                        failure_probability += 1;
                    }
                }
                else {
                    // no successes - either a simple failure or a botch
                    if (ones > 0) {
                        botch_probability += 1;
                    }
                    else {
                        failure_probability += 1;
                    }
                }
            }
            botch_probability /= num_of_runs;
            failure_probability /= num_of_runs;
            for (unsigned sn = 0; sn < ability; ++sn) {
                success_probability[sn] /= num_of_runs;
            }

            double total_success_probability = std::accumulate(success_probability.begin(), success_probability.end(), 0.0);
            double total_probability = total_success_probability + botch_probability + failure_probability;

            // print results
            std::cout << "Difficulty: " << difficulty << ", ability: " << ability <<
                ", Total: " << total_probability <<
                ", Botch: " << botch_probability << ", Failure: " << failure_probability <<
                ", Total success: " << total_success_probability;
            for (unsigned snum = 1; snum <= ability; ++snum) {
                std::cout << ", " << snum << " successes: " << success_probability[snum-1];
            }
            std::cout << std::endl;
        }
    }
}

// 10 sided dice simulation
//
auto make_distribution()
{
    // a uniform distribution of unsigned integers from 1 to 10
    return std::uniform_int_distribution<unsigned>(1, 10);
}

// random generator
//
using RndGen = std::mt19937_64; // 64-bit Mersenne Twister by Matsumoto and Nishimura

auto make_gen()
{
    std::random_device rd;  // non-deterministic random number generator using hardware entropy source
    return RndGen(rd());    // seeding Mersenne Twister with a non-deterministic value
}

int main(int argc, char ** argv)
{
    // cmd line arguments
    unsigned num_of_runs = 100000;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--num_of_run") == 0) {
            const char * opt_value = argv[++i];
            char * end;
            unsigned long res = std::strtoul(opt_value, &end, 10);
            if (opt_value == end) {
                std::cerr << "Failed to parse 'num_of_run' value: '" << opt_value << "'" << std::endl;
                return -1;
            }
            if (res < min_num_of_run || res > max_num_of_run) {
                std::cerr << "Invalid 'num_of_run' value (" << res << "), should be between " << min_num_of_run <<
                    " and " << max_num_of_run << std::endl;
                return -1;
            }
            num_of_runs = static_cast<unsigned>(res);
            break;
        }
        else {
            std::cerr << "Unknown option '" << argv[i] << "'" << std::endl;
            return -1;
        }
    }

    // create generator and run simulation
    auto rnd_gen = make_gen();
    auto dist = make_distribution();
    run_simulation(num_of_runs, [&dist, &rnd_gen] () {
        return dist(rnd_gen);
    });
    return 0;
}

