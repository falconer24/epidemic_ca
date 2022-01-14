#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// Find the sum of elements of an integer matrix of MxN
int sum(int *A, int M,int N)
{
 int i,j;
 long int s=0;
 for(i=0;i<M;i++)
	 for(j=0;j<N;j++)
		 s += A[i*M+j];
 return s;
}

// Print the given matrix A, MxN in the file called filename
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

// Print the given string to the file called name;

void fprintstring(char *s, char *name)
{
 FILE *fp;
 fp=fopen(name,"a");
 fprintf(fp,"%s",s);
 fclose(fp);
 
}
// Just like fprintiM but prints to console
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
 printf("\n\n");
}

// Generate an integer matrix of MxN with uniformly distrubuted, random, p percent of elements being 1, the rest being 0.
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


// Population Parameters
 #define MAX_POP 40000
 #define MAX_RURAL 4000
 #define MIN_URBAN 8000
 #define MIN_RURAL 1000
 // Size of Urban areas is SIZE_URBAN x SIZE_URBAN contiguous squares
 #define SIZE_URBAN 5
// Migration parameters
 #define MIGRATION_RATE_RURAL 0.05
 #define MIGRATION_RATE_RANDOM_RURAL 0.01
 #define MIGRATION_RATE_RANDOM_URBAN 0.01
 #define MIGRATION_RATE_URBAN 0.00
 #define MAX_MIGRATION_RADIUS 20
 #define MIGRATION_RATE_FESTIVAL 0.05
 #define FEST_START 200
 #define FEST_END (FEST_START+20)

// Disease Parameters
#define INFECTION_RATE 0.4
#define RECOVERY_RATE 0.1

// Randomly populate the world
int* populate_random(int M,int N, double p)
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
			 	A[i*M+j]= MIN_URBAN +at*(MAX_POP-MIN_URBAN) ;
			 else
			 	A[i*M+j]= MIN_RURAL + at*MAX_RURAL;

			}
	}
 	
 return A;
}

// Pick up the population distribution from a file
int * populate_from_file(int M,int N,char*filename)
{
 FILE *fp; 
 int *A;
 fp=fopen(filename,"r");
 A = (int*)calloc(M*N, sizeof(int));

 int i,j;
 for(i=0;i<M;i++)
	 for(j=0;j<N;j++)
		{
		 fscanf(fp,"%d",&A[i*M+j]);
		}
 fclose(fp);
 return A;
}

// Matrix copy

void Mcopy(int*dest, int*src,int M, int N)
{
 int i,j;
 for(i=0;i<M;i++)
	 for(j=0;j<N;j++)
	 	dest[i*M+j] = src[i*M+j];
 return ;
}

// Find the smaller and larger of given numbers
// Used for getting the starting and ending indices of neighbourhoods of boundary line elements.

int max(int a,int b)
{
 return ((a>=b)?a:b);
}

int min(int a,int b)
{
 return ((a<=b)?a:b);
}

// Main
int main(int argc, char*argv[])
{
 if(argc!=6)
 	{
	 printf("Usage:./life Ngen M N Pfile Ifile\nNgen = Number of generations\nM,N = World\
	 dimensions\nPfile = Initial distribution of population\nIfile = Initial distribution of\
	 infectious patients.");
	 return;
	}
 
 // All variable declarations
 int 	Ngen,				// Number of iterations for which to run the simulation
 	M,				// Number of Rows in the world
	N;				// Number of Columns in the world
 double p;				// Percentage of the columns that are urban -- used only when using random population distribution

 int 	cell_index,i,j,k,l,		// Matrix Index variables
	t,h1,h2,y,z;

 int 	*pop, 				// Population Matrix
 	*temp_pop, 			// Temporary pointer
	*native, 			// Hold the original state of the population
	*S,				// Number of susceptibles
	*I,				// Number of infected and infectious patients
	*R, 				// Number of recovered patients
					
	*PI;				// Percentage of Infectious to the total population
					// This is what we want to observe -- our main output

 float 	ms,				// Used in SIR with migration
 	mi,
	mr;

 int 	istart, 			// Used in finding the migration neighbourhood
 	jstart, 
	iend, 
	jend,
	found_city,
	*H;				// Used for temporarily storing the neighbourhood

 char* 	infile;				// Holds name of input files
 char 	name[100]; 			// Holds names of output files
 
 float 	t1, t2;				// Temporary -- used in SIR model

 Ngen = atoi(argv[1]);			// Getting the input
 M = atoi(argv[2]);
 N = atoi(argv[3]);
 //p = atof(argv[4]);
 infile = argv[4];
 
 //printf("%d %d %d %f\n",Ngen,M,N,p);

 name[0]='\0';
 
 pop = populate_from_file(M,N,infile);	// One big crowded city surrounded by rural areas

 //pop = populate_random(M,N,p);		// A random distribution of rural and urban with
 //printiM(pop, M, N);
 	
 native = (int*)malloc(sizeof(int)*M*N);
 Mcopy(native,pop,M,N);			// The matrix native stores the original state of
						// population distribution.
 //printiM(pop, M, N);

 // Initialise matrices for S, I and R. Each element S/I/R_{i,j} is the S, I and R for the
 // pop{i,j}

  S = (int*)calloc(M*N, sizeof(int));
  Mcopy(S,pop,M,N);		// Initially, everyone is susceptible
  I = populate_from_file(M,N,argv[5]);		// Random spatial distribution of infected people
  for(i=0;i<M;i++)
	 for(j=0;j<N;j++)
	 	S[i*M+j] -= I[i*M+j];		// Initial infectious people are removed from S;

  PI = (int*)calloc(M*N, sizeof(int));
  R = (int*)calloc(M*N, sizeof(int));

 for(k=0;k<Ngen;k++)
 	{
	 sprintf(name,"/tmp/Population%03d.dat",k);
	 fprintiM(pop, M, N,name);
	
	// Constant random migration is always there.
	 for(i=0;i<M;i++)
	 	{
		 for(j=0;j<N;j++)
		 	{
			 cell_index = i*M+j;

			 h1 = rand()%(M-1);
			 h2 = rand()%(N-1);
			 if(pop[cell_index] <= MAX_RURAL && pop[h1*M+h2]<=MAX_RURAL)		// rural to rural and urban to urban only
			 	{
				 y = (MIGRATION_RATE_RANDOM_RURAL*(float)pop[cell_index]);
				 if(pop[cell_index] - y<MIN_RURAL)
					y = pop[cell_index] - MIN_RURAL;
				 pop[h1*M+h2] += y;	// Migration actual
				 pop[cell_index] -= y;	

				 ms = y*(float)S[cell_index]/pop[cell_index];		// Calculate the fraction of migrants from S category
				 mi = y*(float)I[cell_index]/pop[cell_index];		// Do the same for I category
				 mr = y*(float)R[cell_index]/pop[cell_index];		// and for R category

			 	//printf("%f %f %f\n",ms,mr,mi);

				 S[h1*M+h2] += ms;		// Update the susceptible populations 
				 S[cell_index]   -= ms;
				 
				 I[h1*M+h2] += mi;		// Update the infected
				 I[cell_index]   -= mi;
				 
				 R[h1*M+h2] += mr;		// and the recovered
				 R[cell_index]   -= mr;
				}
			 if(pop[cell_index] >= MIN_URBAN && pop[h1*M+h2] >= MIN_URBAN)		// rural to rural and urban to urban only
			 	{
				 y = (MIGRATION_RATE_RANDOM_URBAN*(float)pop[cell_index]);
				 if(pop[cell_index] - y<MIN_RURAL)
					y = pop[cell_index] - MIN_RURAL;
				 pop[h1*M+h2] += y;
				 pop[cell_index] -= y;
				 
				 ms = y*(float)S[cell_index]/pop[cell_index];		// Calculate the fraction of migrants from S category
				 mi = y*(float)I[cell_index]/pop[cell_index];		// Do the same for I category
				 mr = y*(float)R[cell_index]/pop[cell_index];		// and for R category

				 S[h1*M+h2] += ms;		// Update the susceptible populations 
				 S[cell_index]   -= ms;
				 
				 I[h1*M+h2] += mi;		// Update the infected
				 I[cell_index]   -= mi;
				 
				 R[h1*M+h2] += mr;		// and the recovered
				 R[cell_index]   -= mr;
				}
			}
		}

	// Periodic and temporary migration from urban to rural -- like during
	// diwali/urus time.
	// When the fest starts, pop is pointed to native.
	/*if(k==FEST_START)
		{
	 //	fprintstring("\nFestival Begins\n",name);
		 temp_pop = pop ;		// so that we don't lose the current state.
		 pop = native;
		}
	if(k==FEST_END)
		{
	//	 fprintstring("\nFestival Ends\n",name);
		 pop = temp_pop;		// everyone goes back to work
		}
	 */
	 //printiM(pop, M, N);
	 // Implement Migration Model
	 // Simple model -- Migration is at a constant rate from rural to urban area only
	 // There can be no migration to a urban cell which has exceeded capacity or from a rural
	 // cell which has too few members
	 // No migration beyond a radius M/2
	 //if(k<FEST_START || k>FEST_END) // Migration from rural to urban at all times except festival
	 {
	 for(i=0;i<M;i++)
	 	{
		 //printf("\n");
		 for(j=0;j<N;j++)
		 	{
			 cell_index = i*M+j;
			 // For every cell in the world, check if it is rural or urban
			 if(pop[cell_index]<= MAX_RURAL)
			 	{
				 // If rural, migrate according to Rate to the nearest city with
				 // enough space
				 found_city=0;		// Flag
				 l=1;		// Radius to search in
				 while(!found_city)
					 {
					 istart = max(0,i-l);		// Get the neighbourhood
					 				// boundary according to radius
					 jstart = max(0,j-l);
					 
					 iend = min(i+l,M-1);
					 jend = min(j+l,N-1);
					 H = (int*)malloc(sizeof(int)*(iend-istart+1)*(jend-jstart+1));
					 // Search neighbours in current radius for city. Go one by
					 // one
					 for(t=0,y=0,h1=istart;h1<(iend+1);h1++)
						 {
						  //printf("\n");
						  for(h2=jstart;h2<(jend+1);h2++)
							{
						 	 H[t] = pop[(h1)*M+(h2)];
						 	 //printf("%d\t", H[t]);
							 
							 // Check if the neighbour meets urbanity
							 // criterion
						 	 if(H[t] >= MIN_URBAN)
						 		{
								 //printf("\nNearest urban center = %d %d, Radius = %d",h1,h2,l);

								 // How many will migrate ?
								 y = (MIGRATION_RATE_RURAL*(float)pop[cell_index]);
								 // Migrate but make sure that the
								 // village is not completely
								 // depopulated
								 if(pop[cell_index] - y<MIN_RURAL)
								 	y = pop[cell_index] - MIN_RURAL;
								 pop[h1*M+h2] += y;
								 pop[cell_index] -= y;
								 
								 ms = y*(float)S[cell_index]/pop[cell_index];		// Calculate the fraction of migrants from S category
								 mi = y*(float)I[cell_index]/pop[cell_index];		// Do the same for I category
								 mr = y*(float)R[cell_index]/pop[cell_index];		// and for R category

								 S[h1*M+h2] += ms;		// Update the susceptible populations 
								 S[cell_index]   -= ms;
								 
								 I[h1*M+h2] += mi;		// Update the infected
								 I[cell_index]   -= mi;
								 
								 R[h1*M+h2] += mr;		// and the recovered
								 R[cell_index]   -= mr;
								 
								 found_city=1;
								 break;
								}
							}
						  if (found_city) break;
						 }
					 if(!found_city) 
					 	{
						 //printf("\nNo city in radius %d",l);
						 if(l>=MAX_MIGRATION_RADIUS) 
						 	found_city=1;
						 l++;
						}
					// printf("\n");
					 //printiM(H,(iend-istart+1),(jend-jstart+1));
					 free(H);
					 //printf("%d\t",y);
					 }
				
				}// Migration From rural to Urban ends
			 //printf("\n");

			}
		}
	 }//festival check if statement ends
	 //printf("\n");

	 // Migration model ends. Implement the epidemic model within each cell
	
	// Epidemic Model
	// SIR Model using ODEs solved by Eulers Method.
	
	// Print the current matrix of Percentage Infected into a file
	sprintf(name,"/tmp/Infected%03d.dat",k);
	fprintiM(PI, M, N,name);
	
	for(i=0;i<M;i++)
		{
		// printf("\n");
		 for(j=0;j<N;j++)
		 	{
			 cell_index = i*M+j;
			 t1 = INFECTION_RATE*(float)(S[cell_index]*I[cell_index])/pop[cell_index];
			 t2 = (RECOVERY_RATE*(float)I[cell_index]);
			 I[cell_index] += t1 - t2;
			 S[cell_index] -= t1;
			 R[cell_index] += t2;
			 //printf("%f %f %d %d %d\n",t1,t2,S[cell_index],I[cell_index],R[cell_index]);
			 PI[cell_index] = (100*I[cell_index]) / pop[cell_index];
			 //printf("%d\t",I[cell_index]);
			}
		}
	 //printf("\n");

	}		

}
