/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 *	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

	/* This file contains the stub for a predictive pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time

	/* Local vars */
	int proc;
	int pc;
	int page;

	double pagePattern[15][15] = 
	{    
	// 0     1   2   3   4   5   6   7   8   9    10   11  12   13  14
		{0.0  ,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //0
		{0.0  ,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //1
		{0.0  ,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //2
		{0.25 ,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.0 ,0.25,0.0,0.0 ,0.0,0.0  }, //3
		{0.0  ,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //4
		{0.0  ,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //5
		{0.0  ,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //6
		{0.0  ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //7
		{0.5  ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //8
		{0.0  ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,1.0 ,0.0,0.0 ,0.0,0.0  }, //9
		{0.0  ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,1.0,0.0 ,0.0,0.0  }, //10
		{0.25 ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.75,0.0,0.0  }, //11
		{0.0  ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,1.0,0.0  }, //12
		{0.125,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.75,0.0 ,0.0,0.0 ,0.0,0.125}, //13
		{1.0  ,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }  //14
	};


	/* initialize static vars on first run */
	if(!initialized){
		/* Init complex static vars here */

		initialized = 1;
	}

	/* TODO: Implement Predictive Paging */
	for(proc=0;proc<MAXPROCESSES;proc++){
		int i,j,k;

		/* Keep track of which pages we should swap in/out, 1 = swap in, 0 = swap out */
		int pageSwap[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		/* Get the program counter */
		pc = q[proc].pc;
		/* Get the current page */
		page = pc/PAGESIZE;

		/* set the page we currently need to 1 */
		pageSwap[page] = 1;

		// Branch loop
		for(i = 0; i < 15; i++){
			if(pagePattern[page][i] > 0){
				pageSwap[i] = 1;
			}
			if(pagePattern[page][i] >= .75){
				for(j = 0; j < 15; j++){
					if(pagePattern[i][j] > 0){
						pageSwap[j] = 1;
					}
					if(pagePattern[i][j] >= 0.50){
						for(k = 0; k < 15; k++){
							if(pagePattern[i][k] > 0){
								pageSwap[k] = 1;
							}
						}
					}
				}
			}
		}


		/* Use pageSwap array to determine which pages we can swap in/out */
		for(i = 0; i < 15; i++){
			if(pageSwap[i] == 1){
				pagein(proc, i);

			}
			if(pageSwap[i] == 0){
				pageout(proc,i);
			}
		}
	}

	/* advance time for next pageit iteration */
	tick++;
}
