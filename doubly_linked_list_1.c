//未ソート双方向連結リスト(head・tailあり)

#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

//整数と、リストの次要素を指すポインタをまとめた構造体slobjを定義
typedef struct slobj_{
    int key;
    struct slobj_ *next;
    struct slobj_ *prev;
}*slobj;

//slobj型の変数のポインタをslist型と定義→いずれリストの先頭要素を指す
typedef struct{
    slobj head;
    slobj tail;
}*slist;

//メモリ確保

//keyにxを持つリスト要素を作る関数
slobj slobj_new(int x){
    slobj p;
    NEW(p,1);
    p->key=x;
    p->next=NULL;
    p->prev=NULL;
    return p;
}

//slist型の空リスト1つを作る関数
slist slist_new(void){
    slist L;
    NEW(L,1);
    L->head=NULL;
    L->tail=NULL;
    return L;
}

//メモリ解放

//slobj要素のメモリを解放する
void free_slobj(slobj p){
    free(p);
    return;
}

//リスト全体のメモリを解放する
void free_slist(slist L){
    slobj p,tmp;
    p=L->head;

    while(p!=NULL){
        tmp=p->next;
        free_slobj(p);
        p=tmp;
    }
    //NULLはfreeしなくていい(?)ので、pとtmpはfreeしない
    free(L);

    return;
}

//search O(n)

//リストの中でkeyがkとなるslobj型の要素p、なければnullを返す(複数個あれば先頭のもの)
slobj slist_search(slist L, int k){
    slobj p= L->head;

    while(p!=NULL){
        if(p->key==k){
            break;
        }
        else{
            p=p->next;
        }
    }

    return p;
}

//insert O(1)

//slobj型の要素pをリストLの先頭に挿入する関数
void slist_insert_head(slist L,slobj p){
    p->prev=NULL;
    p->next=L->head;
    L->head=p;
    
    return;
}

//slobj型の要素pをリストLの最後に挿入する関数
void slist_insert_tail(slist L,slobj p){
    p->prev=L->tail;
    p->next=NULL;
    L->tail=p;
    
    return;
}

//delete O(1)

//リストLの中のslobj要素pを削除する
void slist_delete(slist L, slobj p){
    if(p==L->head){
        p->next->prev=NULL;
        L->head=p->next;
    }
    if(p==L->tail){
        p->prev->next=NULL;
        L->tail=p->prev;
    }
    if(p!=L->head&&p!=L->tail){
        p->prev->next=p->next;
        p->next->prev=p->prev;
    }
    free_slobj(p);
    
    return;
}

//その他

//リストの要素を先頭から表示する関数
void slist_print(slist L){
    slobj p;
    p=L->head;
    while(p!=NULL){
        printf("%d ",p->key);
        p=p->next;
    }
    printf("\n");
    
    return;
}