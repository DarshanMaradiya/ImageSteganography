/*
This code is completely written from scratch after understanding theoretically!
This is not copied from any source.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.142857

int displayBlck(int*);
int shifting(int*,int*);
int DCT(int*,float*);
int displayDCT(float[][8]);
int Quantization(float*,int*);
int IDCT(int*,int*);
int Rev_Quantization(int*,int*);
int Reshifting(int*,int*);

int Quant[8][8]={
{16,11,10,16,24,40,51,61},
{12,12,14,19,26,58,60,55},
{14,13,16,24,40,57,69,56},
{14,17,22,29,51,87,80,62},
{18,22,37,56,68,109,103,77},
{24,35,55,64,81,104,113,92},
{49,64,78,87,103,121,120,101},
{72,92,95,98,112,100,103,99}};

int main()
{	int i,j;
	int ShiftedBlck[8][8];
	float DCT_Co[8][8];
	int IDCT_Co[8][8];
	int quantized[8][8];
	int Rquantized[8][8];
	int Re_ShiftedBlck[8][8];
	
	//	YOUTUBE
/*	
	int SmplBlck[8][8]={
	{62,55,55,54,49,48,47,55},
	{62,57,54,52,48,47,48,53},
	{61,60,52,49,48,47,49,54},
	{63,61,60,60,63,65,68,65},
	{67,67,70,74,79,85,91,92},
	{82,95,101,106,114,115,112,117},
	{96,111,115,119,128,128,130,127},
	{109,121,127,133,139,141,140,133}};
	
*/
//	PPT

	int SmplBlck[8][8]={
	{99,127,145,121,89,65,66,99},
	{60,78,97,99,94,99,89,73},
	{38,39,51,72,91,120,122,89},
	{69,47,46,60,83,116,134,126},
	{116,85,56,48,58,82,101,112},
	{148,133,88,49,29,35,47,66},
	{90,94,111,93,60,34,28,36},
	{35,65,117,135,112,63,28,23}};

/*
	int SmplBlck[8][8]={
{ 255, 255, 255, 255, 255, 255, 255, 255 }, 
{ 255, 255, 255, 255, 255, 255, 255, 255 },
{ 255, 255, 255, 255, 255, 255, 255, 255 }, 
{ 255, 255, 255, 255, 255, 255, 255, 255 }, 
{ 255, 255, 255, 255, 255, 255, 255, 255 }, 
{ 255, 255, 255, 255, 255, 255, 255, 255 }, 
{ 255, 255, 255, 255, 255, 255, 255, 255 }, 
{ 255, 255, 255, 255, 255, 255, 255, 255 } };

*/
	
	//	Sample Block-----------------------------------------------
	printf("\nSample Block :\n\n");
	displayBlck(&SmplBlck[0][0]);
	printf("\n\n");
	
	//	Quantization Constants 
	printf("\nBlock of quantization :\n\n");
	displayBlck(&Quant[0][0]);
	
	//	shifting by 128-------------------------------------------
	shifting(&SmplBlck[0][0],&ShiftedBlck[0][0]);
	
	printf("\nShifted Block :\n\n");
	displayBlck(&ShiftedBlck[0][0]);
	

	//	Applying DCT and stores its coefficients------------------
	printf("\nDoing DCT :\n");
	DCT(&ShiftedBlck[0][0],&DCT_Co[0][0]);
	
	printf("\nDCT Block :\n\n");
	displayDCT(&DCT_Co[0][0]);	
	
	// Quantization-----------------------------------------------
	printf("\nDoing Quantization :\n");	
	Quantization(&DCT_Co[0][0],&quantized[0][0]);
	
	printf("\nQuantized Block :\n");
	displayBlck(&quantized[0][0]);
	
	printf("\n<<<<<<<<< Embedding Work >>>>>>>>>\n");
	printf("\nAfter completing quantization on every 8*8 block,\nWe here generate binary string of our `secret message` using huffman encoding based on the frequency");
	printf("\nThen we embed these bits into the quantized image array-value's LSB(Least Significant bit)\nwhere array-value is non-zero and also not equal to 1 or -1.");
	printf("\nThen we do the reverse procedure we have followed so far as follows!\n");
	printf("\n<<<<<<<<< >>>>>>>>>\n");
	
	// Reverse Quantization-----------------------------------------------
	printf("\nDoing Reverse Quantization :\n");	
	Rev_Quantization(&quantized[0][0],&Rquantized[0][0]);
	
	printf("\nReverse Quantized Block :\n");
	displayBlck(&Rquantized[0][0]);

	//	Applying IDCT and getting back coefficients---------------
	printf("\nDoing IDCT :\n");
	IDCT(&Rquantized[0][0],&IDCT_Co[0][0]);
	
	printf("\nIDCT Block :\n\n");
	displayBlck(&IDCT_Co[0][0]);
	
	//	Re-Shifting-----------------------------------------------
	printf("\nRe-Shiftig : \n");
	Reshifting(&IDCT_Co[0][0],&Re_ShiftedBlck[0][0]);
	displayBlck(&Re_ShiftedBlck[0][0]);
			
}

int displayBlck(int *array)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(*(array+(i*8+j)) >= 0) 
			{
				printf("+");
				printf("%03d ",*(array+(i*8+j)));
			}
			else
			{
				printf("-");
				printf("%03d ",abs(*(array+(i*8+j))));
			}
		}
		printf("\n");
	}
	
	return 0;
}

int shifting(int *SrcBlck,int *TrgtBlck)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			*(TrgtBlck+(i*8+j))=*(SrcBlck+(i*8+j))-128;
		}
	}
	
	return 0;
}

int DCT(int *SrcBlck,float *TrgtBlck)
{
//	printf("\nEntered in DCT()");
	int u,v,x,y,c=0;
	float C_u,C_v,DCT[8][8],sum;
	
	for(u=0;u<8;u++)
	{
		
		for(v=0;v<8;v++)
		{	
			if(u==0) C_u=(1.0/sqrt(2));
			else C_u=1.0;
			
			if(v==0) C_v=(1.0/sqrt(2));
			else C_v=1.0;
			
			sum=0;
			
			for(x=0;x<8;x++)
			{
				for(y=0;y<8;y++)
				{
					sum+=( (*(SrcBlck+(x*8+y))) * cos( (2*(x) + 1) * (u) * PI / 16 ) * cos( (2*(y) + 1) * (v) * PI / 16 ) );
//					printf("\n%d",c++);
				}
			}
			
			*(TrgtBlck+(u*8+v))=(sum*(1/4.0)*C_u*C_v);

			
		}
	}
	return 0;
}

int displayDCT(float array[][8])
{
	int u,v;
	for(u=0;u<8;u++)
	{
		for(v=0;v<8;v++)
		{
		//	printf("%f ",array[u][v]);
			
			if(array[u][v] >= 0.0) 
			{
				printf("+");
				printf("%07.2f ",floor(array[u][v]));
			}
			else
			{
				printf("-");
				printf("%07.2f ",fabs(floor(array[u][v])));
			}
		}
		printf("\n");
	}
	
	return 0;
}

int Quantization(float *SrcBlck,int *TrgtBlck)
{
	int u,v;
	for(u=0;u<8;u++)
	{
		for(v=0;v<8;v++)
		{
			*(TrgtBlck+(u*8+v))=(*(SrcBlck+(u*8+v))/Quant[u][v]);
//			printf("\n %f",*(TrgtBlck+(u*8)+v));
		}
	}
	return 0;
}

int Rev_Quantization(int *SrcBlck,int *TrgtBlck)
{
	int u,v;
	for(u=0;u<8;u++)
	{
		for(v=0;v<8;v++)
		{
			*(TrgtBlck+(u*8+v))=(*(SrcBlck+(u*8+v))*Quant[u][v]);
//			printf("\n %f",*(TrgtBlck+(u*8)+v));
		}
	}
	return 0;
}

int IDCT(int *SrcBlck,int *TrgtBlck)
{
	int u,v,x,y;
	float C_u,C_v,sum;
	
	for(x=0;x<8;x++)
	{
		
		for(y=0;y<8;y++)
		{	
			sum=0;
			
			for(u=0;u<8;u++)
			{
				for(v=0;v<8;v++)
				{
					if(u==0) C_u=(1.0/sqrt(2));
					else C_u=1.0;
			
					if(v==0) C_v=(1.0/sqrt(2));
					else C_v=1.0;
			
					sum+=(int)( (*(SrcBlck+(u*8+v))) *C_u * C_v * cos( (2*(x) + 1) * (u) * PI / 16 ) * cos( (2*(y) + 1) * (v) * PI / 16 ) );
				}
			}
			
			*(TrgtBlck+(x*8+y))=(int)(sum/(4));
			
		}
	}
	return 0;
}

int Reshifting(int *SrcBlck,int *TrgtBlck)
{
	int i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			*(TrgtBlck+(i*8+j))= *(SrcBlck+(i*8+j))+128;
		}
	}
	
	return 0;
}

