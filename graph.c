//グラフ関連の関数

#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n){p=malloc((n)*sizeof(p[0]));if(p==NULL){printf("not enough memory\n");exit(1);};}
//pの型の変数n個の要素分のメモリを確保し、そのアドレスをpに代入するマクロ

#define WHITE 0
#define GRAY 1
#define BLACK 2

//節点uの構造体の定義
typedef struct slobj_{
    struct slobj_ *next;
    int v; //uから枝が伸びている節点v
    double d; //枝(u,v)の重み
}* slobj;

//slobj型のリストの定義
typedef struct{
    slobj head;
    slobj tail;
}* slist;

//グラフの構造体の定義
typedef struct{
    int n; //節点数
    int m; //枝数
    slist* E; //枝リストの配列E[0]~E[n-1],未ソート単方向
}* graph;

//メモリ確保

//vにn,dにxを持つslobj型のリスト要素のメモリ確保+作成する関数
slobj slobj_new(int n,double x){
    slobj p;
    NEW(p,1);
    p->v=n;
    p->d=x;
    p->next=NULL;
    return p;
}

//slist型のメモリを1個確保し、空リスト1つを作る関数
slist slist_new(void){
    slist L;
    NEW(L,1);
    L->head=NULL;
    L->tail=NULL;
    return L;
}

//各slistを空リストにした,節点n枝数mのgraph型のメモリを確保する関数
graph graph_new(int n,int m){
    graph G;
    NEW(G,1);
    
    G->n=n;
    G->m=m;
    
    NEW(G->E,G->n);
    int i;
    for(i=0;i<G->n;i++){
        G->E[i]=slist_new();
    }
    
    return G;
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

//グラフGのメモリを解放する
void free_graph(graph G){
    int n=G->n;
    for(int i=0;i<n;i++){
        free_slist(G->E[i]);
    }
    free(G);
    return;
}

//枝の追加

//リストLの末尾に要素rを追加する関数
void slist_append_tail(slist L,slobj r){
    if(L->head==NULL){
        L->head=r;
        L->tail=r;
    }
    else{
        L->tail->next=r;
        L->tail=r;
    }
    
    return;
}

//節点i(0~n-1)から節点j(0~n-1)に、重さxの枝を追加する関数
void add_edge(graph G, int i, int j, double x){
    //枝数を+1する
    G->m=G->m+1;

    //枝データを追加
    slobj p=slobj_new(j,x);
    slist_append_tail(G->E[i],p);

    return;
}

//DFS

//colorは節点の色を格納する配列、time[0]に現在時刻が格納されている
//d[i]が節点iを最初に発見した時刻、f[i]が節点iの探索終了した時刻、parent[i]はDFS木でのiの親の節点
//グラフGで点uからの探索を行う関数
void DFS_Visit(graph G,int u,int* parent,int* color,int* d,int* f,int* time){
    color[u]=GRAY;
    time[0]++;
    d[u]=time[0];
    
    slobj p=G->E[u]->head;
    while(p!=NULL){
        if(color[p->v]==WHITE){
            parent[p->v]=u;
            DFS_Visit(G,p->v,parent,color,d,f,time);
        }
        p=p->next;
    }
    color[u]=BLACK;
    time[0]++;
    f[u]=time[0];
    
    return;
}

//グラフGの深さ優先探索を行う関数
void DFS(graph G,int* d,int* f){
    int* color;
    int* parent;
    
    NEW(color,(G->n));
    NEW(parent,(G->n));
    
    int u;
    for(u=0;u<G->n;u++){
        color[u]=WHITE;
        parent[u]=-1;
    }
    
    int time[1];
    time[0]=0;
    
    for(u=0;u<G->n;u++){
        if(color[u]==WHITE){
            DFS_Visit(G,u,parent,color,d,f,time);
        }
    }
    free(color);
    
    return;
}

//BFS

//int変数kをenqueueする関数(nはQueueに入れられる最大要素数)
void enqueue(int* Q, int k, int n){
    int i=0;
    while(i<n){
        if(Q[i]!=-1){
            i++;
        }
        else{
            Q[i]=k;
            break;
        }
    }

    if(i==n){
        printf("overflow error\n");
    }

    return;
}

//dequeueして値を返す関数(nはQueueに入れられる最大要素数)
//値がなければ-1
int dequeue(int* Q, int n){
    int ans=Q[0];
    for(int i=1;i<n;i++){
        Q[i-1]=Q[i];
    }
    Q[n-1]=-1;

    return ans;
}

//colorは節点の色を格納する配列(白：まだ・灰；中；黒：既)、time[0]に現在時刻が格納されている
//d[i]が節点iを最初に発見した(キューに入れられた)時刻、f[i]が節点iの探索終了した(キューからでた)時刻、parent[i]はBFS木でのiの親の節点
//グラフGで点uからの探索を行う関数
void BFS_Visit(graph G,int u,int* Q,int* parent,int* color,int* d,int* f,int* time){
    color[u]=BLACK;
    time[0]++;
    f[u]=time[0];
    
    slobj p=G->E[u]->head;
    while(p!=NULL){
        if(color[p->v]==WHITE){
            parent[p->v]=u;
            enqueue(Q,p->v,G->n);
            d[p->v]=time[0];
            color[p->v]=GRAY;
        }
        p=p->next;
    }
    
    return;
}

//グラフGの深さ優先探索を行う関数
void BFS(graph G,int* d,int* f){
    int* color;
    int* parent;
    int* Q; //キュー
    
    NEW(color,(G->n));
    NEW(parent,(G->n));
    NEW(Q,(G->n));
    
    int u;
    for(u=0;u<G->n;u++){
        color[u]=WHITE;
        parent[u]=-1;
        Q[u]=-1;
    }
    
    int time[1];
    time[0]=0;

    
    while(1){
        int v=dequeue(Q, G->n);
        if(v!=-1){
            BFS_Visit(G, v, Q, parent, color, d, f, time);
        }
        else{
            int counter=0;
            for(int i=0;i<G->n;i++){
                if(color[i]==WHITE){
                    counter=1;
                    enqueue(Q,i,G->n);
                    d[v]=time[0];
                    color[v]=GRAY;

                    v=dequeue(Q,G->n);
                    BFS_Visit(G, v, Q, parent, color, d, f, time);
                    break;
                }
            }
            if(counter==0){
                break;
            }
        }
    }
    free(color);
    free(Q);
    
    return;
}

//強連結成分分解

//グラフGの転置のメモリを確保・出力する関数
graph t_graph(graph G){
    graph GT=graph_new(G->n,G->m);
    
    int i;
    slobj p,q;
    for(i=0;i<G->n;i++){
        p=G->E[i]->head;
        while(p!=NULL){
            q=slobj_new(i+1,p->d);
            slist_append_tail(GT->E[(p->v)-1],q);
            p=p->next;
        }
    }
    
    return GT;
}

//点を訪れる順番をordで指定してDFSを行う関数
void DFS2(graph G,int* ord,int* parent,int* d,int* f){
    int* color;
    NEW(color,(G->n)+1);
    
    int u;
    for(u=1;u<=G->n;u++){
        color[u]=WHITE;
        parent[u]=-1;
    }
    
    int time[1];
    time[0]=0;
    
    int i;
    for(i=1;i<=G->n;i++){
        if(color[ord[i]]==WHITE){
            DFS_Visit(G,ord[i],parent,color,d,f,time);
        }
    }
    free(color);
    
    return;
    
}

//グラフGの強連結成分分解を行う関数
int* SCC(graph G,int* d,int* f){
    DFS(G,d,f);
    graph GT=t_graph(G);
    
    int* ord; //ordはf[u]の大きい順にuを並べた配列
    int* ord_tmp;
    int n=GT->n;
    NEW(ord,n+1);
    NEW(ord_tmp,2*n+1);
    
    int i;
    for(i=1;i<=2*n;i++) ord_tmp[i]=-1;
    for(i=1;i<=n;i++) ord_tmp[f[i]]=i;
    int k=0;
    for(i=2*n;i>=1;i--){
        if(ord_tmp[i]!=-1){
            k++;
            ord[k]=ord_tmp[i];
        }
    }
    free(ord_tmp);
    //ここまでordの作成
    
    int* parent;
    NEW(parent,(G->n)+1);
    DFS2(GT,ord,parent,d,f);

    free_graph(GT);
    free(ord);
    
    return parent;
    //parentを見れば連結成分がわかる(-1の個数が成分個数、parent[v]がvがどの成分に属するか)
}

//ダイクストラ法(頂点からの最短距離)

#define LEFT(i) (2*(i))
#define RIGHT(i) (2*(i)+1)
#define PARENT(i) ((i)/2)

#define INFTY 999999

#define min(a,b) ((a)<(b)?(a):(b))

typedef struct{
    int priority;
    double value;
}heapdata; //heapの構造体の中の要素を定義

//ヒープ二分木のポインタをheap型と定義
typedef struct{
    int max_size; //ヒープ二分木(配列A)の中に格納できる最大要素数
    int heap_size; //ヒープ二分木(配列A)の中に現在入っている要素の数
    heapdata* A; //要素を格納するための配列(ポインタ;)
    int* inv; //int[i]=jは、「priorityにiが入っているのはヒープのj番目」を意味
}* heap;

//ヒープH内の配列のA[i]とA[j]の値を入れ替える関数
void heap_swap(heap H,int i,int j){
    heapdata y;
    
    y=H->A[i];
    H->A[i]=H->A[j];
    H->A[j]=y;
    
    //invの更新
    H->inv[(H->A[i].priority)]=i;
    H->inv[(H->A[j].priority)]=j;
    
    return;
}

//A[i]を根とする部分木がヒープになるように整序する関数
void heap_heapify(heap H, int i){
    int l,r,smallest,size;
    heapdata* A;
    
    l=LEFT(i); //iの左の子の添字
    r=RIGHT(i); //iの右の子の添字
    size=H->heap_size; //今のヒープHのサイズ
    A=H->A; //ヒープ二分木に格納されている要素の配列
    
    //以下smallestを決めるコード
    if(l>size&&r>size){ //A[l]やA[r]が存在しない場合
        smallest=i;
    }
    else if(l<=size&&r>size){ //A[l]が存在し、A[r]が存在しない場合
        if(A[l].value>A[i].value){
            smallest=i;
        }
        else{
            smallest=l;
        }
    }
    else{ //A[l]とA[r]が両方存在する場合
        if(A[r].value>A[l].value){
            smallest=l;
        }
        else{
            smallest=r;
        }
        if(A[i].value<A[smallest].value){
            smallest=i;
        }
    }
    
    if(smallest!=i){
        heap_swap(H,i,smallest);
        heap_heapify(H,smallest);
    }
    
    return;
}

//要素n個の配列A、max_sizeをもつヒープ木Hのメモリを確保し出力する関数
heap heap_build(int n,heapdata* A,int max_size){
    int i,j,k;
    heap H;
    NEW(H,1);
    
    H->max_size=max_size;
    H->heap_size=n;
    H->A=A;
    
    //invの設定
    NEW(H->inv,max_size+1);
    for(i=1;i<=max_size;i++){
        H->inv[i]=-1; //invの初期値は-1
    }
    for(j=1;j<=(H->heap_size);j++){
        H->inv[(H->A[j].priority)]=j;
    }
    
    for(k=n/2;k>=1;k--){
        heap_heapify(H,k);
    }
    
    return H;
}

//ヒープ木の中の最小要素を削除し、その最小要素を除いたヒープ木を再構成する関数
heapdata heap_extract_min(heap H){
    int n=H->heap_size;
    
    heap_swap(H,1,n);
    H->heap_size=n-1;
    H->inv[(H->A[n].priority)]=-1; //削除されたらinvは-1になる
    heap_heapify(H,1);
    
    return H->A[n];
}

//data xをヒープHに挿入する関数
void heap_insert(heap H,heapdata x){
    int n=H->heap_size;
    
    H->A[n+1]=x;
    H->heap_size=n+1;
    H->inv[(H->A[n+1].priority)]=n+1;
    
    int i=n+1;
    while(PARENT(i)>=1&&H->A[i].value<H->A[PARENT(i)].value){
        heap_swap(H,i,PARENT(i));
        i=PARENT(i);
    }
    
    return;
}

//priorityがjである要素がヒープHの何番目に入っているのかを返す関数
int heap_exist(heap H,int j){
    int i=H->inv[j];
    
    if(i>0){
        return i;
    }
    else{
        return -1;
    }
}

//ヒープからA[i]を削除する関数
void heap_delete(heap H,int i){
    int j=i;
    while(PARENT(j)>0){
        heap_swap(H,j,PARENT(j));
        j=PARENT(j);
    }
    //A[i]の親から根までの値を順に下ろし、A[i]を根に入れた
    
    heapdata x=heap_extract_min(H);
    
    return;
}

//グラフGをの頂点sからDijkstra法を行い、結果を配列dist(dist[i]はsからiまでの最短距離)に収める関数
void Dijkstra(graph G,int s,double* dist){
    int n=G->n;
    
    //distの初期化
    int i;
    for(i=1;i<=n;i++){
        dist[i]=INFTY;
    }
    dist[s]=0;
    
    //ヒープの作成
    int j;
    heapdata x;
    heapdata* A;
    NEW(A,n+1);
    for(j=1;j<=n;j++){
        x.priority=j;
        x.value=dist[j];
        A[j]=x;
    }
    heap H=heap_build(n,A,n);
    
    heapdata y,z;
    slobj p;
    int k;
    while((H->heap_size)>0){
        y=heap_extract_min(H);
        dist[y.priority]=y.value;
        
        p=G->E[y.priority]->head;
        while(p!=NULL){
            k=heap_exist(H,p->v);
            if(k>0){
                z.priority=p->v;
                z.value=min((H->A[k].value),(y.value+p->d));
                heap_delete(H,k);
                heap_insert(H,z);
            }
            p=p->next;
        }
    }

    free(H);
    
    return;
}