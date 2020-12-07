#include "BplusTree.h"

BplusTree::TreeNode::TreeNode(){
    leaf = true;
    left = nullptr;
    right = nullptr;
}

BplusTree::TreeNode::TreeNode(bool isleaf){
    leaf = isleaf;
    left = nullptr;
    right = nullptr;
    //leaf nodes don't need middle pointer
    if(!leaf){
        middle = nullptr;
    }
}

int BplusTree::TreeNode::GetKeysSize(){
    return keys.size();
}

bool BplusTree::TreeNode::KeyExist(const std::string& date){
    for(int i = 0; i < GetKeysSize();i++){
        if(keys.at(i)->GetDate() == date){
            return true;
        }
    }
    return false;
}

Key* BplusTree::TreeNode::FindKey(const std::string& date){
    for(int i = 0; i < GetKeysSize();i++){
        if(keys.at(i)->GetDate() == date){
            return keys.at(i);
        }
    }
    return nullptr;
}

bool BplusTree::TreeNode::IsLeaf(){
    return leaf;
}

std::vector<Key*>* BplusTree::TreeNode::GetKeys(){
    return &keys;
}

void BplusTree::TreeNode::RemoveFirst(){
    if(!leaf){
        keys[0] = keys[1];
        keys.pop_back();
        //removes first key in nonleaf TreeNode
    }
}

void BplusTree::TreeNode::AddKey(Key* newKey){
    //inserting key into treenode not linked list
    //Change positions only
    for(auto & key : keys){
        if(newKey->GetDate() < key->GetDate()){
            Key* tempKey = key;
            key = newKey;
            newKey = tempKey;
        }
    }
    keys.push_back(newKey);
}

BplusTree::TreeNode* BplusTree::TreeNode::GetNext() {
    return right;
}

//---------------------------------END OF TREENODE STRUCT------------------------------

BplusTree::BplusTree(){
    root = nullptr;
}

BplusTree::~BplusTree(){
    std::queue<TreeNode*> queue;
    queue.push(root);
    while(!queue.empty()){
        TreeNode* recent = queue.front();
        queue.pop();
        if(recent == nullptr){
            continue;
        }
        if(recent->IsLeaf()){
            delete recent;
        }
        else{
            queue.push(recent->left);
            queue.push(recent->middle);
            queue.push(recent->right);
            delete recent;
        }
    }

}

BplusTree::TreeNode* BplusTree::GetFirstTreeNode(){
    if(root == nullptr){
        return nullptr;
    }
    else{
        TreeNode* toLeafNode = root;
        while(!toLeafNode->IsLeaf()){
            toLeafNode = toLeafNode->left;
        }
        return toLeafNode;
    }
}

void BplusTree::InsertKey(Key* newKey){
    //Helper function
    if(root == nullptr){
        root = new TreeNode(true);
        root = Insert(root,newKey);
    }
    else{
        TreeNode * tempNode = Insert(root,newKey);
        if(tempNode !=root){
            //split occurred
            auto* newRoot = new TreeNode(false);
            newRoot->AddKey(tempNode->keys[0]);
            newRoot->left = root;
            newRoot->middle = tempNode;
            root = newRoot;
            tempNode->RemoveFirst();
        }
    }
}

BplusTree::TreeNode* BplusTree::Insert(TreeNode* root,Key* newKey){
    if(!root->IsLeaf()){
        if(newKey->GetDate() < root->keys[0]->GetDate()){
            //left pointer
            TreeNode* newLeft = Insert(root->left,newKey);
            if(newLeft != root->left){
                //split occurred
                if(root->GetKeysSize() == 2){
                    //Too many pointers New TreeNode needed
                    auto* newTreeNode = new TreeNode(false);
                    //Changing pointers to TreeNodes
                    newTreeNode->left = root->middle;
                    newTreeNode->middle = root->right;
                    root->right = nullptr;
                    root->middle = newLeft;
                    //Changing keys
                    newTreeNode->AddKey(root->keys[0]);
                    root->keys[0] = newLeft->keys[0];
                    newLeft->RemoveFirst();
                    newTreeNode->AddKey(root->keys[1]);
                    root->keys.pop_back();
                    return newTreeNode;
                }
                else{
                    //We can add a TreeNode to one of our pointers
                    root->AddKey(newLeft->keys[0]);

                    root->right = root->middle;
                    root->middle = root->left;
                    root->left = newLeft;
                    newLeft->RemoveFirst();
                }
            }

        }
        else{
            if(root->GetKeysSize() == 2){
                if(newKey->GetDate() >= root->keys[1]->GetDate()){
                    //right
                    //pointer already full need new TreeNode
                    TreeNode* newRight = Insert(root->right,newKey);
                    if(newRight != root->right){
                        //split occurred
                        //Too many pointers New TreeNode needed
                        auto* newTreeNode = new TreeNode(false);
                        //Changing pointers to TreeNodes
                        newTreeNode->left = root->right;
                        newTreeNode->middle = newRight;
                        root->right = nullptr;
                        //Changing keys
                        newTreeNode->AddKey(root->keys[1]);
                        newTreeNode->AddKey(newRight->keys[0]);
                        root->keys.pop_back();
                        newRight->RemoveFirst();
                        return newTreeNode;
                    }
                }
            }
            else{
                //middle pointer
                TreeNode* newMiddle = Insert(root->middle,newKey);
                if(newMiddle != root->middle){
                    //split occurred
                    if(root->GetKeysSize() == 2){
                        //Too many pointers New TreeNode needed
                        auto* newTreeNode = new TreeNode(false);
                        //Changing pointers to TreeNodes
                        newTreeNode->left = newMiddle;
                        newTreeNode->middle = root->right;
                        root->right = nullptr;
                        //Changing keys
                        newTreeNode->AddKey(newMiddle->keys[0]);
                        newTreeNode->AddKey(root->keys[1]);
                        root->keys.pop_back();
                        newMiddle->RemoveFirst();
                        return newTreeNode;
                    }
                    else{
                        //no split needed right pointer add to it
                        root->AddKey(newMiddle->keys[0]);
                        root->right = newMiddle;
                        newMiddle->RemoveFirst();
                    }
                }
            }
        }
    }

    else{
        if(root->GetKeysSize() == 5){
            //split needs to occur  leaf node needed
            auto* newLeafNode = new TreeNode(true);
            root->AddKey(newKey);
            //size is now 6 so we are moving half of the keys to the newLeafNode
            for(int i = 0; i < 3;i++){
                Key* tempKey = root->keys.back();
                newLeafNode->AddKey(tempKey);
                root->keys.pop_back();
            }
            newLeafNode->right = root->right;
            newLeafNode->left = root;
            root->right = newLeafNode;
            if(newLeafNode->right != nullptr){
                newLeafNode->right->left = newLeafNode;
            }

            return newLeafNode;
        }
        else{
            root->AddKey(newKey);
        }
    }
    return root;
}

void BplusTree::InsertData(std::vector<Key*> keys){
    for(auto & key : keys){
        InsertKey(key);
    }
}

void BplusTree::LevelOrderTranserse(){
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
            if(recent->IsLeaf()){
                std::cout << "leaf with keys: ";
                for(int i = 0;i < recent->GetKeysSize();i++){
                    std::cout << recent->keys[i]->GetDate() << " ";
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
                for(int i = 0;i < recent->GetKeysSize();i++){
                    std::cout << recent->keys[i]->GetDate() << " ";
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

void BplusTree::Transverse(){
    //print out all keys
    TreeNode* toLeafNode = root;
    while(!toLeafNode->IsLeaf()){
        toLeafNode = toLeafNode->left;
    }

    while(toLeafNode != nullptr){
        for(int j = 0; j < toLeafNode->GetKeysSize();j++) {
            Key* currentKey = toLeafNode->GetKeys()->at(j);
            int counties = 0;
            for (int i = 0; i < currentKey->GetStates()->size(); i++) {
                counties += currentKey->GetStates()->at(i)->GetCounties()->size();
            }
            std::cout << currentKey->GetDate() << std::endl;
            std::cout << "Amount of States:" << currentKey->GetStates()->size();
            std::cout << " , Amount of Counties: " << counties << std::endl;
        }
        toLeafNode = toLeafNode->GetNext();
    }
}

Key* BplusTree::FindKey(const std::string& date){
    if(root == nullptr){
        return nullptr;
    }
    TreeNode* findLeafNode = root;
    while(!findLeafNode->IsLeaf()){
        if(date < findLeafNode->keys[0]->GetDate()){
            findLeafNode = findLeafNode->left;
        }
        else{
            if(findLeafNode->GetKeysSize() == 2){
                if(date >= findLeafNode->keys[1]->GetDate()){
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

    for(int i = 0;i < findLeafNode->GetKeysSize();i++){
        if (findLeafNode->GetKeys()->at(i)->GetDate() == date){
            return findLeafNode->GetKeys()->at(i);
        }
    }

    return nullptr;
}