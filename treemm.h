#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
public:
    class Iterator
    {
      public:
        Iterator()
        {
            m_pos = -1;
            m_vptr = nullptr;
        }

        Iterator(std::vector<ValueType>& v)
        {
            m_pos = 0; // m_pos tracks which index of vector we are on
            m_vptr = &v;
        }

        ValueType& get_value() const
        {
            return (*m_vptr)[m_pos];
        }

        bool is_valid() const
        {
            return m_pos >= 0 && m_pos < m_vptr->size();
        }

        void advance()
        {
            m_pos++;
        }

      private:
          int m_pos;
          std::vector<ValueType>* m_vptr;
    };

    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        deleteTreeHelper(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (m_root == nullptr)
        {
            m_root = new Node(key);
            m_root->addToNode(value);
            return;
        }
        Node* cur = m_root;
        for (;;)
        {
            if (key == cur->key)
            {
                cur->addToNode(value);
                return;
            }

            if (key < cur->key)
            {
                if (cur->left != nullptr)
                    cur = cur->left;
                else
                {
                    cur->left = new Node(key);
                    cur->left->addToNode(value);
                    return;
                }
            }

            else if (key > cur->key)
            {
                if (cur->right != nullptr)
                    cur = cur->right;
                else
                {
                    cur->right = new Node(key);
                    cur->right->addToNode(value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* cur = m_root;
        while(cur != nullptr)
        {
            if (key == cur->key)
                return Iterator(cur->v); // pass the vector of values in the node to Iterator
            else if (key < cur->key)
                cur = cur->left;
            else
                cur = cur->right;
        }        
        return Iterator();  
    }

private:
    
    struct Node
    {
        Node(const KeyType& key_to_add)
        {
            key = key_to_add;
            left = right = nullptr;
        }
        void addToNode(const ValueType& value) { v.push_back(value); }
        KeyType key;
        std::vector<ValueType> v;
        Node* left, * right;
    };
    
    Node* m_root;

    void deleteTreeHelper(Node* cur) // helper recursive function to delete tree
    {
        if (cur == nullptr)
            return;
        deleteTreeHelper(cur->left);
        deleteTreeHelper(cur->right);
        delete cur;
    }
};

#endif // TREEMULTIMAP_INCLUDED
