#ifndef COVIDVISUALIZATION_BPLUSTREE_H
#define COVIDVISUALIZATION_BPLUSTREE_H

#include "Key.h"
#include <iostream>
#include <queue>



class BplusTree{
    //3 children
    //2 keys for non leaf nodes
    //5 keys for leaf nodes
private:
    struct TreeNode{
    private:
        bool leaf;
    public:
        std::vector<Key*> keys;
        TreeNode* left;
        TreeNode* middle;
        TreeNode* right;

        TreeNode();

        ~TreeNode();

        TreeNode(bool isleaf);

        int GetKeysSize();

        bool IsLeaf();

        void RemoveFirst();

        void AddKey(Key* newKey);

        void InsertKey(Key* newKey);
    };
    //---------------------------------END OF TREENODE STRUCT------------------------------
    TreeNode* root;

public:
    BplusTree();

    ~BplusTree();

    void InsertKey(Key* newKey);

    TreeNode* Insert(TreeNode* root,Key* newKey);

    void InsertData(const std::vector<Key*>& keys);

    void LevelOrderTranserse();

    void Transverse();

    Key* FindKey(const std::string& date);
};

#endif //COVIDVISUALIZATION_BPLUSTREE_H
