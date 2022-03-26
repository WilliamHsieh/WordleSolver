#include <bits/stdc++.h>

// #Wordle
namespace wordle {

// ##Correctness
enum class Correctness : int {
    correct = 0, misplaced = 1, wrong = 2
};

// ##Profile
struct Profile {
    friend bool operator==(const Profile&, const Profile&) = default;

    operator int() {
        auto res = 0;
        for (auto& x : val) {
            res = res * 3 + static_cast<int>(x);
        }
        return res;
    }

    operator std::string() {
        std::string res;
        for (auto& x : val) {
            res += "O#X"[static_cast<int>(x)];
        }
        return res;
    }

    friend auto& operator>>(std::istream& is, Profile& p) {
        std::string s; is >> s; // O(correct), #(miss-placed), X(wrong)
        std::ranges::transform(s, p.val.begin(), [](auto& x) {
            switch(x) {
                case 'O':
                    return Correctness::correct;
                case '#':
                    return Correctness::misplaced;
                case 'X':
                default:
                    return Correctness::wrong;
            }
        });
        return is;
    }

    std::array<Correctness, 5> val;
};

// ##Word
struct Word {
    Word() : cnt(Combinations, 0) {}
    Word(std::string_view sv) : Word() {
        s = sv;
    }

    auto match(const Word& w) const {
        auto p = Profile{};
        std::ranges::transform(s, w.s, p.val.begin(), [&](auto& cur, auto& ans) {
            if (cur == ans) {
                return Correctness::correct;
            } else if (w.s.find(cur) != std::string::npos) {
                return Correctness::misplaced;
            } else {
                return Correctness::wrong;
            }
        });
        return p;
    }

    auto compute_entropy() {
        entropy = .0;
        for (auto x : cnt) {
            auto tmp = double(x) / cnt.size();
            entropy -= tmp == 0 ? 0 : tmp * log(tmp);
        }
    }

    friend auto& operator>>(std::istream& is, Word& w) {
        return is >> w.s >> w.freq;
    }

    std::vector<int> cnt;
    std::string s;
    double entropy;
    long long freq;
    static constexpr auto Combinations = 3 * 3 * 3 * 3 * 3;
};

// ##Solver
struct Solver {
    Solver(std::ifstream& fin) : cand(Wit(fin), Wit{}) {}

    auto run() {
        compute();

        // show 10 most informative candidate
        auto top = std::min(10, (int)cand.size());
        for (int i = 0; i < top; i++) {
            std::cout << '#' << i << ": " << cand[i].s << ' ' << cand[i].entropy << '\n';
        }

        // let user select a candidate
        int c;
        std::cout << "Choose: #";
        std::cin >> c;
        assert(c >= 0 and c < top);

        Profile p;
        std::cout << "Result: ";
        std::cin >> p;

        // get answer!
        if ((std::string)p == "OOOOO") {
            return true;
        }

        auto chosen = *std::ranges::find_if(cand, [&](auto& w) {
            return w.s == cand[c].s;
        });

        // update cand according to the remaining candidate
        auto tmp = std::vector<wordle::Word>{};
        for (auto& w : cand) {
            if (chosen.match(w) == p) {
                tmp.emplace_back(w.s);
            }
        }
        cand.swap(tmp);
        return false;
    }

  private:
    using Wit = std::istream_iterator<wordle::Word>;
    std::vector<wordle::Word> cand;

    void compute() {
        int n = cand.size();
        std::cout << ": " << n << " left.\n";

        // calculate all-pair matching profile && entropy
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                auto p = cand[i].match(cand[j]);
                cand[i].cnt[p] += 1;
            }
            cand[i].compute_entropy();
        }
        std::ranges::sort(cand, std::greater{}, &Word::entropy);
    }
};

}

// #Main
int main() {
    auto fin = std::ifstream("dict.txt");
    auto game = wordle::Solver(fin);
    for (int i = 1; i < 6; i++) {
        std::cout << std::string(20, '-') << '\n'
            << "Round " << i;
        if (game.run()) {
            std::cout << std::string(20, '-') << '\n'
                << "Wordle " << i << "/6\n";
            return 0;
        }
    }
    std::cout << "FAILED!\n";
    return 0;
}
