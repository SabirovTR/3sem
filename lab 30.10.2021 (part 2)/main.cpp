#include <iostream>
#include <vector>
#include <limits>
#include <random>

template <typename T> class SkipList {
    struct SkipNode {
        T value;
        double key;
        std::vector<SkipNode*> forwardNodes;
        std::vector<SkipNode*> betweenNodes;

        explicit
        SkipNode(T value, double key = 0,
                 std::vector<SkipNode*> forwardNodes = {}, std::vector<SkipNode*> betweenNodes = {}) :
                 value(value), key(key),
                 forwardNodes(forwardNodes), betweenNodes(betweenNodes) { };

        SkipNode(const SkipNode& sn) = default;

        SkipNode& operator=(const SkipNode& sn) = default;

        ~SkipNode() = default;
    };

public:
    struct BidirectionalIterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SkipNode;
        using pointer           = SkipNode*;
        using reference         = SkipNode&;

        explicit
        BidirectionalIterator(pointer ptr_sn) : ptr_sn(ptr_sn) {}

        //pre increment
        BidirectionalIterator& operator++() {
            ptr_sn = ptr_sn->forwardNodes[0];
            return *this;
        }
        //post increment
        BidirectionalIterator operator++(int) {
            BidirectionalIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        //pre increment
        BidirectionalIterator& operator--() {
            ptr_sn = ptr_sn->betweenNodes[0];
            return *this;
        }
        //post increment
        BidirectionalIterator operator--(int) {
            BidirectionalIterator tmp = *this;
            --(*this);
            return tmp;
        }
        reference operator*() const {
            return *ptr_sn;
        }
        pointer operator->() {
            return ptr_sn;
        }

        void find(BidirectionalIterator begin, BidirectionalIterator end, double key) {
            BidirectionalIterator tmp_it = begin;
            while (tmp_it != end && tmp_it->key != key) {
                tmp_it++;
            }
            *this = tmp_it;
        }

        void lower_bound(BidirectionalIterator begin, BidirectionalIterator end, double key) {
            BidirectionalIterator tmp_it = begin;
            while (tmp_it != end && tmp_it->key < key) {
                tmp_it++;
            }
            *this = tmp_it;
        }

        void upper_bound(BidirectionalIterator begin, BidirectionalIterator end, double key) {
            BidirectionalIterator tmp_it = begin;
            while (tmp_it != end && tmp_it->key <= key) {
                tmp_it++;
            }
            *this = tmp_it;
        }

        friend bool operator!=(const BidirectionalIterator& a, const BidirectionalIterator& b) {
            return a.ptr_sn != b.ptr_sn;
        }
    private:
        pointer ptr_sn;
    };

    BidirectionalIterator begin() {
        return BidirectionalIterator(head->forwardNodes[0]);
    }
    BidirectionalIterator end() {
        return BidirectionalIterator(null);
    }

    struct reverse_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = SkipNode;
        using pointer           = SkipNode*;
        using reference         = SkipNode&;

        explicit
        reverse_iterator(pointer ptr_sn) : ptr_sn(ptr_sn) {}

        //pre increment
        reverse_iterator& operator++() {
            ptr_sn = ptr_sn->betweenNodes[0];
            return *this;
        }
        //post increment
        reverse_iterator operator++(int) {
            reverse_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        //pre increment
        reverse_iterator& operator--() {
            ptr_sn = ptr_sn->forwardNodes[0];
            return *this;
        }
        //post increment
        reverse_iterator operator--(int) {
            reverse_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        reference operator*() const {
            return *ptr_sn;
        }
        pointer operator->() {
            return ptr_sn;
        }

        friend bool operator!=(const reverse_iterator& a, const reverse_iterator& b) {
            return a.ptr_sn != b.ptr_sn;
        }
    private:
        pointer ptr_sn;
    };

    reverse_iterator rbegin() {
        return reverse_iterator(null->betweenNodes[0]);
    }
    reverse_iterator rend() {
        return reverse_iterator(head);
    }

private:
    SkipNode* null;
    SkipNode* head;
    int listSize;
    int MaxLevel = 4;
    const double probability = 0.61803398875; //based on golden ratio

    static bool comp(double left, double right) {return (left < right);} //?

    int randomLevel() const {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> p(0, 1);
        int level = 1;
        while (probability >= p(gen) && level < MaxLevel) {level++;}
        return level;
    }

public:
    //constructor
    SkipList() {
        T tmp_value = 0;
        double tmp_key;
        std::vector<SkipNode*> tmp_forwardNodes, tmp_betweenNodes;

        tmp_key = std::numeric_limits<double>::max();
        for (int i = 0; i < MaxLevel; i++) {
            tmp_forwardNodes.push_back(nullptr);
            tmp_betweenNodes.push_back(head);
        }
        SkipNode *p_null = new SkipNode(tmp_value, tmp_key, tmp_forwardNodes, tmp_betweenNodes);
        null = p_null;

        tmp_forwardNodes.clear();
        tmp_betweenNodes.clear();

        tmp_key = std::numeric_limits<double>::min();
        for (int i = 0; i < MaxLevel; i++) {
            tmp_forwardNodes.push_back(null);
            tmp_betweenNodes.push_back(nullptr);
        }
        SkipNode *p_head = new SkipNode(tmp_value, tmp_key, tmp_forwardNodes, tmp_betweenNodes);
        head = p_head;

        for (int i = 0; i < MaxLevel; i++) { null->betweenNodes[i] = head; }

        listSize = 0;
    }

    //copy_constructor
    SkipList(const SkipList &sl) : head(sl.head), null(sl.null), listSize(sl.listSize), MaxLevel(sl.MaxLevel) {
        std::vector<SkipNode*> tmp_vect(MaxLevel);
        head = new SkipNode(sl.head->value, sl.head->key, tmp_vect, tmp_vect);
        null = new SkipNode(sl.null->value, sl.null->key, tmp_vect, tmp_vect);

        for (int i = 0; i < MaxLevel; i++) {
            head->forwardNodes[i] = null;
            head->betweenNodes[i] = nullptr;
            null->forwardNodes[i] = nullptr;
            null->betweenNodes[i] = head;
        }

        SkipNode *tmp_ptr = sl.head->forwardNodes[0];
        while (tmp_ptr != sl.null) {
            this->insert(tmp_ptr->value, tmp_ptr->key, tmp_ptr->forwardNodes.size());
            tmp_ptr = tmp_ptr->forwardNodes[0];
        }
    }

    //move_constructor
    SkipList(SkipList &&sl) : head(sl.head), null(sl.null), listSize(sl.listSize), MaxLevel(sl.MaxLevel) {
        sl.head->~SkipNode();
        sl.null->~SkipNode();
        sl.listSize = 0;
        sl.MaxLevel = 0;

        for (int i = 0; i < MaxLevel; i++) {null->betweenNodes[i]->forwardNodes[i] = null;}
    }

    void insert(double value, double key, int level = 0) {
        SkipNode *tmp_ptr = head;

        if (level == 0) {level = randomLevel();}
        std::vector<SkipNode*> tmp_Nodes(level);
        SkipNode *ptr_sn = new SkipNode(value, key, tmp_Nodes, tmp_Nodes);
        for (int i = MaxLevel - 1; i >= 0; i--) {
            while (comp(tmp_ptr->forwardNodes[i]->key, key)) {
                tmp_ptr = tmp_ptr->forwardNodes[i];
            }
            if (i < level) {
                ptr_sn->forwardNodes[i] = tmp_ptr->forwardNodes[i];
                ptr_sn->betweenNodes[i] = tmp_ptr;
                tmp_ptr->forwardNodes[i]->betweenNodes[i] = ptr_sn;
                tmp_ptr->forwardNodes[i] = ptr_sn;
            }
        }
        listSize++;
    }

    //destructor
    ~SkipList() {
        this->clear();
        head->~SkipNode();
        null->~SkipNode();
    }

    bool search(double key) {
        SkipNode *tmp_ptr = head;

        for (int i = MaxLevel - 1; i >= 0; i--) {
            while (comp(tmp_ptr->forwardNodes[i]->key, key)) {
                tmp_ptr = tmp_ptr->forwardNodes[i];
            }
        }
        if (tmp_ptr->forwardNodes[0]->key == key) {return true;}
        else {return false;}
    }

    int count(double key) {
        SkipNode *tmp_ptr = head;

        for (int i = MaxLevel - 1; i >= 0; i--) {
            while (tmp_ptr->forwardNodes[i]->key < key) {
                tmp_ptr = tmp_ptr->forwardNodes[i];
            }
        }
        int count = 0;

        while (tmp_ptr->forwardNodes[0]->key == key) {
            tmp_ptr = tmp_ptr->forwardNodes[0];
            count++;
        }
        return count;
    }

    bool empty() const { return (listSize == 0); }

    int size() const { return listSize; }

    void clear() {
        if (!this->empty()) {
            SkipNode *tmp_ptr = head;
            SkipNode *check_ptr = head->forwardNodes[0];

            while (check_ptr != null) {
                tmp_ptr = check_ptr;
                check_ptr = check_ptr->forwardNodes[0];
                tmp_ptr->~SkipNode();
            }

            for (int i = 0; i < MaxLevel; i++) {
                head->forwardNodes[i] = null;
                null->betweenNodes[i] = head;
            }
            listSize = 0;
        }
    }

    void erase(BidirectionalIterator& it) {
        SkipNode tmp_sn = *it;
        for (int i = 0; i < tmp_sn.forwardNodes.size(); i++) {
            tmp_sn.forwardNodes[i]->betweenNodes[i] = tmp_sn.betweenNodes[i];
            tmp_sn.betweenNodes[i]->forwardNodes[i] = tmp_sn.forwardNodes[i];
        }
        it++;
        tmp_sn.~SkipNode();
        listSize--;
    }

    //copy_assignment_operator
    SkipList& operator=(const SkipList &sl) {
        if (&sl == this) {return *this;}

        SkipList *tmp_sl = new SkipList(sl);
        head = tmp_sl->head;
        null = tmp_sl->null;
        listSize = tmp_sl->listSize;
        MaxLevel = tmp_sl->MaxLevel;
        return *this;
    }

    //move_assignment_operator
    SkipList& operator=(SkipList &&sl) {
        if (&sl == this) {return *this;}

        head = sl.head;
        null = sl.null;
        for(int i = 0; i < MaxLevel; i++) {null->betweenNodes[i]->forwardNodes[i] = null;}

        sl.clear();
        sl.head->~SkipNode();
        sl.null->~SkipNode();
        sl.listSize = 0;
        sl.MaxLevel = 0;
    }

    friend std::ostream& operator<<(std::ostream& out, const SkipList& sl) {
        SkipNode* tmp_ptr;
        tmp_ptr = sl.head->forwardNodes[0];
        while (tmp_ptr != sl.null) {
            out << tmp_ptr->key << " ";
            out << tmp_ptr->betweenNodes[0]->key << " " << tmp_ptr->forwardNodes[0]->key << " ";
            out << tmp_ptr->value << " ";
            out << tmp_ptr->betweenNodes[0]->value << " " << tmp_ptr->forwardNodes[0]->value << " ";
            out << tmp_ptr->forwardNodes.size() << std::endl;
            tmp_ptr = tmp_ptr->forwardNodes[0];
        }
        return out;
    }
};

int main() {
    SkipList<char> sl;
    for (int i = 0; i < 4; i++) {
        sl.insert('b', 1);
        sl.insert('a', 0);
        sl.insert('f', 4);
        sl.insert('w', 10);
    } //generation of sl
    SkipList<char> slsl(sl);
    std::cout << slsl;

    SkipList<char>::BidirectionalIterator it = sl.begin();
    int counter = 1;
    while (it != sl.end()) {
        std::cout << counter << " ";
        counter++;
        it++;
    }
    std::cout << std::endl;
    it.find(sl.begin(), sl.end(), 4);
    std::cout << it->value << std::endl;
    it.find(sl.begin(), sl.end(), 3);
    std::cout << it->value << std::endl;
    it.lower_bound(sl.begin(), sl.end(), 1);
    std::cout << it->value << std::endl;
    it.upper_bound(sl.begin(), sl.end(), 1);
    std::cout << it->value << std::endl;
    std::cout << "listSize before erase: " << sl.size() << std::endl;
    sl.erase(it);
    sl.erase(it);
    sl.erase(it);
    sl.erase(it);
    std::cout << sl;
    std::cout << "listSize after erase: " << sl.size() << std::endl;
    return 0;
}