#include "PmergeMe.hpp"

#include <iostream>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <algorithm>
#include <utility>
#include <string>

static bool isAllDigits(const std::string &s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            return false;
    return true;
}

bool PmergeMe::parseArgs(int argc, char **argv, std::vector<int> &out) {
    out.clear();
    for (int i = 1; i < argc; ++i) {
        std::string s(argv[i]);
        if (!isAllDigits(s)) return false;
        long v = std::strtol(s.c_str(), 0, 10);
        if (v <= 0 || v > INT_MAX) return false;
        out.push_back(static_cast<int>(v));
    }
    return !out.empty();
}

void PmergeMe::printSequence(const char *label, const std::vector<int> &v) {
    std::cout << label;
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i];
        if (i + 1 < v.size()) std::cout << " ";
    }
    std::cout << std::endl;
}

std::vector<size_t> PmergeMe::buildJacobsthalOrder(size_t n) {
    // devolve índices 1-based em [2..n] numa ordem baseada em Jacobsthal (por batches)
    std::vector<size_t> order;
    if (n < 2) return order;

    std::vector<size_t> J;
    J.push_back(0);
    J.push_back(1);
    while (true) {
        size_t next = J[J.size()-1] + 2 * J[J.size()-2];
        J.push_back(next);
        if (next >= n) break;
        if (J.size() > 64) break;
    }

    size_t prev = 1;
    for (size_t k = 2; k < J.size(); ++k) {
        size_t cur = J[k];
        if (cur > n) cur = n;
        for (size_t i = cur; i > prev; --i)
            order.push_back(i);
        prev = cur;
        if (prev == n) break;
    }

    for (size_t i = n; i > prev; --i)
        order.push_back(i);

    return order;
}

template <typename Seq>
void PmergeMe::binaryInsert(Seq &seq, int value) {
    typename Seq::iterator it = std::lower_bound(seq.begin(), seq.end(), value);
    seq.insert(it, value);
}

struct PairSecondLess {
    bool operator()(const std::pair<int,int> &a, const std::pair<int,int> &b) const {
        return a.second < b.second;
    }
};

template <typename Seq>
void PmergeMe::fordJohnsonSort(Seq &seq) {
    if (seq.size() <= 1) return;

    typedef std::pair<int,int> Pair;
    std::vector<Pair> pairs;
    pairs.reserve(seq.size() / 2);

    bool hasStraggler = (seq.size() % 2 != 0);
    int straggler = 0;

    for (size_t i = 0; i + 1 < seq.size(); i += 2) {
        int a = seq[i];
        int b = seq[i+1];
        if (a <= b) pairs.push_back(Pair(a,b));
        else pairs.push_back(Pair(b,a));
    }
    if (hasStraggler) straggler = seq[seq.size()-1];

    std::sort(pairs.begin(), pairs.end(), PairSecondLess());

    Seq mainChain;
    mainChain.push_back(pairs[0].second);
    for (size_t i = 1; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].second);

    std::vector<int> pend;
    pend.reserve(pairs.size());
    for (size_t i = 0; i < pairs.size(); ++i)
        pend.push_back(pairs[i].first);

    // insere o primeiro min cedo
    binaryInsert(mainChain, pend[0]);

    // insere os restantes mins pela ordem Jacobsthal
    size_t m = pend.size();
    std::vector<size_t> order = buildJacobsthalOrder(m);
    for (size_t oi = 0; oi < order.size(); ++oi) {
        size_t idx1 = order[oi]; // 1-based
        if (idx1 < 2 || idx1 > m) continue;
        binaryInsert(mainChain, pend[idx1 - 1]);
    }

    if (hasStraggler)
        binaryInsert(mainChain, straggler);

    seq = mainChain;
}

// explicit instantiation (C++98)
template void PmergeMe::binaryInsert< std::vector<int> >(std::vector<int>&, int);
template void PmergeMe::binaryInsert< std::deque<int> >(std::deque<int>&, int);
template void PmergeMe::fordJohnsonSort< std::vector<int> >(std::vector<int>&);
template void PmergeMe::fordJohnsonSort< std::deque<int> >(std::deque<int>&);

double PmergeMe::sortAndMeasureVector(const std::vector<int> &input, std::vector<int> &sortedOut) {
    sortedOut = input;
    std::clock_t start = std::clock();
    fordJohnsonSort(sortedOut);
    std::clock_t end = std::clock();
    return (double)(end - start) * 1000000.0 / (double)CLOCKS_PER_SEC;
}

double PmergeMe::sortAndMeasureDeque(const std::vector<int> &input, std::deque<int> &sortedOut) {
    sortedOut.assign(input.begin(), input.end());
    std::clock_t start = std::clock();
    fordJohnsonSort(sortedOut);
    std::clock_t end = std::clock();
    return (double)(end - start) * 1000000.0 / (double)CLOCKS_PER_SEC;
}

void PmergeMe::run(int argc, char **argv) {
    std::vector<int> input;
    if (!parseArgs(argc, argv, input)) {
        std::cerr << "Error" << std::endl;
        return;
    }

    printSequence("Before: ", input);

    std::vector<int> sortedVec;
    std::deque<int> sortedDeq;

    double tVec = sortAndMeasureVector(input, sortedVec);
    double tDeq = sortAndMeasureDeque(input, sortedDeq);

    printSequence("After:  ", sortedVec);

    std::cout << "Time to process a range of " << input.size()
              << " elements with std::vector : " << tVec << " us" << std::endl;

    std::cout << "Time to process a range of " << input.size()
              << " elements with std::deque  : " << tDeq << " us" << std::endl;
}
