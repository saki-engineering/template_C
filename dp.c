#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

#define INFTY 999999

typedef long* vector;
typedef vector* matrix;
typedef matrix* tmatrix;

//0-c-1の添字のものを作る(初期値0)
vector create_vector(int c){
    vector v;
    NEW(v,c);
    for(int i=0;i<c;i++){
        v[i]=0;
    }
    return v;
}

//0-b-1,0-c-1の添字のものを作る
matrix create_matrix(int b, int c){
    matrix M;
    NEW(M,1);
    for(int i=0;i<b;i++){
        M[i]=create_vector(c);
    }
    return M;
}

//0-a-1,0-b-1,0-c-1の添字のものを作る
tmatrix create_tmatrix(int a, int b, int c){
    tmatrix T;
    NEW(T,1);
    for(int i=0;i<a;i++){
        T[i]=create_matrix(b,c);
    }
    return T;
}

//サイズcのベクトルのメモリ解法
void free_vector(vector v, int c){
    free(v);
    return;
}

//サイズb,cのmatrixのメモリ解法
void free_matrix(matrix M, int b, int c){
    for(int i=0;i<b;i++){
        free_vector(M[i],c);
    }
    free(M);
    return;
}

//サイズa,b,cのtmatrixのメモリ解法
void free_tmatrix(tmatrix T, int a, int b, int c){
    for(int i=0;i<a;i++){
        free_matrix(T[i],b,c);
    }
    free(T);
    return;
}