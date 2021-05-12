//
//  main.cpp
//  algorithm_redblacktree
//
//  Created by 최희연 on 2021/05/10.
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

const int black = 0;
const int red = 1;

class medicalRecord{    //진료기록을 담는 클래스
public:
    string sickness;
    int expense;
    medicalRecord(string sickness, int expense){
        this->sickness = sickness;
        this->expense = expense;
    }
};

class patient{  // 환자 정보를 담는 클래스
public:
    int patientNumber;
    string name;
    string phoneNumber;
    pair<int, int> address;
    stack<medicalRecord> mr;
    patient(int n, string name, string pN, int ax, int ay){
        this->patientNumber = n;
        this->phoneNumber = pN;
        this->address.first = ax;
        this->address.second = ay;
    }
    patient();
};

class node{ //red black tree를 위한 노드 클래스
public:
    int b;  // black or red
    int depth;
    node *l, *r, *parent;
    patient* p;
    node(int bb, int nn, string name, string pN, int ax, int ay, string sick, int expense, class node* ll, class node* rr, class node* pp){
        b=bb;
        p = new patient(nn, name, pN, ax, ay);
        l = ll;
        r = rr;
        parent = pp;
        depth = 0;
        medicalRecord mR = medicalRecord(sick, expense);
        p->mr.push(mR);
    }
    node(){}
    
};

class redBlackTree{
    node* root;
    int treeSize;
    
public:
    redBlackTree(){
        treeSize = 0;
        root = nullptr;
    }
    
    node* getRoot(){
        return this->root;
    }
    
    // 환자 추가
    node* insert(int n, string name, string pN, int ax, int ay, string sick, int expense){
        
        if(search(n) != nullptr)
            return nullptr;
        
        
        else{
            if(root == nullptr){    //root가 null이면 트리에 아무것도 없으므로 블랙으로 생성
                root = new node(black, n, name, pN, ax, ay, sick, expense, nullptr, nullptr, nullptr);
                root->depth = 0;
                return root;
                
            }
            
            else{
                auto nowNode = getRoot();
                node* newNode = new node(red, n, name, pN, ax, ay, sick, expense, nullptr, nullptr, nullptr);
                while(nowNode != nullptr){
                    if(nowNode->p->patientNumber > n){
                        if(nowNode->l==nullptr){
                            nowNode->l = newNode;
                            newNode->b = red;
                            newNode->parent = nowNode;
                            break;
                        }
                        else
                            nowNode = nowNode->l;
                        
                    }
                    else{   //nowNode->p->patientNumber < n
                        if(nowNode->r==nullptr){
                            nowNode->r = newNode;
                            newNode->b = red;
                            newNode->parent = nowNode;
                            break;
                        }
                        else
                            nowNode = nowNode->r;
                    }
                }
                fixup(newNode);
                newNode->depth = newNode->parent->depth + 1;
                return newNode;
            }
        }
        
    }
    
    void fixup(node* z){
        while(z->parent->b == red){
            node* grand = z->parent->parent;
            node* uncle;
            if(grand->l == z->parent){  // Left에서 규칙위반
                if(grand->r){
                    uncle = grand->r;
                    if(uncle->b == red){    //recoloring
                        z->parent->b = black;
                        grand->b = red;
                        uncle->b = black;
                        if(grand->p->patientNumber!=root->p->patientNumber)
                            z = grand;
                        else
                            break;
                    }
                    else if(z == grand->l->r){  //LR에서 규칙위반
                        leftRotation(z->parent);
                        rightRotation(grand);
                        grand->b = black;
                        grand->l->b = red;
                        grand->r->b = red;
                        
                    }
                    else{   //LL에서 규칙 위반
                        rightRotation(z->parent);
                        z->b = red;
                        z->parent->b = black;
                        z->parent->r->b = red;
                    }
                    
                }
                else{
                    z->parent->b = black;
                    z->parent->parent->b = red;
                }
                
            }
            
            else{   //R에서 규칙 위반
                if(grand->l){
                    uncle = grand->l;
                    if(uncle->b == red){    //recoloring
                        z->parent->b = black;
                        grand->b = red;
                        uncle->b = black;
                        if(grand->p->patientNumber!=root->p->patientNumber)
                            z = grand;
                        else
                            break;
                    }
                    else if(z == grand->r->r){  //RR에서 규칙위반
                        leftRotation(z->parent);
                        z->b = red;
                        z->parent->b = black;
                        z->parent->l->b = red;
                        
                    }
                    else{   //RL에서 규칙 위반
                        rightRotation(z->parent);
                        leftRotation(z);
                        z->b = red;
                        z->parent->b = black;
                        z->parent->l->b = red;
                        
                    }
                }
                else{
                    z->parent->b = black;
                    z->parent->parent->b = black;
                }
            }
        }
        root->b = black;
    }
    
    void leftRotation(node* v){
        node* nw_node = new node();
        if(v->r->l)
            nw_node->r = v->r->l;
        nw_node->l = v->l;
        nw_node->p->patientNumber = v->p->patientNumber;
        nw_node->b = v->b;
        v->p = v->r->p;
        
        v->l = nw_node;
        if(nw_node->l)
            nw_node->l->parent = nw_node;
        if(nw_node->r)
            nw_node->r->parent = nw_node;
        nw_node->parent = v;
        
        if(v->r->r)
            v->r = v->r->r;
        else
            v->r = nullptr;
        
        if(v->r)
            v->r->parent = v;
    }
    
    void rightRotation(node* v){
        node* newNode = new node();
        if(v->l->r)
            newNode->l = v->l->r;
        newNode->r = v->r;
        newNode->p = v->p;
        newNode->b = v->b;
        
        v->p = v->l->p;
        v->b = v->l->b;
        
        v->r = newNode;
        if(newNode->l)
            newNode->l->parent = newNode;
        if(newNode->r)
            newNode->r->parent = newNode;
        newNode->parent = v;
        
        if(v->l->l)
            v->l = v->l->l;
        else
            v->l = nullptr;
        
        if(v->l)
            v->l->parent = v;
    }
    
    node* search(int patient_number){
        node* nowNode = root;
        while(nowNode!=nullptr){
            if(patient_number == nowNode->p->patientNumber){
                return nowNode;
            }
            else if(patient_number < nowNode->p->patientNumber){
                nowNode = nowNode->l;
                continue;
            }
            else{
                nowNode = nowNode->r;
                continue;
            }
        }
        return nullptr; // 못찾았을 경우
    }
    
    int epidemic(string d, node* nowNode, int cnt){
        if(nowNode == nullptr)
            return cnt;
        if(nowNode->p->mr.top().sickness == d)
            cnt++;
        if(nowNode->l)
            epidemic(d, nowNode->l, cnt);
        if(nowNode->r)
            epidemic(d, nowNode->r, cnt);
        return cnt;
        
    }
    
    int addDisease(int k, string d, int c){
        node* find = search(k);
        if(find==nullptr)
            return 0;
        else{
            find->p->mr.push(medicalRecord(d, c));
            return find->depth;
        }
    }
    
};

int main(int argc, const char * argv[]) {
    int T;
    cin>>T;
    redBlackTree* rbt = new redBlackTree();
    while(T--){
        char type; // I:신규가입, F:검색, A:추가진료, E:유행병 조사
        cin>>type;
        if(type == 'I'){
            int k;
            string n;
            string h;
            int ax, ay;
            string di;
            int c;
            cin>>k>>n>>h>>ax>>ay>>di>>c;
            node* returnValue = rbt->insert(k, n, h, ax, ay, di, c);
            if(returnValue == nullptr){
                cout<<rbt->search(k)->depth<<' '<<0<<endl;
            }
            else{
                cout<<returnValue->depth<<' '<<1<<endl;
            }
        }
        else if(type=='F'){
            int k;
            cin>>k;
            node* returnValue = rbt->search(k);
            if(returnValue == nullptr)
                cout<<"Not found"<<endl;
            else
                cout<<returnValue->depth<<' '<<returnValue->p->name<<' '<<returnValue->p->phoneNumber<<' '<<returnValue->p->address.first<<' '<<returnValue->p->address.second<<endl;
        }
        else if(type == 'A'){
            int k;
            string di;
            int c;
            cin>>k>>di>>c;
            if(rbt->search(k)==nullptr)
                cout<<"Not found"<<endl;
            else
                cout<<rbt->addDisease(k, di, c)<<endl;
        }
        else if(type == 'E'){
            string di;
            cin>>di;
            cout<<rbt->epidemic(di, rbt->getRoot(), 0);
        }
        
        
    }
}
