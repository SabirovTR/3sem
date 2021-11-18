#include <iostream>
#include <vector>
#include <limits>
#include <random>

class SkipList {
    struct SkipNode {
        double value;
        double key;
        std::vector<SkipNode*> forwardNodes;
        std::vector<SkipNode*> betweenNodes;

        explicit
        SkipNode(double value = 0, double key = 0,
                 std::vector<SkipNode*> forwardNodes = {}, std::vector<SkipNode*> betweenNodes = {}) :
                 value(value), key(key),
                 forwardNodes(forwardNodes), betweenNodes(betweenNodes) { };

        SkipNode(const SkipNode& sn) = default;

        SkipNode& operator=(const SkipNode& sn) = default;

        ~SkipNode() = default;
    };

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
        double tmp_value, tmp_key;
        std::vector<SkipNode*> tmp_forwardNodes, tmp_betweenNodes;

        tmp_value = tmp_key = std::numeric_limits<double>::max();
        for (int i = 0; i < MaxLevel; i++) {
            tmp_forwardNodes.push_back(nullptr);
            tmp_betweenNodes.push_back(head);
        }
        SkipNode *p_null = new SkipNode(tmp_value, tmp_key, tmp_forwardNodes, tmp_betweenNodes);
        null = p_null;

        tmp_forwardNodes.clear();
        tmp_betweenNodes.clear();

        tmp_value = tmp_key = std::numeric_limits<double>::min();
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
    SkipList(const SkipList &sl) : head(head), null(null), listSize(sl.listSize), MaxLevel(sl.MaxLevel) {
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
        sl.head = nullptr;
        sl.null = nullptr;
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
            out << tmp_ptr->value << " ";
            out << tmp_ptr->betweenNodes[0]->value << " " << tmp_ptr->forwardNodes[0]->value << " ";
            out << tmp_ptr->forwardNodes.size() << std::endl;
            tmp_ptr = tmp_ptr->forwardNodes[0];
        }
        return out;
    }
};

int main() {
    SkipList sl;
    for (int i = 0; i < 4; i++) {
        sl.insert(1, 1);
        sl.insert(0, 0);
        sl.insert(4, 4);
        sl.insert(10, 10);
    } //generation of sl
    //SkipList slsl(sl);
    //SkipList slslsl(slsl);
    //std::cout << slsl.count(10) << std::endl;
    //std::cout << sl;
    SkipList slslsl = sl;
    //std::cout << slslsl;
    slslsl.clear();
    std::cout << sl;
    //std::cout << slslsl;
    return 0;
}