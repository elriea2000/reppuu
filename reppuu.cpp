/*
 * ����l�œK���v���O�����g�󕗌��ł���܂��I�h v0.2
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

//�͍ڋ@������e�X���b�g�̐���l���v�Z
void calcSeiku(int *src, int *dst);
void calcSeiku(std::vector<int> &src, std::vector<int> &dst);
//on�r�b�g�̌v��
int bitcount(unsigned int a);
//n���̃r�b�g�̕��я����t���ɂ���
unsigned int bitrev(unsigned int a, int n);

//�ݒ�
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
			else if(strncmp(buf,"outputlistname=",15)==0) sscanf(buf+15,"%255s",outputlistname); //�Z�L���A����Ȃ�
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
		
		fprintf(fp,"#�\��������̐�\n");
		fprintf(fp,"candNum=%d\n",candNum);
		fprintf(fp,"#�n���x�{�[�i�X\n");
		fprintf(fp,"bonus=%d\n",bonus);
		fprintf(fp,"#�ڕW�̐���l\n");
		fprintf(fp,"target=%d\n",target);
		fprintf(fp,"#����l�ւ̏�悹�̔䗦�itarget��j\n");
		fprintf(fp,"marginrate=%g\n",marginrate);
		fprintf(fp,"#�v�Z�ł��؂�̐���l�̔䗦�itarget��j�i�v�Z�������p�j\n");
		fprintf(fp,"maxmarginrate=%g\n",maxmarginrate);
		fprintf(fp,"#�̗p����͍ڋ@�̑΋�l\n");
		fprintf(fp,"apow=%d\n",apow);
		fprintf(fp,"#���̐�\n");
		fprintf(fp,"knum=%d\n",knum);
		fprintf(fp,"#���̑����X���b�g���\n");
		fprintf(fp,"acnum={\n");
		for(int i=0;i<knum;++i){
			fprintf(fp,"\t");
			for(int j=0;j<4;++j) fprintf(fp,"%d%c",acnum[i][j], j<4-1?',':'\n');
		}
		fprintf(fp,"}\n");
		fprintf(fp,"#�p�^�[���S�񋓂̏�񂪗~�����ꍇ�C�t�@�C����\n");
		fprintf(fp,"outputlistname=%s\n",outputlistname);
		fprintf(fp,"#�G���^�[�L�[���������܂Ńv���O�����̏I����҂��D1:ON 0:OFF\n");
		fprintf(fp,"pauseOnExit=%d\n",pauseOnExit?1:0);
		return true;
	}
};
Conf conf;

//��(+10)�C��601(+11)�C�󕗉�(+12)�@�k�d�H�˂����Ȃ���
//int ks[3]={10,1,1};

//DP�e�[�u��
//int dp[4*6+1][1<<(4*6)];
//�������ċA�őS�T��
//�͐�(<6)�C���Ă���X���b�g�C�̗p��ԁC����l�f�[�^
//~ void calcAllPattern_r(int n, int head, unsigned int stat, std::vector<std::vector<int> > &kantai_s){
	//~ if(head<4*n){
		//~ int nstat;
		//~ //���ĂȂ��ꍇ
		//~ nstat=stat;
		//~ if(dp[head+1][nstat]==-1){
			//~ dp[head+1][nstat]=(head<0?0:dp[head][stat]);
			//~ calcAllPattern_r(n,head+1, nstat, kantai_s);
		//~ }
		//~ //���Ă�ꍇ
		//~ int c=head%4;
		//~ int r=head/4;
		//~ if(kantai_s[r][c]>0 && dp[head][stat]<=conf.maxmargin){ //�͍ڋ@��0�͂��Ȃ� and ����l����߂�����������ĂȂ�
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
	if(head>=4*n){ //�������ďI��
		dp2[stat]=val; //����ς�2^(4*n)�K�v �ő�2^24 -> 64MB
		return;
	}
	
	int c=head%4;
	int r=head/4;
	
	//���ĂȂ�
	calcAllPattern_r_2(n,head+1,stat,val,kantai_s);
	
	//1���Ă�
	//�[��target�𒴂�����ł��؂�ł��������
	if(kantai_s[r][c]>0 && val<conf.target)
		calcAllPattern_r_2(n,head+1,stat|(1<<head),val+kantai_s[r][c],kantai_s);
	
	//2���Ă�...
	
}



//���\���̂��߂̍\����
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
	
	//�S�T��
	//~ memset(dp,-1,sizeof(dp));
	//~ dp[0][0]=0;
	//~ calcAllPattern_r(conf.knum,0,0,conf.acpow);
	calcAllPattern_r_2(conf.knum,0,0,0,conf.acpow);
	
	//�p�����[�^
	std::priority_queue<Res> q;
	
	int target=conf.target*conf.marginrate; //���ۂ̖ڕW����l
	
	int min=1<<30;
	unsigned int minstat=0;
	//���ʂ𑖍�
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
	
	//�\��
	printf("top %d candidates: \n",conf.candNum);
	while(!q.empty()){
		Res r=q.top(); q.pop();
		char tmp[256];
		sprintf(tmp,"%%0%dx %%d %%d / %%d\n",conf.knum);
		//printf(tmp,bitrev(r.stat,4*conf.knum),bitcount(r.stat),dp[4*conf.knum][r.stat],target);
		printf(tmp,bitrev(r.stat,4*conf.knum),bitcount(r.stat),dp2[r.stat],target);
	}
	
	//�L�[���͑҂�
	if(conf.pauseOnExit){
		puts("hit enter to exit.");
		getchar();
	}
	
	return 0;
}

//�͍ڋ@������e�X���b�g�̐���l���v�Z
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
//on�r�b�g�̌v��
int bitcount(unsigned int a){
	int cnt=0;
	while(a){ cnt+=(a&1); a>>=1;}
	return cnt;
}
//n���̃r�b�g�̕��я����t���ɂ��� n<=31
unsigned int bitrev(unsigned int a, int n){
	unsigned int b=0;
	for(int i=0;i<n;++i,(a>>=1),(b<<=1)){
		b|=(a&1);
	}
	return b>>1;
}

