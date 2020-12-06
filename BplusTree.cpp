#include "BplusTree.h"

BplusTree::TreeNode::TreeNode(){
    leaf = true;
}

BplusTree::TreeNode::TreeNode(bool isleaf){
    leaf = isleaf;
    //leaf nodes don't need pointers
    if(!leaf){
        left = nullptr;
        right = nullptr;
        middle = nullptr;
    }
}

BplusTree::TreeNode::~TreeNode(){
    if(leaf) {
        for (auto & key : keys) {
            key->left = nullptr;
            key->right =  nullptr;
        }
    }
}

int BplusTree::TreeNode::GetKeysSize(){
    return keys.size();
}

bool BplusTree::TreeNode::IsLeaf(){
    return leaf;
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

void BplusTree::TreeNode::InsertKey(Key* newKey){
    //insert a key into the leaf linked list
    //Change pointers and positions
    if(keys.empty()){
        keys.push_back(newKey);
        return;
    }

    Key* tempKey;
    for(auto & key : keys){
        if(newKey->GetDate() < key->GetDate()){
            //swapping left and right pointers appropriately
            newKey->right = key;
            newKey->left = key->left;
            if(key->left !=nullptr){
                key->left->right = newKey;
            }
            key->left = newKey;

            //swapping keys in vector appropriately
            tempKey = key;
            key = newKey;
            newKey = tempKey;
        }
    }
    //swapping left and right pointers appropriately
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
            //recent = NULL;
        }
        else{
            queue.push(recent->left);
            queue.push(recent->middle);
            queue.push(recent->right);
            delete recent;
            //recent = NULL;
        }
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
            root->InsertKey(newKey);
            //size is now 6 so we are moving half of the keys to the newLeafNode
            for(int i = 0; i < 3;i++){
                Key* tempKey = root->keys.back();
                newLeafNode->AddKey(tempKey);
                root->keys.pop_back();
            }
            return newLeafNode;
        }
        else{
            root->InsertKey(newKey);
        }
    }
    return root;
}

void BplusTree::InsertData(const std::vector<Key*>& keys){
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
    Key* allKeys = toLeafNode->keys[0];
    while(allKeys != nullptr){
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << allKeys->GetDate() << std::endl;
        allKeys = allKeys->right;
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
    Key* findKey = findLeafNode->keys[0];
    while(findKey != nullptr && date >= findKey->GetDate()){
        if(date == findKey->GetDate()){
            return findKey;
        }
        findKey = findKey->right;
    }
    return nullptr;
}