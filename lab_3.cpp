#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<iostream>
using namespace std;

double t

inline long Strike_num(bool composite[],long i,long stride,long limit){
                                                                                                                                                                                for(;i<=limit;i+=stride)                                                                                                                                                        composite[i]=true;
 return i;
}

long unfriendly(long n)
{
        long count=0;
 bool* composite[]=new bool[n+1];
 memset(composite,0,n);
 long m=(long)sqrt((double)n);
t=omp_get_wtime();
for(long i=2;i<=m;i++)
{
 if(!composite[i])
 {
         count++;
         Strike_num(composite,2*i,i,n);
 }

}

for(long i=m+1;i<=n;i++)
  {
    if(!composite[i])
            count++;
  }

t=omp_get_wtime()-t;
delete[] composite;

return count;

}

long friendly(long n){
long count=0;
bool* composite[]=new bool[n+1];
long m=(long)sqrt((double)n);
long* striker[]=new long[m];
long* factor[]=new long[m];
long n_factor=0;
memset(composite,0,n);
t=omp_get_wtime();
for(long i=2;i<=m;i++)
{
 if(!composite[i])
 {
   count++;
  striker[n_factor]=Strike_word(composite,2*i,i,m);
  factor[n_factor++]=i;
 }
}

 for(long window=m+1;window<=n;window+=m){
   long limit=min(window+m-1,n);
   for(long k=0;k<n_factor;k++)
           striker[k]=Strike_word(composite,striker[k],factor[k],limit);
  for(long k=window;k<=limit;k++)
          if(!composite[k])
                  count++;

 }

t=omp_get_wtime()-t;

delete[] composite;
delete[] striker;
delete[] factor;

return count;

}

long parallel_sieve(long n,int t){

 long count = 0;
 long m = (long)sqrt((double)n);

 long n_factor = 0;
 long* factor = new long[m];
 t = omp_get_wtime();
#pragma omp parallel

 {  bool* composite[]=new bool[m+1];
         long* striker[]=new long[m];
#pragma omp single
 for(long i=2;i<=m;i++)
 {
    count++;
    striker[n_factor]=Strike_word(composite,2*i,i,m);
    factor[n_factor++]=i;

 }

 long base=-1;
#pragma omp for reduction(+:count)
 for(long window=m+1;window<=n;window+=m){

         if(base!=window)
         {
           base=window;
           for(long k=0;k<n_factor;k++)
                   striker[k]=(base+factor[k]-1)/factor[k]*factor[k]-base;
         }


         long limit=min(window+m-1,n)-base;
               for(long k=0;k<n_factor;k++)
                                  striker[k]=Strike_word(composite,striker[k],factor[k],limit)-m;
                 for(long k=0;k<=limit;k++)
                                   if(!composite[k])
                                             count++;
                            base+=m;
                  }
delete[] composite;
delete[] striker;

 }

 t=omp_get_wtime()-t;

 delete[] factor;

 return count;

}

int main()
{ long n;
 int t;
cout<<"Enter n"<<endl;
cin>>n;

cout<<"Cache Unfriendly Sieve"<<endl;
cout<<"COUNT:"<<unfriendly(n)<<endl;
cout << "Time = " << t << endl;



cout<<"Cache Friendly Sieve"<<endl;
cout<<"COUNT:"<<friendly(n)<<endl;
cout << "Time = " << t << endl;


cout<<"Parallel Sieve"<<endl;
cout<<"COUNT:"<<parallel_sieve(n,t)<<endl;
cout << "Time = " << t << endl;

}
