//ソートアルゴリズム(昇順)の関数まとめ

#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

//A[1]~A[n]のソート

//選択ソートO(n^2)
int* selection_sort(int* A, int n){
    for(int i=1;i<=n;i++){
        int k=i;//今の基準はA[k]
        int tmp=A[i];
        for(int j=i+1;j<=n;j++){
            if(A[j]<tmp){
                k=j;
                tmp=A[j];
            }
        }
        if(k!=i){
            int tmp2;
            tmp2=A[k];
            A[k]=A[i];
            A[i]=tmp2;
        }
    }


    return A;
}

//挿入ソートO(n^2)
int* insert_sort(int* A, int n){
    int i,j;
    for(i=1;i<=n;i++){
        int tmp=A[i];
        for(j=i-1;j>=0&&A[j]>tmp;j--){
            A[j+1]=A[j];
        }
        if(j!=i){
            A[j+1]=tmp;
        }
    }

    return A;
}