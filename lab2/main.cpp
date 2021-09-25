#include <iostream>
#include <set>
#include <random>

class GeneralState {
public:
    virtual bool contains(int s) const { return false; };
};

class DiscreteState : public GeneralState{
private:
    int const state;

public:
    DiscreteState(): state(0) { }
    DiscreteState(int state): state(state) { }

    bool contains(int s) const override {
        return s == state;
    }
};

class SegmentState : public GeneralState {
private:
    int const beg, end;

public:
    SegmentState(): beg(0), end(-1) { }
    SegmentState(int beg, int end): beg(beg), end(end) { }

    bool contains(int s) const {
        return s >= beg && s <= end;
    }
};

class SetState : public GeneralState{
private:
    std::set<int> const states;

public:
    SetState(): states() { }
    SetState(const std::set<int>  &src): states(src) { }

    bool contains(int s) const override {
        return states.count(s) > 0;
    }
};

class UnionState : public GeneralState {
private:
    DiscreteState const a;
    SegmentState const b;
    SetState const c;
public:
    UnionState(DiscreteState a, SegmentState b, SetState c) : a(a), b(b), c(c) {}
    UnionState(DiscreteState a, SegmentState b) : a(a), b(b) {}
    UnionState(SegmentState b, SetState c) : b(b), c(c) {}
    UnionState(DiscreteState a, SetState) : a(a), c(c) {}

    bool contains(int s) const {
        return a.contains(s) || b.contains(s) || c.contains(s);
    }
};

class InterState : public GeneralState {
private:
    DiscreteState const a;
    SegmentState const b;
    SetState const c;
    bool aa = false, bb = false, cc = false;
public:
    InterState(DiscreteState a, SegmentState b, SetState c) : a(a), b(b), c(c) {}
    InterState(DiscreteState a, SegmentState b) : a(a), b(b) {
        cc = true;
    }
    InterState(SegmentState b, SetState c) : b(b), c(c) {
        aa = true;
    }
    InterState(DiscreteState a, SetState c) : a(a), c(c) {
        bb = true;
    }

    bool contains(int s) const {
        return (aa || a.contains(s)) && (bb || b.contains(s)) && (cc || c.contains(s));
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

    float operator()(GeneralState const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

int main(int argc, const char * argv[]) {
    DiscreteState d(1);
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    ProbabilityTest pt(10, 0, 100, 100000);
    UnionState u(d, s, ss);
    UnionState udss(d, ss);
    InterState ids(d, s);
    InterState isss(s, ss);

    std::cout << "Probability from quantity of tests:" << std::endl;
    for (int i = 1; i <= 1000; i++){
        ProbabilityTest pt(50, 0, 100, 1000*i);
        std::cout << pt(d) << std::endl;
        std::cout << pt(s) << std::endl;
        std::cout << pt(ss) << std::endl;
        std::cout << pt(u) << std::endl;
        std::cout << pt(udss) << std::endl;
        std::cout << pt(ids) << std::endl;
        std::cout << pt(isss) << std::endl;
    }

    std::cout << "Probability from bound:" << std::endl;
    for (int i = 1; i <= 1000; i++){
        ProbabilityTest pt(50, 0, 1*i, 1000);
        std::cout << pt(d) << std::endl;
        std::cout << pt(s) << std::endl;
        std::cout << pt(ss) << std::endl;
        std::cout << pt(u) << std::endl;
        std::cout << pt(udss) << std::endl;
        std::cout << pt(ids) << std::endl;
        std::cout << pt(isss) << std::endl;
    }
    return 0;
}
