#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int sum(int *A, int M,int N)
{
 int i,j;
 long int s=0;
 for(i=0;i<M;i++)
	 for(j=0;j<N;j++)
		 s += A[i*M+j];
 return s;
}

char * zeropad(int k, int Max)
{
 int n1 = log10(Max);
 
 int n2;
 if(k==0)
 	n2=1;
 else 
 	n2 = log10(k);

 char *s = (char*)malloc((n1+1)*sizeof(char));
 int i;
 for(i=0;i<(n1-n2);i++)
 	s[i]='0';
 if(k!=0)
	 sprintf(s,"%s%d%c",s,k,'\0');
 else
	 sprintf(s,"%s00%c",s,'\0');
 return s;
}

void fprintiM(int *A, int M,int N, char*filename)
{
 int i,j;
 FILE *fp;
 fp=fopen(filename,"w");
 for(i=0;i<M;i++)
 	{
	 fprintf(fp,"\n");
	 for(j=0;j<N;j++)
		{
		 fprintf(fp,"%d\t",A[i*M+j]);
		}
	}
 fprintf(fp,"\n");
 fclose(fp);
}

void printiM(int *A, int M,int N)
{
 int i,j;
 for(i=0;i<M;i++)
 	{
	 printf("\n");
	 for(j=0;j<N;j++)
		{
		 printf("%d\t",A[i*M+j]);
		}
	}
 printf("\n");

}

int* iMrandom(int M,int N, double p)
{
 int *A;
 double at;

 A = (int*)calloc(M*N, sizeof(int));

 if(fabs(p-0)<(float)(1/(M*N)))
 	return A;
 else
 	{
	 int i,j;
	 for(i=0;i<M;i++)
		 for(j=0;j<N;j++)
		 	{
			 at = drand48();
			 if(at<=p)
			 	A[i*M+j]=1;
			}
	}
 	
 return A;
}

int max(int a,int b)
{
 return ((a>=b)?a:b);
}

int min(int a,int b)
{
 return ((a<=b)?a:b);
}

int main(int argc, char*argv[])
{
 if(argc==1)
 	{
	 printf("Usage:./life Ngen M N p\nNgen = Number of generations\nM,N = World dimensions\np = intial percentage occupation\n");
	 return;
	}
 
 int Ngen,M,N;
 double p;

 Ngen = atoi(argv[1]);
 M = atoi(argv[2]);
 N = atoi(argv[3]);
 p = atof(argv[4]);
 
 //printf("%d %d %d %f\n",Ngen,M,N,p);

 int *this_gen,*next_gen,i,j,k;
 int istart, jstart, iend, jend;//*H;
 int h1,h2,y,z;
 int *tgen;
 char name[100];

 this_gen = iMrandom(M,N,p);
 next_gen = iMrandom(M,N,0);
 
 for(k=0;k<Ngen;k++)
 	{
	 sprintf(name,"/tmp/Life%s.dat",zeropad(k,Ngen));
	 fprintiM(this_gen, M, N,name);
	 for(i=0;i<M;i++)
	 	{
		 //printf("\n");
		 for(j=0;j<N;j++)
		 	{
			 istart = max(0,i-1);
			 jstart = max(0,j-1);
			 
			 iend = min(i+1,M-1);
			 jend = min(j+1,N-1);
			 //H = (int*)malloc(sizeof(int)*(iend-istart+1)*(jend-jstart+1));
			 for(y=0,h1=istart;h1<(iend+1);h1++)
				 for(h2=jstart;h2<(jend+1);h2++)
				 	{
 					// H[t] = this_gen[(h1)*M+(h2)];
 					 y +=  this_gen[(h1)*M+(h2)];
					// printf("%d\t", H[t]);
					}
			 z = this_gen[i*M+j];
			 y -= z;
 			 next_gen[i*M+j] = z*((y<2 | y>3)*0 + (y==2 | y==3)*1) + (1-z)*((y==3)*1 + (y!=3)*0);

			 //printf("\n");
			 //printiM(H,(iend-istart+1),(jend-jstart+1));
			 //free(H);
			 //printf("%d\t",y);
			}
		}
	 //printf("\n");
	 tgen = this_gen;
	 this_gen = next_gen;
	 next_gen = tgen;
	}		

}
