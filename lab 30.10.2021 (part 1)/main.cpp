#include <iostream>
#include <vector>
#include <limits>
#include <random>

class SkipList {


    struct SkipNote {
        double value;
        double key;
        std::vector<SkipNote*> forwardNotes;

        explicit
        SkipNote(double value = 0, double key = 0, std::vector<SkipNote*> forwardNotes = {}) :
                value(value), key(key), forwardNotes(forwardNotes) { };

        //        explicit
        //        SkipNote(double value = 0, double key = 0, size_t MaxLevel = 1) :
        //        value(value), key(key) { forwardNotes = std::vector<SkipNote*>(MaxLevel); }

        SkipNote(const SkipNote& sn) = default;

        SkipNote& operator=(const SkipNote& sn) = default;

        ~SkipNote() = default;
    };

private:
    SkipNote* null;
    SkipNote* head;
    size_t listSize;
    size_t MaxLevel = 4;
    const double probability = 0.61803398875; //based on golden ratio

public:
    SkipList() {
        double tmp_value, tmp_key;
        std::vector<SkipNote*> tmp_forwardNotes;

        tmp_value = tmp_key = std::numeric_limits<double>::max();
        for (size_t i = 0; i < MaxLevel; i++) { tmp_forwardNotes.push_back(nullptr); }
        SkipNote *p_null = new SkipNote(tmp_value, tmp_key, tmp_forwardNotes);
        null = p_null;

        tmp_value = tmp_key = std::numeric_limits<double>::min();
        tmp_forwardNotes.clear();
        for (size_t i = 0; i < MaxLevel; i++) { tmp_forwardNotes.push_back(null); }
        SkipNote *p_head = new SkipNote(tmp_value, tmp_key, tmp_forwardNotes);
        head = p_head;

        listSize = 0;
    };

    SkipList(const SkipList& sl) :
    null(sl.null), head(sl.head), listSize(sl.listSize), MaxLevel(sl.MaxLevel) {
        SkipNote *tmp_sn = head;
        while (tmp_sn->forwardNotes[0] != nullptr) {
            SkipNote *sn = new SkipNote(*tmp_sn->forwardNotes[0]);
            tmp_sn->forwardNotes[0] = sn;
            tmp_sn = sn;
        }
//        for (size_t i = 0; i < MaxLevel; i++) {
//
//        }
    }

    void insert(double value, double key) {
        SkipNote* tmp_sn = head;
        std::vector<SkipNote*> tmp_betweenNotes(MaxLevel);
        std::vector<SkipNote*> tmp_forwardNotes(MaxLevel);
        for (size_t i = 1; i <= MaxLevel; i++) {
            while (tmp_sn->forwardNotes[MaxLevel - i]->key < key) {
                tmp_sn = tmp_sn->forwardNotes[MaxLevel - i];
            }
            tmp_betweenNotes[MaxLevel - i] = tmp_sn;
            tmp_forwardNotes[MaxLevel - i] = tmp_sn->forwardNotes[0];
        }
        std::mt19937 gen(0);
        std::uniform_real_distribution<> p(0, 1);




        size_t tmp_level = 1;
//      SkipNote* sn = new SkipNote(value, key);
        for (size_t i = 0; i < MaxLevel; i++) {
            if (probability < p(gen)) {
                tmp_level++;
            }
            else {
                break;
            }
        }
        std::vector<SkipNote*> tmp_list(tmp_level);
        SkipNote* sn = new SkipNote(value, key, tmp_list);
        for (size_t i = 0; i < tmp_level; i++) {
            tmp_list[i] = tmp_forwardNotes[i];
            *tmp_betweenNotes[i] = *sn;
        }
        listSize++;
    }

    bool empty() const { return (listSize == 0); }

    size_t size() const { return listSize; }

    friend std::ostream& operator<<(std::ostream& out, const SkipList& sl) {
        SkipNote* tmp_sn = sl.head;
        for (size_t i = 0; i < sl.listSize; i++) {
            out << tmp_sn->value << " ";
        }
        return out;
    }
};

int main() {
    SkipList sl;
    std::cout << sl << std::endl;
    sl.insert(1, 1);
    SkipList slsl(sl);
    //sl.insert(3, 3);
    //sl.insert(2, 2);
    std::cout << sl.size() << std::endl;
    std::cout << sl << std::endl;
    std::cout << slsl << std::endl;
    //    std::cout << std::numeric_limits<double>::max() << std::endl;
    //    std::mt19937 gen(0);
    //    std::uniform_real_distribution<> p(0, 1);
    //    std::cout << p(gen) << std::endl;
    //    std::cout << p(gen) << std::endl;
    //    std::vector<int> a(5);
    //    std::vector<int> b(a.begin(), a.begin() + 2);
    //    std::cout << a.size() << " " << b.size() << std::endl;
    return 0;
}