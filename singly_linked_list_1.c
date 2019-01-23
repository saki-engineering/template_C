//未ソート単方向連結リスト(headあり・tailなし)

#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

//整数と、リストの次要素を指すポインタをまとめた構造体slobjを定義
typedef struct slobj_{
    int key;
    struct slobj_ *next;
}*slobj;

//slobj型の変数のポインタをslist型と定義→いずれリストの先頭要素を指す
typedef struct{
    slobj head;
}*slist;

//メモリ確保

//keyにxを持つリスト要素を作る関数
slobj slobj_new(int x){
    slobj p;
    NEW(p,1);
    p->key=x;
    p->next=NULL;
    return p;
}

//slist型の空リスト1つを作る関数
slist slist_new(void){
    slist L;
    NEW(L,1);
    L->head=NULL;
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
        free(p);
        p=tmp;
    }
    //NULLはfreeしなくていい(?)ので、pとtmpはfreeしない
    free(L);

    return;
}

//search

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

//insert

//slobj型の要素pをリストLの先頭に挿入する関数
void slist_insert_head(slist L,slobj p){
    p->next=L->head;
    L->head=p;
    
    return;
}

//delete

//リストの中で要素がkとなる最初のslobj要素を削除する
void slist_delete(slist L, int k){
    slobj p,q;
    //qが削除したい要素、pがqの前の要素
    p=NULL;
    q=L->head;

    //削除する要素qとその前要素pを探索する
    while(q!=NULL){
        if(q->key==k){
            break;
        }
        else{
            q=p;
            p=p->next;
        }
    }

    if(q==L->head){
        L->head=q->next;
        free(q);
    }
    else if(q!=NULL){
        p->next=q->next;
        free(q);
    }
    
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