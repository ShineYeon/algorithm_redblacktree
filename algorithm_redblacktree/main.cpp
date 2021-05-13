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
    node* leafNode;
    int treeSize;
    
public:
    redBlackTree(){
        leafNode = new node;
        treeSize = 0;
        leafNode->b = black;
        leafNode->l = nullptr;
        leafNode->r = nullptr;
        leafNode->parent=nullptr;
        root = leafNode;
    }
    
    node* getRoot(){
        return this->root;
    }
    
    // 환자 추가
    node* insert(int n, string name, string pN, int ax, int ay, string sick, int expense){
        
        //
        node* x = this->root;   // 삽입할 곳을 찾기 위함
        node* y = nullptr;  // 삽입할 곳의 부모노드
        node* z = search(n);
        
        if(z != nullptr)    // 해당 환자가 이미 존재할 경우 nullptr반환
            return nullptr;
        
        
        else{   // 존재하지 않을 경우 삽입 연산 수행
            z = new node(red, n, name, pN, ax, ay, sick, expense, leafNode, leafNode, nullptr);
            while(x!=leafNode){
                y = x;
                
                if(x->p->patientNumber < n)
                    x = x->r;
                else
                    x = x->l;
            }
            
            z->parent = y;
            
            if(y == nullptr)
                root = z;
            else if(z->p->patientNumber > y->p->patientNumber)
                y->r = z;
            else
                y->l=z;
            
            if(z->parent == nullptr){
                z->b = black;
                if(z == root)
                    z->depth = 0;
                else
                    z->depth = (z->parent->depth) + 1;
                return z;
            }
            
            if(z->parent->parent == nullptr){
                if(z == root)
                    z->depth = 0;
                else
                    z->depth = (z->parent->depth) + 1;
                return z;
            }
            fixup(z);
            if(z == root)
                z->depth = 0;
            else
                z->depth = (z->parent->depth) + 1;
            
            return z;
            
            
        }
        
        
    }
    
    
    void fixup(node* z){
        while(z!=root && z->parent->b == red){
            node* grand = z->parent->parent;
            node* uncle = (z->parent == grand->l) ? grand->r : grand->l;
            bool side = (z->parent == grand->l) ? true : false;
            //부모가 할아버지의 왼쪽자식이면 1, 오른쪽 자식이면 0
            
            if (uncle && uncle->b == red) {
                z->parent->b = black;
                uncle->b = black;
                grand->b = red;
                z = grand;
            }
            
            else {
                /*case 2-1*/
                if (z == (side ? z->parent->r : z->parent->l)) {
                    z = z->parent;
                    side ? leftRotation(z) : rightRotation(z);
                }
                /*case 2-2*/
                z->parent->b = black;
                grand->b = red;
                side ? rightRotation(grand) : leftRotation(grand);
            }
        }
        
        root->b = black;
    }
    
    void leftRotation(node* v) {
        node* w;
        
        w = v->r;
        v->r = w->l;
        if (w->l != leafNode) {
            w->l->parent = v;
        }
        w->parent = v->parent;
        
        if (!v->parent) {
            root = w;
        } else if (v == v->parent->l) {
            v->parent->l = w;
        } else {
            v->parent->r = w;
        }
        v->parent = w;
        v->l = v;
    }
    
    
    void rightRotation(node* v) {
        node* w;
        
        w = v->l;
        v->l = w->r;
        if (w->r != leafNode) {
            w->r->parent = v;
        }
        w->parent = v->parent;
        
        if (!v->parent) {
            root = w;
        } else if (v == v->parent->l) {
            v->parent->l = w;
        } else {
            v->parent->r = w;
        }
        v->parent = w;
        w->r = v;
    }
    
    
    
    node* search(int patient_number){
        
        node* x = root;
        if(x==leafNode)
            return nullptr;
        while(x!=leafNode){
            if(patient_number == x->p->patientNumber)
                return x;
            else if(patient_number<x->p->patientNumber)
                x=x->l;
            else
                x=x->r;
        }
        return nullptr;
    }
    
    int epidemic(string d, node* nowNode, int cnt){
        if(nowNode == nullptr)
            return cnt;
        if(nowNode->p->mr.top().sickness == d)  //
            cnt++;
        if(nowNode->l)
            epidemic(d, nowNode->l, cnt);   //
        if(nowNode->r)
            epidemic(d, nowNode->r, cnt);   //
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

