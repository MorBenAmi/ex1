#include <stdio.h>
#include "TransformHammingBlock.h"

void TransformHammingBlock(unsigned char block[8], unsigned char hamming_block[8])
{
	int i = 0; //parity bit index(2,4,8,16,32..)
	int j = 0; //data bit index( 3,5,6,7,9,10,11,...)
	int k = 1; //original block bit index(1,2,3,4,5,6,7,8,9,10,...)
	int m = 0; //power of 2(0,1,2,3,4,5)
	int n = 0; // bit index for parity check (so we won't change original m)
	int parity_index = 0;

	unsigned char bit = 0;//the k bit in block
	unsigned char bit_parity=0;
	unsigned char bit_parity_check=0;
	unsigned char parity = 0;

	for(i = 2; i < 64; i = i*2)
	{
		m++;
		for(j = i+1; j < i*2; j++)
		{
			bit = 1 << (8-((k-1)%8));
			bit = block[(k-1)/8] & bit;
			bit = bit >> (8-((k-1)%8));
			bit = bit << (8-((j-1)%8));
			hamming_block[(j-1)/8] = hamming_block[(j-1)/8] | bit;
			bit = bit >> (8-((j-1)%8));

			parity_index = 1;
			for(n = 0; n <= m; n++)
			{
				//check if bit m in j is 1
				bit_parity=1 << n;
				if(bit_parity == (j & bit_parity))
				{
					bit = bit << (8-((parity_index-1)%8));
					hamming_block[(parity_index-1)/8] = hamming_block[(parity_index-1)/8] ^ bit;
					bit = bit >> (8-((parity_index-1)%8));
				}
				parity_index = parity_index * 2;
			}
			k++;
		}
	}
}
