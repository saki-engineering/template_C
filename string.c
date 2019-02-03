#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

//char型のポインタ(=配列と意味は同じ)をString型と定義([0]から数える)
typedef char* String;

//文字列の長さを求める関数
int string_len(String str){
    int len=0;
    while(str[len]!=0){
        len++;
    }
    return len;
}

//文字列を標準入力から読み込み、それをString型のメモリを確保し直してくれる関数
//S[0]~S[n-1]までが入力で、S[n]は0になる
String string_input(void){
    int i,len;
    char buf[1000];
    String str;
    scanf("%s",buf);//buf=入力した文字列が入るchar型の配列
    //ここまでは文字列の長さがわからなかったから、一旦1000個分のメモリを用意しそこに入力させる
    
    len=string_len(buf);//len=文字列の長さを表すint型変数
    NEW(str,len+1);//strはただのポインタだから、ここでNEWを使ってメモリを確保しなくちゃいけない
    for(i=0;i<len;i++){
        str[i]=buf[i];
    }
    str[len]=0;
    
    return str;
}

//与えられた文字列を比較し、結果によって決められたint型変数を返す関数
//p<qなら-1,p>qなら+1,p==qなら0
int string_compare(String p,String q){
    int np=string_len(p);
    int nq=string_len(q);
    int m;
    if(np<=nq){
        m=np;
    }
    else{
        m=nq;
    }
    //mは、文字列p,qのうち短い方の長さを表すint型変数
    
    int i=0;
    int n=0;
    while(i<=m){
        if(p[i]<q[i]){
            n=-1;
            break;
        }
        else if(p[i]>q[i]){
            n=1;
            break;
        }
        else{
            i++;
        }
    }
    
    return n;
}

int ctoi(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	return 0;
}