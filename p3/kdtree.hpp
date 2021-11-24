#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include<iostream>

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            // TODO: implement this function

            class rangeException{};
            // If we increment a nullptr(end), throw exception
            if(this->node == nullptr) {
                rangeException exception;
                throw exception;
            }

            // If this node does not have the right child
            if(this->node->right == nullptr) {
                while(this->node->parent != nullptr && this->node->parent->left != this->node) {
                    this->node = this->node->parent;
                }
                //If we find a parent such that its left child is this node, then we set the node to that parent, and we found the next
                if(this->node->parent != nullptr && this->node->parent->left == this->node) {
                    this->node = this->node->parent;
                }

                //Else the loop ends due to the the parent is null, then we meet the root node, return nullptr
                else {
                    this->node = nullptr;
                }
            }

            // If this node has a right child, go to that child, find the left most descendant
            else {
                this->node = this->node->right;
                while(this->node->left != nullptr) {
                    this->node = this->node->left;
                }
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            // TODO: implement this function

            class rangeException;
            // If we decrement the nullptr, we should get the right most node
            if(this->node == nullptr) {
                this->node = this->tree->root;
                while(this->node->right != nullptr) {
                    this->node = this->node->right;
                }
            }

            else {
                //if this node have left child, we find the right most node of the left child
                if(this->node->left != nullptr) {
                    this->node = this->node->left;
                    while(node->right != nullptr) {
                        this->node = this->node->right;
                    }
                }

                //if this node does not have left child, then we fidn the parent such that it is in the parent's right subtree
                else {
                    while(this->node->parent != nullptr && this->node->parent->right != this->node) {
                        this->node = this->node->parent;
                    }

                    if(this->node->parent->right == this->node) {
                        this->node = this->node->parent;
                    }
                    //else this node is the left most node 
                    else {
                        rangeException exception;
                        throw exception;
                    }
                }
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(node == nullptr) {
            return nullptr;
        }

        //if the current node has exactly the same key on all dimensions, then we are done
        if(this->isEqualKey(node->key(), key)) {
            return node;
        }

        //else we need to check the next layer
        if(this->compareKey_DIM<DIM>(key, node->key())) {
            return find<DIM_NEXT>(key, node->left);
        }
        else {
            return find<DIM_NEXT>(key, node->right);
        }
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(node == nullptr) {
            node = new Node(key, value, parent);

            //update the treesize
            this->treeSize ++;
            return true;
        }
        else {
            //if the key is equal in all dimension, then we change the value by replacing the node
            if(this->isEqualKey(node->key(), key)) {
                //create a new node with given information
                Node * new_node = new Node(key, value, parent);

                //get the original pointers copied
                new_node->left = node->left;
                new_node->right = node->right;
                if(node->left != nullptr) {
                    new_node->left->parent = new_node;
                }
                if(node->right != nullptr) {
                    new_node->right->parent = new_node;
                }
                delete node;
                node = new_node;
                
                return false;
            }

            if(this->compareKey_DIM<DIM>(key, node->key())) {
                return insert<DIM_NEXT>(key, value, node->left, node);
            }
            else {
                return insert<DIM_NEXT>(key, value, node->right, node);
            }
        }
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)){
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(node == nullptr) {
            return nullptr;
        }
        Node * min = findMin<DIM_CMP, DIM_NEXT>(node->left); //no matter what, the left subtree are all condidates
        if(DIM != DIM_CMP) {
            Node * rightMin = findMin<DIM_CMP,DIM_NEXT>(node->right); //if the dimension of this layer is not the target dimension, then right subtree are also candidates
            min = this->compareNode<DIM_CMP, std::less<>>(min,rightMin); //the min should be the smaller of right min and left min
        }
        return this->compareNode<DIM_CMP, std::less<>>(min, node); //don't bother to think about whether this layer is the desired dimension layer, just do a comparison
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if(node == nullptr) {
            return nullptr;
        }
        Node * max = findMax<DIM_CMP, DIM_NEXT>(node->right); //no matter what, the right subtree are all candidates 
        if(DIM_CMP != DIM) {
            Node * leftMax = findMax<DIM_CMP,DIM_NEXT>(node->left); //if the dimension of this layer is not the comparison dimension, then the left subtree are all candidates as well
            max = this->compareNode<DIM_CMP, std::greater<>>(max,leftMax);
        }
        return this->compareNode<DIM_CMP,std::greater<>>(max,node);
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // TODO: implement this function
        if (node == nullptr) {
            return nullptr;
        }

        bool isRoot = false;
        //note that the erase node may be the root node, in this case, if we delete the node directly without update the root pointer, the root pointer will be null
        if (node == this->root) {
            isRoot = true;
        }
        //if the key is what we want to find 
        if (this->isEqualKey(node->key(), key)) {
            //if the node is a leaf node, delete it directly
            if(node->left == nullptr && node->right == nullptr) {
                node->parent = nullptr;
                delete node;
                node = nullptr;
                //update the treesize
                treeSize--;
            }

            //if the node has right subtree then
            else if(node->right != nullptr) {
                //std::cout<<"The node has a right subtree"<<std::endl;
                Node * minNode = this->findMin<DIM, DIM_NEXT>(node->right);
                if(minNode == nullptr) {
                    // std::cout<<"The findMin is wrong"<<std::endl;
                }
                //copy the minnode to this node
                node = NodeCopy(node, minNode);
                // if(node->right == nullptr) {
                //     std::cout<<"The copy is not successful!"<<std::endl;
                // }
                // std::cout<<"The right node is:";
                // std::cout<<node->right->data.second<<std::endl;
                // std::cout<<"The victim is now with value:";
                // std::cout<<node->data.second<<std::endl;
                // std::cout<<"The victim's parent is:";
                // std::cout<<node->parent->data.second<<std::endl;
                // std::cout<<"The victim's parent's right child is:";
                // std::cout<<node->parent->right->data.second<<std::endl;
                node->right = erase<DIM_NEXT>(node->right, minNode->key());
            }

            //if the node has left subtree and does not have right subtree
            else if(node->left != nullptr) {
                Node * maxNode = this->findMax<DIM, DIM_NEXT>(node->left);
                node = NodeCopy(node, maxNode);
                // if(node->left == nullptr) {
                //     std::cout<<"The copy is not successful!"<<std::endl;
                // }
                // std::cout<<"The left node is:";
                // std::cout<<node->left->data.second<<std::endl;
                node->left = erase<DIM_NEXT>(node->left, maxNode->key());
            }
        }
        else {
            if(this->compareKey_DIM<DIM>(key, node->key())) {
                node->left = erase<DIM_NEXT>(node->left, key);
            }
            else {
                node->right = erase<DIM_NEXT>(node->right, key);
            }
        }

        if(isRoot) {
            root = node;
        }
        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // TODO: define your helper functions here if necessary

    //copy one node to the other node
    Node * NodeCopy(Node* dst, Node * src) {
        if(src == nullptr) {
            return nullptr;
        }
        //create a new node, deep copy the data from the srouce node
        Node * new_node = new Node(src->key(),src->value(), nullptr);

        //update three pointers of newnode
        new_node->left = dst->left;
        new_node->right = dst->right;
        new_node->parent = dst->parent;

        //change the parent's pointer
        if(dst->parent != nullptr && dst->parent->left == dst) {
            dst->parent->left = new_node;
        }
        if(dst->parent != nullptr && dst->parent->right == dst) {
            dst->parent->right = new_node;
        }

        //change the left child's pointer
        if(dst->left != nullptr) {
            dst->left->parent = new_node;
        }

        //change the right child's pointer
        if(dst->right != nullptr) {
            dst->right->parent = new_node;
        }

        //set the original pointers to null so that it will not cause series deletion
        dst->left = nullptr;
        dst->right = nullptr;
        dst->parent = nullptr;

        //delete the dst and return the new node
        delete dst;
        // std::cout<<"The copied data is:";
        // std::cout<<new_node->data.second<<std::endl;
        return new_node;
    }

    //return the node with smaller key
    static bool returnSmaller (const Node * a, const Node * b) { 
        if (a->data.first < b->data.first) {
            return true;
        }
        else {
            return false;
        }
    }

    //return the node with larger key
    static Node * returnLarger (const Node * a, const Node * b) {
        if(a->key() > b ->key()) {
            return a;
        }
        else {
            return b;
        }
    }

    static bool isEqualNode (Node * a, Node * b) {
        if (a->key() == b->key()) {
            return true;
        }
        return false;
    }

    static bool isEqualKey (const Key & a, const Key & b) {
        if (a == b) {
            return true;
        }
        else {
            return false;
        }
    }

    template<size_t DIM>
    static bool compareKey_DIM(const Key & a, const Key & b) {
        if(std::get<DIM>(a) < std::get<DIM>(b)) {
            return true;
        }
        else {
            return false;
        }
    }

    static bool compareData_ALL(const Data & a, const Data & b) {
        if(a.first < b.first) {
            return true;
        }
        else {
            return false;
        }
    }

    static bool isEqualData_ALL(const Data & a, const Data & b) {
        if(a.first == b.first) {
            return true;
        }
        else {
            return false;
        }
    }

    template<size_t DIM>
    static bool compareData_DIM(const Data & a, const Data & b) {
        return compareKey_DIM<DIM>(a.first, b.first);
    }

    template<size_t DIM>
    static Node * compareNode_DIM(Node * a, Node * b) {
        if(compareKey_DIM<DIM>(a->key(), b->key())) {
            return a;
        }
        else {
            return b;
        }
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        // TODO: implement this function
        // First run a stable sort and then do a std::unique to get rid of all the duplicated value
        // typename std::vector<std::pair<Key, Value>>::iterator i = v.begin();
        std::stable_sort(v.begin(), v.end(), this->compareData_ALL);

        // while(i != v.end()) {
        //     std::cout<<(*i).second<<std::endl;
        //     std::cout<<std::get<0>((*i).first)<<" "<<std::get<1>((*i).first)<<" "<<std::get<2>((*i).first)<<std::endl;
        //     i++;
        // }

        auto new_head = std::unique(v.rbegin(), v.rend(), this->isEqualData_ALL);
        v.erase(v.begin(), new_head.base());


        //Build the tree on the root node of this, and the depth of the root node is 0
        this->root = build_helper<0>(v, nullptr);

        // update the treesize
        this->treeSize = v.size();
    }

    template<size_t DIM>
    Node * build_helper(std::vector<std::pair<Key, Value>> data, Node * parent) {
        if(data.empty()) {
            return nullptr;
        }
        constexpr size_t NEXT_DIM = (DIM + 1) % this->KeySize;
        //std::vector<std::pair<Key, Value>>::iterator iter_1 = data.begin();
        auto m = data.begin() + (data.size() - 1) / 2; //choose the left element when the size is even 
        std::nth_element(data.begin(), m, data.end(), this->compareData_DIM<DIM>);
        std::vector<std::pair<Key,Value>> left;
        std::vector<std::pair<Key,Value>> right;
        auto median = data[(data.size() - 1) / 2]; // set the median element

        //get left
        for(size_t i = 0; i < (data.size() - 1) / 2 ; i++) {
            left.push_back(data[i]);
        }

        //get right
        for(size_t i = ((data.size() - 1) / 2) + 1; i < data.size(); i++) {
            right.push_back(data[i]);
        }

        //free the original vector since it has been divided
        data.clear();

        //create a new node as the root node
        Node * node = new Node(median.first, median.second, parent);
        node->left = build_helper<NEXT_DIM>(left, node);
        node->right = build_helper<NEXT_DIM>(right, node);
        return node;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        // TODO: implement this function
        this->root = copy_helper(that.root, nullptr);
        this->treeSize = that.treeSize;
    }

    //deep copy the tree rooted at the root_node, return the root node of the copied tree
    //parent is the parent of the returned root node
    Node * copy_helper(Node * root_node, Node * parent) {
        if(root_node == nullptr) {
            return nullptr;
        }
        Node * new_node = new Node(root_node->key(), root_node->value(), parent);
        new_node->left = copy_helper(root_node->left, new_node);
        new_node->right = copy_helper(root_node->right, new_node);
        return new_node;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        // TODO: implement this function
        if(that.root == this->root) {
            return *this;
        }
        //if this is not self-assignment, we delete the original root first and then copy the tree
        //update the treeSize
        delete_helper(this->root);
        this->root = copy_helper(that.root,nullptr);
        this->treeSize = that.treeSize;
        return *this;
    }

    //delete the tree rooted at victim
    void delete_helper(Node * victim) {
        if(victim == nullptr) {
            return;
        }
        delete_helper(victim->left);
        delete_helper(victim->right);
        victim->left = nullptr;
        victim->right = nullptr;
        victim->parent = nullptr;
        delete victim;
        return;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        // TODO: implement this function'
        delete_helper(this->root);
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};
