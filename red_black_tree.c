//赤黒木関連の関数

#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

#define RED 0
#define BLACK 1

//節点の構造体を定義
typedef struct node_{
    int key;
    int color;
    struct node_ *parent;
    struct node_ *left;
    struct node_ *right;
}*node;

//treeオブジェクトの定義
typedef struct{
    node root;
}*tree;

//メモリ確保

//keyにkをもつ節点要素の作成
node node_new(int k){
    node x;
    NEW(x,1);
    x->key=k;
    x->parent=NULL;
    x->left=NULL;
    x->right=NULL;

    return x;
}

//赤黒木構造を作る関数
tree tree_new(void){
    tree T;
    NEW(T,1);
    T->root=NULL;

    return T;
}

//メモリ解放

//nodeを解放する関数
void free_node(node x){
    free(x);
    return;
}

//巡回

//中間順巡回(ソート順で出力できる)
void inorder_tree_walk(node x){
    if(x!=NULL){
        inorder_tree_walk(x->left);
        printf("%d ",x->key);
        inorder_tree_walk(x->right);
    }
    return;
}

//先行順巡回
void preorder_tree_walk(node x){
    if(x!=NULL){
        printf("%d ",x->key);
        preorder_tree_walk(x->left);
        preorder_tree_walk(x->right);
    }
    return;
}

//後行順巡回
void postorder_tree_walk(node x){
    if(x!=NULL){
        postorder_tree_walk(x->left);
        postorder_tree_walk(x->right);
        printf("%d ",x->key);
    }
    return;
}

//SEARCH O(h)

//xを根とする二分木の中からkeyがkとなっているnodeを返す関数(なければNULL,複数個あったらどれか)
node tree_search(node x, int k){
    if(x==NULL&&x->key==k){
        return x;
    }
    else if((x->key)<k){
        return tree_search(x->left, k);
    }
    else{
        return tree_search(x->right, k);
    }
}

//MINUMUM O(h)

//xを根とする二分探索木から最小要素を出力する関数
node tree_minimum(node x){
    while(x->left!=NULL){
        x=x->left;
    }
    return x;
}

//MAXIMUM O(h)

//xを根とする二分探索木から最大要素を出力する関数
node tree_maximum(node x){
    while(x->right!=NULL){
        x=x->right;
    }
    return x;
}

//SUCCESSOR O(h)

//中間順で次の節点を出力する関数
node tree_successor(node x){
    node y;
    if(x->right!=NULL){
        return tree_minimum(x->right);
    }
    y=x->parent;
    while(y!=NULL&&x==y->right){
        x=y;
        y=y->parent;
    }
    return y;
}

//PREDECESSOR O(h)

//中間順で次の節点を出力する関数
node tree_predecessor(node x){
    node y;
    if(x->left!=NULL){
        return tree_maximum(x->left);
    }
    y=x->parent;
    while(y!=NULL&&x==y->left){
        x=y;
        y=y->parent;
    }
    return y;
}

//回転O(1)

//xを中心に左回転
void left_rotate(tree T, node x){
    node y=x->right;

    x->right=y->left; //スライドでいうベータを付け替え
    if(y->left!=NULL){
        y->left->parent=x;
    }

    y->parent=x->parent; //yの親を付け替え
    if((x->parent)==NULL){
        T->root=y;
    }
    else{
        if(x==(x->parent->left)){
            x->parent->left=y;
        }
        else{
            x->parent->right=y;
        }
    }

    y->left=x; //yの左の子の付け替え
    x->parent=y; //xの親の付け替え

    return;
}

//xを中心に右回転
void right_rotate(tree T, node y){
    node x=y->left;

    y->left=x->right; //スライドでいうベータを付け替え
    if(x->right!=NULL){
        x->right->parent=y;
    }

    x->parent=y->parent; //xの親を付け替え
    if((y->parent)==NULL){
        T->root=x;
    }
    else{
        if(y==(y->parent->left)){
            y->parent->left=x;
        }
        else{
            y->parent->right=x;
        }
    }

    x->right=y; //xの右の子の付け替え
    y->parent=x; //yの親の付け替え

    return;
}

//INSERT O(h)

//node xのinsertの際に二色木条件を保つようにする関数
void insert_fixup(tree T, node x){
    node y,z;
    while(x->parent->color==RED){
        z=x->parent;
        if(z==z->parent->left){
            y=z->parent->right;
            if(y->color==RED){
                z->color=BLACK;
                y->color=BLACK;
                z->parent->color=RED;
                x=z->parent;
            }
            else{
                if(x==z->right){
                    x=z;
                    left_rotate(T,x);
                }
                x->parent->color=BLACK;
                x->parent->parent->color=RED;
                right_rotate(T,x->parent->parent);
            }
        }
        else{
            y=z->parent->left;
            if(y->color==RED){
                z->color=BLACK;
                y->color=BLACK;
                z->parent->color=RED;
                x=z->parent;
            }
            else{
                if(x==z->left){
                    x=z;
                    right_rotate(T,x);
                }
                x->parent->color=BLACK;
                x->parent->parent->color=RED;
                left_rotate(T,x->parent->parent);
            }
        }
    }
    T->root->color=BLACK;

    return;
}

//insert関数
void tree_insert(tree T, node z){
    node x,y;
    y=NULL;
    x=T->root;

    //挿入場所の探索(yがzの親になる)
    while(x!=NULL){
        y=x;
        if((z->key)<(x->key)){
            x=x->left;
        }
        else{
            x=x->right;
        }
    }

    z->parent=y;
    if(y==NULL){
        T->root=z;
    }
    else if((z->key)<(y->key)){
        y->left=z;
    }
    else{
        y->right=z;
    }

    z->color=RED;
    insert_fixup(T,z);

    return;
}

//DELETE O(h)

//削除の際の木の修正
void delete_fixup(tree T, node x){
    node w;
    while(x!=T->root&&x->color==BLACK){
        if(x==x->parent->left){
            w=x->parent->right;
            if(w->color==RED){
                w->color=BLACK;
                x->parent->color=RED;
                left_rotate(T,x->parent);
                w=x->parent->right;
            }
            if(w->left->color==BLACK&&w->right->color==BLACK){
                w->color=RED;
                x=x->parent;
            }
            else{
                if(w->right->color==BLACK){
                    w->left->color=BLACK;
                    w->color=RED;
                    right_rotate(T,w);
                    w=x->parent->right;
                }
                w->color=x->parent->color;
                x->parent->color=BLACK;
                w->right->color=BLACK;
                left_rotate(T,x->parent);
                x=T->root;
            }
        }
        else{
            w=x->parent->left;
            if(w->color==RED){
                w->color=BLACK;
                x->parent->color=RED;
                right_rotate(T,x->parent);
                w=x->parent->left;
            }
            if(w->right->color==BLACK&&w->left->color==BLACK){
                w->color=RED;
                x=x->parent;
            }
            else{
                if(w->left->color==BLACK){
                    w->right->color=BLACK;
                    w->color=RED;
                    left_rotate(T,w);
                    w=x->parent->left;
                }
                w->color=x->parent->color;
                x->parent->color=BLACK;
                w->left->color=BLACK;
                right_rotate(T,x->parent);
                x=T->root;
            }
        }
    }
    x->color=BLACK;

    return;
}

//delete
void tree_delete(tree T, node z){
    node x,y;

    //yの設定(最終的に消すやつ)
    if((z->left)==NULL||(z->right)==NULL){ //もしzの子が1個以下なら
        y=z;
    }
    else{ //もしzの個が2個以下なら
        y=tree_successor(z);
    }

    //xの設定(yの左の子があれば左の子、なければ右の子、どっちもなければNULL)
    if((y->left)!=NULL){
        x=y->left;
    }
    else{
        x=y->right;
    }

    //子がいたらその子の親を変更
    if(x!=NULL){
        x->parent=y->parent;
    }
    if((y->parent)==NULL){ //もしTの根を削除するなら、根を更新
        T->root=x;
    }
    else if(y==y->parent->left){ //もしyが何かの左の子なら
        y->parent->left=x; //親の子を変更
    }
    else{ //もしyが何かの右の子なら
        y->parent->right=x; //親の子を変更
    }
    if(y!=z){ //もしzの子が2個あったとき
        z->key=y->key;
    }

    //二色木条件を満たすために修正
    if(y->color==BLACK){
        delete_fixup(T,x);
    }

    free_node(y);
    return;
}