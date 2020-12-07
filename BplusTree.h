#ifndef COVIDVISUALIZATION_BPLUSTREE_H
#define COVIDVISUALIZATION_BPLUSTREE_H

#include "Key.h"
#include <iostream>
#include <queue>



class BplusTree{
    //3 children
    //2 keys for non leaf nodes
    //5 keys for leaf nodes
public:
    struct TreeNode{
    private:
        bool leaf;
    public:
        std::vector<Key*> keys;
        TreeNode* left;
        TreeNode* middle;
        TreeNode* right;

        TreeNode();

        TreeNode(bool isleaf);

        int GetKeysSize();

        bool KeyExist(const std::string& date);

        Key* FindKey(const std::string& date);

        bool IsLeaf();

        TreeNode* GetNext();

        std::vector<Key*>* GetKeys();

        void RemoveFirst();

        void AddKey(Key* newKey);
    };
    //---------------------------------END OF TREENODE STRUCT------------------------------

    BplusTree();

    ~BplusTree();

    TreeNode* GetFirstTreeNode();

    void InsertKey(Key* newKey);

    TreeNode* Insert(TreeNode* root,Key* newKey);

    void InsertData(std::vector<Key*> keys);

    void LevelOrderTranserse();

    void Transverse();

    Key* FindKey(const std::string& date);
private:
    TreeNode* root;
};

#endif //COVIDVISUALIZATION_BPLUSTREE_H
