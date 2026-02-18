#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>

class PmergeMe {
public:
    static void run(int argc, char **argv);

private:
    static bool parseArgs(int argc, char **argv, std::vector<int> &out);
    static void printSequence(const char *label, const std::vector<int> &v);

    static std::vector<std::size_t> buildJacobsthalOrder(std::size_t n); // indices 2..n (1-based)

    template <typename Seq>
    static void binaryInsert(Seq &seq, int value);

    template <typename Seq>
    static void fordJohnsonSort(Seq &seq);

    static double sortAndMeasureVector(const std::vector<int> &input, std::vector<int> &sortedOut);
    static double sortAndMeasureDeque(const std::vector<int> &input, std::deque<int> &sortedOut);
};

#endif
