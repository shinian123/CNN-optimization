#include <stdio.h>
#include "ap_cint.h"
typedef uint16 data_type;
#define N 10

void Conv(data_type cc[(N-3)*(N-6)*3])
{
      //数值左边一半比右边一半稠密。
	   data_type  Data[N][N] = {{2,1,2,1,0,2,1,0,1,0},{1,0,3,1,4,1,0,3,1,0},{2,2,0,0,0,2,2,0,0,0},{1,1,1,0,1,1,1,1,0,0},{0,0,2,1,0,0,0,0,1,0},
			                    {2,1,2,1,0,2,1,0,1,0},{1,0,3,1,4,1,0,3,1,0},{2,2,0,0,0,2,2,0,0,0},{1,1,1,0,1,1,1,1,0,0},{0,0,2,1,0,0,0,0,1,0}};
      //三个3x3权重

	   data_type  W[3][3][3] = {{{1,0,1},{1,1,0},{1,1,1}},{{1,1,0},{1,0,1},{1,1,1}}, {{1,1,1},{1,0,1},{1,1,0}}};

	   data_type i1,j1;
	   data_type i2,j2;

	   data_type  W1[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	   data_type  W2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};


	   data_type outres[3][(N-3)*(N-6)];

	   data_type flag1=1,flag2=1;
	   data_type cnt1=0,cnt2=0;


 /*******************************************负载均衡切换权重*********************************************/
Conv_label11:for(data_type p=0;p<3;p++) //计算3个kernel的feature map
{

	    if(flag1==1)
		  for(data_type n1=0;n1<3;n1++)
		  {
#pragma HLS unroll
			  W1[n1][0]=W[cnt1][n1][0];
			  W1[n1][1]=W[cnt1][n1][1];
			  W1[n1][2]=W[cnt1][n1][2];
		  }

	     if(flag2==1)
	 		  for(data_type n2=0;n2<3;n2++)
	 		  {
#pragma HLS unroll
			  W2[n2][0]=W[cnt2][n2][0];
			  W2[n2][1]=W[cnt2][n2][1];
			  W2[n2][2]=W[cnt2][n2][2];

	 		  }

////////////////////////////////////////计算单元1////////////////////////////////////////////////
for(data_type p2=0; p2<2;p2++)
{
#pragma HLS unroll

  if(p2==0)
  {
	   for(i1 = 0;i1<N-3;i1++)
	    {
	       if(cnt1%2==0)
	        {
		    Conv_label5:for(j1 = 0;j1<N/2-3;j1++)
	        {
		    	 data_type sum1 = 0;
		    	 data_type  TEMP1;
	        	for(int ii1=0;ii1<3;ii1++)
	        	{
#pragma HLS unroll

	        		for(int jj1=0;jj1<3;jj1++)
	        		{
	        		 if((W1[ii1][jj1]==0)||(Data[i1+ii1][j1+jj1]==0))
	        		 {
	        			 ;
	        		 }
	        		 else
	        		 {
	        		 TEMP1=Data[i1+ii1][j1+jj1]*W1[ii1][jj1];
	        		 sum1 += TEMP1;
	        		 }
	        		}
	        	}
	        	outres[cnt1][i1*N+j1] = sum1;
	        }
	        }else
	        {
	        	 Conv_label6:for(j1 = N/2;j1<N-3;j1++)
	        	 {
	        		data_type sum1 = 0;
	        		data_type  TEMP1;
	        		for(int ii1=0;ii1<3;ii1++)
	        		{
#pragma HLS unroll

	        		    for(int jj1=0;jj1<3;jj1++)
	        		       {

	        		        if((W1[ii1][jj1]==0)||(Data[i1+ii1][j1+jj1]==0))
	        		        	 {
	        		        	;
	        		        	 }
	        		        else
	        		           {
	        		             TEMP1=Data[i1+ii1][j1+jj1]*W1[ii1][jj1];
	        		        	 sum1 += TEMP1;
	        		            }
	        		        	}
	        		       outres[cnt1][i1*N+j1] = sum1;
	        		        }
	        	 }


	        }

            if(i1==(N-2))
            {
	           flag1=1;
	           cnt1++;
            }
            else
               flag1=0;
	    }
  }
 /*************************************计算单元2********************/
 if(p2==1)
 {

	  for(i2 = 0;i2<N-3;i2++)
	    {
		   if(cnt2%2==1)
		   {
	        Conv_label7:for(j2 = 0;j2<N/2-3;j2++)
	        {

	        	data_type sum2 = 0;
	        	data_type  TEMP2;
	        	for(int ii2=0;ii2<3;ii2++)
	        	{
#pragma HLS unroll

	        		for(int jj2=0;jj2<3;jj2++)
	        		{

	        			 if((W2[ii2][jj2]==0)||(Data[i2+ii2][j2+jj2]==0))
	        				{
	        				  ;
	        				 }
	        			 else
	        				 {
	        				 TEMP2=Data[i2+ii2][j2+jj2]*W2[ii2][jj2];
	        			     sum2 += TEMP2;
	        				}
	        		}
	        	outres[cnt2][i2*N+j2] = sum2;
	        	}
	        }
		   }
		   else
		   {

			 Conv_label8:for(j2 = N/2;j2<N-3;j2++)
			  	    {

			  		    data_type sum2 = 0;
			  	        data_type  TEMP2;
			  	     for(int ii2=0;ii2<3;ii2++)
			  	     {

#pragma HLS unroll

			  	        for(int jj2=0;jj2<3;jj2++)
			  	        		{
			  	        			 if((W2[ii2][jj2]==0)||(Data[i1+ii2][j2+jj2]==0))
			  	        				{
			  	        				  ;
			  	        				 }
			  	        			 else
			  	        				 {
			  	        				 TEMP2=Data[i2+ii2][j2+jj2]*W2[ii2][jj2];
			  	        			     sum2 += TEMP2;
			  	        				}
			  	        		}
			  	        	outres[cnt2][i2*N+j2] = sum2;
			  	        }
			  	    }


		   }

	        if(i2==(N-2))
	        {
	       	     flag2=1;
	       	     cnt2++;
	        }
	         else
	        	 flag2=0;

	    }
 }

}
}

/*****************************结果合并输出****************************************/

     Conv_label9:for(data_type k=0;k<(N-3)*(N-6);k++)
     {
    #pragma HLS pipeline
    	 Conv_label10:for(data_type k2=0;k2<3;k2++)
    	 {
      #pragma HLS unroll
    	   cc[k]=outres[k2][k];
     }
     }

}



