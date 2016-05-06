/*
 * 制空値最適化プログラム“烈風拳であります！” v0.2
 * This source and related resources are available under BSD License.
 * Copyright (c) 2016-, suzuryo All rights reserved.
 */
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<cctype>

#include<vector>
#include<queue>

//#define DEB printf("line: %d\n",__LINE__)

//艦載機数から各スロットの制空値を計算
void calcSeiku(int *src, int *dst);
void calcSeiku(std::vector<int> &src, std::vector<int> &dst);
//onビットの計数
int bitcount(unsigned int a);
//n桁のビットの並び順を逆順にする
unsigned int bitrev(unsigned int a, int n);

//設定
struct Conf{
	int candNum;
	int bonus;
	int target;
	double marginrate;
	int maxmargin;
	double maxmarginrate;
	int knum;
	int apow;
	std::vector<std::vector<int> > acnum;
	std::vector<std::vector<int> > acpow;
	char outputlistname[256];
	bool pauseOnExit;
	
	Conf(){
		defparam();
	}
	void defparam(){
		candNum=20;
		bonus=25;
		target=182;
		marginrate=1.1;
		maxmarginrate=2.0;
		maxmargin=target*maxmarginrate;
		apow=10;
		
		knum=3;
		acnum.clear(); acnum.resize(knum);
		acpow.clear(); acpow.resize(knum);
		//RJ
		acnum[0].resize(4,0); acpow[0].resize(4,0);
		acnum[0][0]=18;
		acnum[0][1]=28;
		acnum[0][2]=6;
		acnum[0][3]=3;
		//AKG
		acnum[1].resize(4,0); acpow[1].resize(4,0);
		acnum[1][0]=20;
		acnum[1][1]=20;
		acnum[1][2]=32;
		acnum[1][3]=10;
		//KG
		acnum[2].resize(4,0); acpow[2].resize(4,0);
		acnum[2][0]=20;
		acnum[2][1]=20;
		acnum[2][2]=46;
		acnum[2][3]=12;
		for(int i=0;i<knum;++i) calcSeiku(acnum[i], acpow[i]);
		
		outputlistname[0]='\0';
		pauseOnExit=true;
	}

	bool readConf(char *fname){
		FILE *fp;
		if(!(fp=fopen(fname,"r"))){
			perror(fname);
			return false;
		}
		
		char buf[1024];
		while(fgets(buf,1024,fp)){
			if(0);
			else if(strncmp(buf,"candNum=",8)==0) sscanf(buf+8,"%d",&candNum);
			else if(strncmp(buf,"bonus=",6)==0) sscanf(buf+6,"%d",&bonus);
			else if(strncmp(buf,"target=",7)==0) sscanf(buf+7,"%d",&target);
			else if(strncmp(buf,"marginrate=",11)==0) sscanf(buf+11,"%lf",&marginrate);
			else if(strncmp(buf,"maxmarginrate=",14)==0){
				sscanf(buf+14,"%lf",&maxmarginrate);
				maxmargin=target*maxmarginrate;
			}
			else if(strncmp(buf,"knum=",5)==0) sscanf(buf+5,"%d",&knum);
			else if(strncmp(buf,"apow=",5)==0) sscanf(buf+5,"%d",&apow);
			else if(strncmp(buf,"acnum=",6)==0){
				acnum.clear(); acnum.resize(knum);
				acpow.clear(); acpow.resize(knum);
				for(int i=0;i<knum;++i){
					acnum[i].resize(4,0); acpow[i].resize(4,0);
					if(!fgets(buf,1024,fp) || buf[0]=='}') break;
					if(buf[0]=='#'){ --i;continue;}
					char *p;
					if(p=strtok(buf,",")) sscanf(p,"%d",&acnum[i][0]);
					else break;
					if(p=strtok(NULL,",")) sscanf(p,"%d",&acnum[i][1]);
					else break;
					if(p=strtok(NULL,",")) sscanf(p,"%d",&acnum[i][2]);
					else break;
					if(p=strtok(NULL,",")) sscanf(p,"%d",&acnum[i][3]);
					else break;
				}
				while(fgets(buf,1024,fp) && buf[0]!='}');
				
				for(int i=0;i<knum;++i) calcSeiku(acnum[i],acpow[i]);
			}
			else if(strncmp(buf,"outputlistname=",15)==0) sscanf(buf+15,"%255s",outputlistname); //セキュアじゃない
			else if(strncmp(buf,"pauseOnExit=",12)==0) sscanf(buf+12,"%d",&pauseOnExit);
		}
		
		return true;
	}
	
	bool writeConf(char *fname){
		FILE *fp;
		if(!(fp=fopen(fname,"w"))){
			perror(fname);
			return false;
		}
		
		fprintf(fp,"#表示する候補の数\n");
		fprintf(fp,"candNum=%d\n",candNum);
		fprintf(fp,"#熟練度ボーナス\n");
		fprintf(fp,"bonus=%d\n",bonus);
		fprintf(fp,"#目標の制空値\n");
		fprintf(fp,"target=%d\n",target);
		fprintf(fp,"#制空値への上乗せの比率（target比）\n");
		fprintf(fp,"marginrate=%g\n",marginrate);
		fprintf(fp,"#計算打ち切りの制空値の比率（target比）（計算高速化用）\n");
		fprintf(fp,"maxmarginrate=%g\n",maxmarginrate);
		fprintf(fp,"#採用する艦載機の対空値\n");
		fprintf(fp,"apow=%d\n",apow);
		fprintf(fp,"#空母の数\n");
		fprintf(fp,"knum=%d\n",knum);
		fprintf(fp,"#空母の装備スロット情報\n");
		fprintf(fp,"acnum={\n");
		for(int i=0;i<knum;++i){
			fprintf(fp,"\t");
			for(int j=0;j<4;++j) fprintf(fp,"%d%c",acnum[i][j], j<4-1?',':'\n');
		}
		fprintf(fp,"}\n");
		fprintf(fp,"#パターン全列挙の情報が欲しい場合，ファイル名\n");
		fprintf(fp,"outputlistname=%s\n",outputlistname);
		fprintf(fp,"#エンターキーが押されるまでプログラムの終了を待つか．1:ON 0:OFF\n");
		fprintf(fp,"pauseOnExit=%d\n",pauseOnExit?1:0);
		return true;
	}
};
Conf conf;

//烈風(+10)，烈風601(+11)，烈風改(+12)　震電？ねぇよんなもん
//int ks[3]={10,1,1};

//DPテーブル
//int dp[4*6+1][1<<(4*6)];
//メモ化再帰で全探索
//艦数(<6)，見ているスロット，採用状態，制空値データ
//~ void calcAllPattern_r(int n, int head, unsigned int stat, std::vector<std::vector<int> > &kantai_s){
	//~ if(head<4*n){
		//~ int nstat;
		//~ //立てない場合
		//~ nstat=stat;
		//~ if(dp[head+1][nstat]==-1){
			//~ dp[head+1][nstat]=(head<0?0:dp[head][stat]);
			//~ calcAllPattern_r(n,head+1, nstat, kantai_s);
		//~ }
		//~ //立てる場合
		//~ int c=head%4;
		//~ int r=head/4;
		//~ if(kantai_s[r][c]>0 && dp[head][stat]<=conf.maxmargin){ //艦載機数0はやらない and 制空値上限過ぎたらもう立てない
			//~ nstat=stat|(1<<head);
			//~ if(dp[head+1][nstat]==-1){
				//~ dp[head+1][nstat]=(head<0?0:dp[head][stat])+kantai_s[r][c];
				//~ calcAllPattern_r(n,head+1, nstat, kantai_s);
			//~ }
		//~ }
	//~ }
//~ }


int dp2[1<<(4*6)];
void calcAllPattern_r_2(int n,int head, unsigned int stat, int val, std::vector<std::vector<int> > &kantai_s){
	if(head>=4*n){ //メモって終了
		dp2[stat]=val; //やっぱり2^(4*n)必要 最大2^24 -> 64MB
		return;
	}
	
	int c=head%4;
	int r=head/4;
	
	//立てない
	calcAllPattern_r_2(n,head+1,stat,val,kantai_s);
	
	//1立てる
	//つーかtargetを超えたら打ち切りでいいじゃん
	if(kantai_s[r][c]>0 && val<conf.target)
		calcAllPattern_r_2(n,head+1,stat|(1<<head),val+kantai_s[r][c],kantai_s);
	
	//2立てる...
	
}



//候補表示のための構造体
struct Res{
	int diff;
	unsigned int stat;
	Res(int _diff=0, unsigned int _stat=0):diff(_diff),stat(_stat){}
	friend static bool operator<(const Res &a,const Res &b){
		//if(a.diff==b.diff) return bitcount(a.stat)<bitcount(b.stat);
		//else return a.diff<b.diff;
		if(bitcount(a.stat)==bitcount(b.stat)) return a.diff<b.diff;
		else return bitcount(a.stat)<bitcount(b.stat);
	}
};


int main(int argc, char **argv){
	if(argc>1){
		if(!conf.readConf(argv[1])) exit(-1);
	}
	else{
		printf("Usage: %s conffile.conf\n",argv[0]);
		conf.writeConf("tmp.conf");
		printf(" - tmp.conf is created.\n");
		exit(1);
	}
	
	for(int i=0;i<conf.knum;++i){
		int sum=0;
		for(int j=0;j<4;++j) sum+=conf.acpow[i][j];
		printf("%d -> %d %d %d %d | %d\n",i,conf.acpow[i][0],conf.acpow[i][1],conf.acpow[i][2],conf.acpow[i][3], sum);
	}
	
	//全探索
	//~ memset(dp,-1,sizeof(dp));
	//~ dp[0][0]=0;
	//~ calcAllPattern_r(conf.knum,0,0,conf.acpow);
	calcAllPattern_r_2(conf.knum,0,0,0,conf.acpow);
	
	//パラメータ
	std::priority_queue<Res> q;
	
	int target=conf.target*conf.marginrate; //実際の目標制空値
	
	int min=1<<30;
	unsigned int minstat=0;
	//結果を走査
	FILE *fp=NULL;
	if(conf.outputlistname[0]) fp=fopen(conf.outputlistname,"w");
	for(int i=0;i<(1<<(4*conf.knum));++i){
		char tmp[256];
		sprintf(tmp,"%%0%dx %%d\n",conf.knum);
		//if(fp) fprintf(fp,tmp,bitrev(i,4*conf.knum),dp[4*conf.knum][i]);
		if(fp) fprintf(fp,tmp,bitrev(i,4*conf.knum),dp2[i]);
		//int diff=dp[4*conf.knum][i]-target;
		int diff=dp2[i]-target;
		if(diff>=0){
			q.push(Res(diff,i));
			if(q.size()>conf.candNum) q.pop();
		}
	}
	if(fp) fclose(fp);
	
	//表示
	printf("top %d candidates: \n",conf.candNum);
	while(!q.empty()){
		Res r=q.top(); q.pop();
		char tmp[256];
		sprintf(tmp,"%%0%dx %%d %%d / %%d\n",conf.knum);
		//printf(tmp,bitrev(r.stat,4*conf.knum),bitcount(r.stat),dp[4*conf.knum][r.stat],target);
		printf(tmp,bitrev(r.stat,4*conf.knum),bitcount(r.stat),dp2[r.stat],target);
	}
	
	//キー入力待ち
	if(conf.pauseOnExit){
		puts("hit enter to exit.");
		getchar();
	}
	
	return 0;
}

//艦載機数から各スロットの制空値を計算
void calcSeiku(int *src, int *dst){
	for(int i=0;i<4;++i){
		dst[i]=(src[i]?(int)floor(conf.apow*sqrt((double)src[i]))+conf.bonus:0);
	}
	return;
}
void calcSeiku(std::vector<int> &src, std::vector<int> &dst){
	for(int i=0;i<4;++i){
		dst[i]=(src[i]?(int)floor(conf.apow*sqrt((double)src[i]))+conf.bonus:0);
	}
	return;
}
//onビットの計数
int bitcount(unsigned int a){
	int cnt=0;
	while(a){ cnt+=(a&1); a>>=1;}
	return cnt;
}
//n桁のビットの並び順を逆順にする n<=31
unsigned int bitrev(unsigned int a, int n){
	unsigned int b=0;
	for(int i=0;i<n;++i,(a>>=1),(b<<=1)){
		b|=(a&1);
	}
	return b>>1;
}

