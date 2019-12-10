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

#define DEBUG 0
#if DEBUG
#define DBG(fmt,...)  fprintf(stderr, fmt, ##__VA_ARGS__);
#else
#define DBG(fmt,...)  {;}
#endif

void pageit(Pentry q[MAXPROCESSES]) {

	/* This file contains the stub for a predictive pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time
	static int lastpc[MAXPROCESSES];
	static int program_type[MAXPROCESSES];

	/* Local vars */
	int proc;
	int pc;
	int page;

	double pagePattern[15][15] = 
	{    
  // 0     1   2   3   4   5     6   7   8   9    10  11  12   13  14
		{0.0  ,1.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //0
		{0.0  ,0.0,1.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //1
		{0.0  ,0.0,0.0,1.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //2
		{0.25 ,0.0,0.0,0.0,0.50,0.0 ,0.0,0.0,0.0,0.0 ,0.25,0.0,0.0 ,0.0,0.0  }, //3
		{0.0  ,0.0,0.0,0.0,0.0 ,0.75,0.0,0.0,0.0,0.0 ,0.25,0.0,0.0 ,0.0,0.0  }, //4
		{0.0  ,0.0,0.0,0.0,0.0 ,0.0 ,1.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //5
		{0.0  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,1.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //6
		{0.0  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,1.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //7
		{0.5  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.5 ,0.0 ,0.0,0.0 ,0.0,0.0  }, //8
		{0.0  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,1.0 ,0.0,0.0 ,0.0,0.0  }, //9
		{0.5  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.5,0.0 ,0.0,0.0  }, //10
		{0.25 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.75,0.0,0.0  }, //11
		{0.0  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,1.0,0.0  }, //12
		{0.125,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.75,0.0 ,0.0,0.0 ,0.0,0.125}, //13
		{1.0  ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0,0.0,0.0 ,0.0 ,0.0,0.0 ,0.0,0.0  }  //14
	};


	/* initialize static vars on first run */
	if(!initialized){
		/* Init complex static vars here */
		initialized = 1;
	}

	for(proc=0;proc<MAXPROCESSES;proc++){

		if(!q[proc].active){
			continue;
		}

		int i,j;

		/* Track the pages we should swap in/out, 1 = swap in, 0 = swap out */
		int pageSwap[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		/* program counter */
		pc = q[proc].pc;
		/* current page */
		page = pc/PAGESIZE;

		int pc_diff = lastpc[proc] - pc;

		if (pc_diff > 1 && program_type[proc] == 0) {
			switch (pc_diff) {
			case 1533:
			//case -902:
			//case -132:
				program_type[proc] = 1;
			  for (i=12; i<=14; i++) pageout(proc, i);
				DBG(">>> Proc=%d PROG=%d\n", proc, program_type[proc]);

				break;
			case 1129:
				program_type[proc] = 2;
			  for (i=9; i<=14; i++) pageout(proc, i);
				DBG(">>> Proc=%d PROG=%d\n", proc, program_type[proc]);
				break;
			case 516:
			case 1683:
				program_type[proc] = 3;
			  pageout(proc, 14);
				DBG(">>> Proc=%d PROG=%d\n", proc, program_type[proc]);
				break;
			case 501:
			case 503:
				program_type[proc] = 5;
			  for (i=4; i<=14; i++) pageout(proc, i);
				DBG(">>> Proc=%d PROG=%d\n", proc, program_type[proc]);
				break;
			}
		}

		// detect exit
/*
		switch (program_type[proc]) {
			case 0:
				if (pc != 1911) break;
			case 1:
				if (pc != 1534) break;
			case 2:
				if (pc != 1130) break;
			case 3:
				if (pc != 1684) break;
			case 5:
				if (pc != 504)  break;

			default:
				break;
*/
		if (  (program_type[proc]==0 &&  pc ==1911)
				||(program_type[proc]==1 &&  pc ==1534)
				||(program_type[proc]==2 &&  pc ==1130)
				||(program_type[proc]==3 &&  pc ==1684)
				||(program_type[proc]==5 &&  pc ==504)
				) {
				DBG(">>> EXIT proc=%d type=%d pc=%d\n", proc, program_type[proc], pc);
				program_type[proc] = 0;
				lastpc[proc]       = -1;
		}

		pagein(proc, page);

  	if (program_type[proc] == 0) {
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
					}
				}
			}

			/* See pageSwap array above for description */
			for(i = 0; i < 15; i++){
				if(pageSwap[i] == 1){
					pagein(proc, i);
				}
				if(pageSwap[i] == 0){
					pageout(proc,i);
				}
			}
		} else if (program_type[proc] == 1) {
			switch (page) {
			case 0:
				pagein(proc, 1);
				pageout(proc, 11);
				break;
			case 3:
				pagein(proc, 4);
				pagein(proc, 10);
				pagein(proc, 11);
				pageout(proc, 2);
				break;
			case 4:
				pagein(proc, 5);
				pageout(proc, 3);
				pageout(proc, 10);
				break;
			case 10:
				pagein(proc, 11);
				pagein(proc, 0);
				pageout(proc, 9);
				pageout(proc, 3);
				pageout(proc, 4);
				break;
			case 11:
				pagein(proc, 0);
				pageout(proc, 10);
			default:
				pagein(proc, page+1);
				pageout(proc, page-1);
				break;
			}
			
		} else if (program_type[proc] == 2) {
			switch (page) {
			case 0:
				pagein(proc, 1);
				pageout(proc, 8);
				break;
			case 8:
				pagein(proc, 0);
				pageout(proc, 7);
				break;
			default:
				pagein(proc, page+1);
				pageout(proc, page-1);
				break;
			}
		} else if (program_type[proc] == 3) {
			switch (page) {
			case 0:
				pagein(proc, 1);
				pageout(proc, 9);
				pageout(proc, 13);
				break;
			case 9:
				pagein(proc, 10);
				pageout(proc, 0);
				pageout(proc, 13);
				break;
			case 12:
				pagein(proc, 0);
				pagein(proc, 9);
				pagein(proc, 13);
				pageout(proc, 11);
				break;
			case 13:
				pagein(proc, 0);
				pagein(proc, 9);
				pageout(proc, 12);
				break;
			default:
				pagein(proc, page+1);
				pageout(proc, page-1);
				break;
			}

		} else if (program_type[proc] == 5) {
			switch (page) {
			case 0:
				pagein(proc, 1);
				pageout(proc, 3);
			  pageout(proc, 4);
				break;
			case 3:
				pagein(proc, 0);
				pageout(proc, 2);
				break;
			default:
				pagein(proc, page+1);
				pageout(proc, page-1);
				break;
			}
    }
		lastpc[proc] = pc;
	}

	/* increment clock */
	tick++;
}
