#include <iostream>

using namespace std;
class Node {
public :
    int data;
    Node * left ;
    Node * right ;
    Node(int data){
        this->data = data;
        left=NULL;
        right=NULL;
    }
    ~Node(){
    }
};
class Tree{
    Node * root; //Pointer of start Element
    public :
    Tree(){
        root=NULL;
    }
    void add( int data){
        //Create Node
        Node * newNode=new Node(data); //newNode->data=data;
        // Build Connections
        // Tree Empty No Root
        if(root == NULL){
            root=newNode;
        }
        // Tree Empty Not Empty
        else{
            Node * current =root; //Jump
            Node * parent  = NULL; //Jump-1
            while(current != NULL ){
                parent=current;//root
                if(data > current ->data  ){
                    current=current->right;//
                }else{
                    current=current->left; //
                }
            }
            // parent -> left=newNode;
            // parent -> right=newNode;
            if(data > parent->data ){
                parent->right=newNode;
            }else{
                parent->left=newNode;
            }

        }
    }
    bool checkDataInTree(int data){
        if(getNodeByData(data) != NULL){
            return true;
        }
        return false;

    }

    int getParentMain(int data){
            Node *node=getNodeByData(data);
            if(node != NULL){
                Node *parent=getParent(node);
                if(parent != NULL){
                    return parent->data;
                }else{
                    return -1; // root
                }
            }else{
                return -5;//NotFound
            }

    }

    int getMaxRightMain(int data){
        Node *node=getNodeByData(data);
        if(node != NULL){
            Node *maxRight=getMaxRight(node);
            return maxRight->data;
        }
        return -1;
    }
    void removeNode( int data ){
        // Helper Functions
        Node * current=getNodeByData(data);
        if (current == NULL){
            return ;
        }
        if(current == root ){
            // ROOT Only
                if(root->left == NULL && root->right == NULL){
                    root=NULL;
                }
                // ROOT & Left Side
                else if(root->right == NULL){
                    root=root->left;
                }
                // ROOT & RightSide
                 else if(root->left == NULL){
                    root=root->right;
                }
                // ROOT & Right & Left Side
                else{
                    Node * newRoot=root->left; //40
                    Node * maxRight=getMaxRight(newRoot);
                    maxRight->right=root->right;
                    root=newRoot;
                }


        }else{
            // Node Not Root
            Node * parent = getParent(current);
            Node * child=NULL;
            // leaf Node don't Have any Child
            if(current->right==NULL && current ->left==NULL){
                child=NULL;
                /*if(current->data > parent->data ){
                    parent->right=NULL;
                }else{
                    parent->left=NULL;
                }*/
            }
            // Left Child Only
            else if(current->right==NULL){
                child= current->left ;
                /*if(current->data > parent->data ){
                    parent->right=child;
                }else{
                    parent->left=child;
                }*/

            }
            // Right Child Only
            else if(current->left==NULL){
                 child= current->right ;
                /*if(current->data > parent->data ){
                    parent->right=child;
                }else{
                    parent->left=child;
                }*/

            }
            // Left & Right Child
            else{
                Node * newparent=current->left;
                Node * maxRight=getMaxRight(newparent);
                maxRight->right=current->right;
                child=newparent;
                /*if(current->data > parent->data ){
                    parent->right=newparent;
                }else{
                    parent->left=newparent;
                }*/
            }
            if(current->data > parent->data ){
                    parent->right=child;
            }
            else{
                    parent->left=child;
            }
        }
        //Delete Node
        delete current ;
    }
    void traverse(){
        cout<<"====================================================="<<endl;
        displayLDR(root);
        cout<<"\n====================================================="<<endl;
    }

    //Lab
    int getMaxDepth(){


    }



    private:

    void displayLDR(Node * node){ //Inorder
        if(node == NULL){ //Base Condition
            return ;
        }
        displayLDR(node->left);
        cout<<node->data<<"  ";
        displayLDR(node->right);
    }
    Node * getMaxRight(Node * current ){
        if(current == NULL){
            return NULL;
        }
        while(current ->right != NULL){
            current =current ->right;
        }
        return current;

    }


    Node * getParent(Node * current ){
        //Node * current =getNodeByData(data);
        //current = root;
        if(current == root){
            return NULL;
        }
        if(current != NULL){
            Node * parent = root;
            while (parent != NULL ){
                if(parent->left == current || parent->right == current){
                    return parent;
                }else{
                    //JUMP
                    if(current->data > parent->data){
                        parent=parent->right;
                    }else{
                        parent=parent->left;
                    }
                }
            }

        }
         return NULL;

    }
    //Search
    Node * getNodeByData(int data){
        Node * current =root; //Jump
        while(current != NULL ){
            if(data ==current ->data ){
                return current ;
            }
            else if(data>current->data){
                current=current->right;
            }
            else{
                current=current->left;
            }
        }
        return NULL;
    }
};
int main()
{
    Tree t;
    t.add(50);
    t.add(40);
    t.add(30);
    t.add(60);
    t.add(70);
    t.add(65);
    t.add(25);
    t.add(45);
    t.add(47);
    t.add(27);
    t.add(35);
    t.add(20);
    t.add(37);
    t.add(39);
    t.add(34);

    //t.add(48);
    //t.add(49);
    /*
    t.getNodeByData(50)!=NULL? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.getNodeByData(45)!=NULL? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.getNodeByData(65)!=NULL? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.getNodeByData(60)!=NULL? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.getNodeByData(21)!=NULL? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.getNodeByData(23)!=NULL? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    */
    t.checkDataInTree(50)!=false? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.checkDataInTree(45)!=false? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.checkDataInTree(65)!=false? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.checkDataInTree(60)!=false? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.checkDataInTree(21)!=false? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    t.checkDataInTree(23)!=false? cout<<"Found"<<endl :cout<<"Not Found"<<endl;
    int parent = t.getParentMain(21);
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;
    parent = t.getParentMain(50);//root
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;
    parent = t.getParentMain(65);//70
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;
    parent = t.getParentMain(45);//40
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;

    cout<<"Max Right : "<<t.getMaxRightMain(40)<<endl;



    t.removeNode(50);
    cout<<"======================================================= "<<endl;
    cout<<"We Delete 50 "<<endl;

    parent = t.getParentMain(50);//NotFound
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;

    parent = t.getParentMain(40);//ROOT
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;


    parent = t.getParentMain(60);//47
    parent == -1 ? cout<<"Root"<<endl: parent == -5 ? cout<<"<<Not Found>>"<<endl:cout<<"Parent = "<<parent<<endl;


    t.traverse();//20  25  27  30  34  35  37  39  40  45  47  60  65  70
    //47
        cout<<"======================================================= "<<endl;
        cout<<"We Delete 47 "<<endl;
        t.removeNode(47);//20  25  27  30  34  35  37  39  40  45 xx  60  65  70
        t.traverse();
    //70.
        cout<<"We Delete 70 "<<endl;
        t.removeNode(70);
        t.traverse(); //20  25  27  30  34  35  37  39  40  45  xx 47  60  65  xx
    //37
        cout<<"We Delete 37 "<<endl;
        t.removeNode(37);
        t.traverse();//20  25  27  30  34  35  xx   39  40  45  xx 47  60  65  xx
    //30
        cout<<"We Delete 30 "<<endl;
        t.removeNode(30);
        t.traverse();//20  25  27  xx  34  35  xx   39  40  45  xx 47  60  65  xx
    cout << "Hello world!" << endl;
    return 0;
}
