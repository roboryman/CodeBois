#ifndef COVIDVISUALIZATION_BPLUSTREE_H
#define COVIDVISUALIZATION_BPLUSTREE_H

#include "Key.h"
#include <chrono>
#include <thread>
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

        TreeNode(){
            leaf = true;
        }

        TreeNode(bool isleaf){
            leaf = isleaf;
            //leaf nodes don't need pointers
            if(!leaf){
                left = nullptr;
                right = nullptr;
                middle = nullptr;
            }
        }

        int getKeysSize(){
            return keys.size();
        }

        bool isLeaf(){
            return leaf;
        }

        void removeFirst(){
            if(!leaf){
                keys[0] = keys[1];
                keys.pop_back();
                //removes first key in nonleaf TreeNode
            }
        }

        void addKey(Key* newKey){
            //inserting key into treenode not linked list
            //Change positions only
            for(int i = 0; i < keys.size();i++){
                if(newKey->getDate() < keys[i]->getDate()){
                    Key* tempKey = keys[i];
                    keys[i] = newKey;
                    newKey = tempKey;
                }
            }
            keys.push_back(newKey);
        }

        void insertKey(Key* newKey){
            //insert a key into the leaf linked list
            //Change pointers and positions
            if(keys.size() == 0){
                keys.push_back(newKey);
                return;
            }
            Key* tempKey;
            for(int i = 0; i < keys.size();i++){
                if(newKey->getDate() < keys[i]->getDate()){
                    //swapping left and right pointers appropraitely
                    newKey->right = keys[i];
                    newKey->left = keys[i]->left;
                    if(keys[i]->left !=nullptr){
                        keys[i]->left->right = newKey;
                    }
                    keys[i]->left = newKey;

                    //swapping keys in vector appropraitely
                    tempKey = keys[i];
                    keys[i] = newKey;
                    newKey = tempKey;
                }
            }
            //swapping left and right pointers appropraitely
            newKey->left = keys[keys.size()-1];
            if(newKey->right == nullptr){
              newKey->right = keys[keys.size()-1]->right;
              if(keys[keys.size()-1]->right != nullptr){
                keys[keys.size()-1]->right->left = newKey;
              }
            }
            keys[keys.size()-1]->right = newKey;
            //add key into Treenode
            keys.push_back(newKey);
        }
    };
    //---------------------------------END OF TREENODE STRUCT------------------------------
    TreeNode* root;

public:
    BplusTree(){
        root = nullptr;
    }

    Key* makeKey(std::string date){
        Key* newKey = new Key(date);
        return newKey;
    }

    void insertKey(Key* newKey){
        //Helper function
        if(root == nullptr){
            root = new TreeNode(true);
            root = Insert(root,newKey);
        }
        else{
            TreeNode * tempNode = Insert(root,newKey);
            if(tempNode !=root){
                //split occured
                TreeNode* newRoot = new TreeNode(false);
                newRoot->addKey(tempNode->keys[0]);
                newRoot->left = root;
                newRoot->middle = tempNode;
                root = newRoot;
                tempNode->removeFirst();
            }
        }
    }

    TreeNode* Insert(TreeNode* root,Key* newKey){
        if(!root->isLeaf()){
            if(newKey->getDate() < root->keys[0]->getDate()){
                //left pointer
                TreeNode* newLeft = Insert(root->left,newKey);
                if(newLeft != root->left){
                    //split occured
                    if(root->getKeysSize() == 2){
                        //Too many pointers New TreeNode needed
                        TreeNode* newTreeNode = new TreeNode(false);
                        //Changing pointers to TreeNodes
                        newTreeNode->left = root->middle;
                        newTreeNode->middle = root->right;
                        root->right = nullptr;
                        root->middle = newLeft;
                        //Changing keys
                        newTreeNode->addKey(root->keys[0]);
                        root->keys[0] = newLeft->keys[0];
                        newLeft->removeFirst();
                        newTreeNode->addKey(root->keys[1]);
                        root->keys.pop_back();
                        return newTreeNode;
                    }
                    else{
                        //We can add a TreeNode to one of our pointers
                        root->addKey(newLeft->keys[0]);

                        root->right = root->middle;
                        root->middle = root->left;
                        root->left = newLeft;
                        newLeft->removeFirst();
                    }
                }

            }
            else{
                if(root->getKeysSize() == 2){
                    if(newKey->getDate() >= root->keys[1]->getDate()){
                        //right
                        //pointer already full need new TreeNode
                        TreeNode* newRight = Insert(root->right,newKey);
                        if(newRight != root->right){
                            //split occured
                            //Too many pointers New TreeNode needed
                            TreeNode* newTreeNode = new TreeNode(false);
                            //Changing pointers to TreeNodes
                            newTreeNode->left = root->right;
                            newTreeNode->middle = newRight;
                            root->right = nullptr;
                            //Changing keys
                            newTreeNode->addKey(root->keys[1]);
                            newTreeNode->addKey(newRight->keys[0]);
                            root->keys.pop_back();
                            newRight->removeFirst();
                            return newTreeNode;
                        }
                    }
                }
                else{
                    //middle pointer
                    TreeNode* newMiddle = Insert(root->middle,newKey);
                    if(newMiddle != root->middle){
                        //split occured
                        if(root->getKeysSize() == 2){
                            //Too many pointers New TreeNode needed
                            TreeNode* newTreeNode = new TreeNode(false);
                            //Changing pointers to TreeNodes
                            newTreeNode->left = newMiddle;
                            newTreeNode->middle = root->right;
                            root->right = nullptr;
                            //Changing keys
                            newTreeNode->addKey(newMiddle->keys[0]);
                            newTreeNode->addKey(root->keys[1]);
                            root->keys.pop_back();
                            newMiddle->removeFirst();
                            return newTreeNode;
                        }
                        else{
                            //no split needed right pointer add to it
                            root->addKey(newMiddle->keys[0]);
                            root->right = newMiddle;
                            newMiddle->removeFirst();
                        }
                    }
                }
            }
        }

        else{
            if(root->getKeysSize() == 5){
                //split needs to occur  leaf node needed
                TreeNode* newLeafNode = new TreeNode(true);
                root->insertKey(newKey);
                //size is now 6 so we are moving half of the keys to the newLeafNode
                for(int i = 0; i < 3;i++){
                    Key* tempKey = root->keys.back();
                    newLeafNode->addKey(tempKey);
                    root->keys.pop_back();
                }
                return newLeafNode;
            }
            else{
                root->insertKey(newKey);
            }
        }
        return root;
    }

    void insertData(const std::vector<Key*>& keys){
        for(int i = 0;i < keys.size(); i++){
            insertKey(keys.at(i));
        }
    }

    void LevelOrderTranserse(){
        std::queue<TreeNode*> queue;
        if(root == nullptr){
            return;
        }
        else{
            queue.push(root);
            while(!queue.empty()){
                TreeNode* recent = queue.front();
                queue.pop();
                if(recent == nullptr){
                    continue;
                }
                if(recent->isLeaf()){
                    std::cout << "leaf with keys: ";
                    for(int i = 0;i < recent->getKeysSize();i++){
                        std::cout << recent->keys[i]->getDate() << " ";
                    }
                    
                    std::cout << recent << " ";
                    if(recent->left != nullptr) {
                        std::cout << recent->left << " ";
                    }
                    if(recent->middle != nullptr) {
                        std::cout << recent->middle << " ";
                    }
                    if(recent->right != nullptr) {
                        std::cout << recent->right << " ";
                    }
                    std::cout << std::endl;
                }
                else{
                    std::cout << "nonLeaf with keys: ";
                    for(int i = 0;i < recent->getKeysSize();i++){
                        std::cout << recent->keys[i]->getDate() << " ";
                    }
                    std::cout << recent << " ";
                    std::cout << recent->left << " ";
                    std::cout << recent->middle << " ";
                    if(recent->right != nullptr) {
                        std::cout << recent->right << " ";
                    }
                    std::cout << std::endl;
                    queue.push(recent->left);
                    queue.push(recent->middle);
                    queue.push(recent->right);

                }
            }
        }
    }

    void Transverse(){
        //print out all keys
        TreeNode* toLeafNode = root;
        while(toLeafNode->isLeaf() == false){
            toLeafNode = toLeafNode->left;
        }
        Key* allKeys = toLeafNode->keys[0];
        while(allKeys != nullptr){
          //std::this_thread::sleep_for(std::chrono::seconds(1));
          std::cout << allKeys->getDate() << std::endl;
          allKeys = allKeys->right;
        }
    }

    Key* findKey(std::string date){
        if(root == nullptr){
            return nullptr;
        }
        TreeNode* findLeafNode = root;
        while(!findLeafNode->isLeaf()){
            if(date < findLeafNode->keys[0]->getDate()){
                findLeafNode = findLeafNode->left;
            }
            else{
                if(findLeafNode->getKeysSize() == 2){
                  if(date >= findLeafNode->keys[1]->getDate()){
                      findLeafNode = findLeafNode->right;
                  }
                  else{
                    findLeafNode = findLeafNode->middle;
                  }
                }
                else{
                    findLeafNode = findLeafNode->middle;
                }
            }
        }
        Key* findKey = findLeafNode->keys[0];
        while(findKey != nullptr && date >= findKey->getDate()){
            if(date == findKey->getDate()){
                return findKey;
            }
            findKey = findKey->right;
        }
        return nullptr;
    }
};//done

#endif //COVIDVISUALIZATION_BPLUSTREE_H
