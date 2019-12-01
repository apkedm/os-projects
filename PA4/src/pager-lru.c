/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

	/* This file contains the stub for an LRU pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time
	static int timestamps[MAXPROCESSES][MAXPROCPAGES];

	/* Local vars */
	int proctmp;
	int pagetmp;

	/* initialize static vars on first run */
	if(!initialized){
		for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
			for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
				timestamps[proctmp][pagetmp] = 0;
			}
		}
		initialized = 1;
	}

	/* TODO: Implement LRU Paging */

	/* Local vars */
	int proc;
	int pc;
	int page;
	int cur_page;
	int lru_page;

	/* Loop through each process */
	for(proc=0; proc<MAXPROCESSES; proc++) {
		/* Check if process is active */
		if(q[proc].active) {
			pc = q[proc].pc; // program counter for process
			page = pc/PAGESIZE;	// page the program counter needs
			/* Check if page is not swapped in */
			if(!q[proc].pages[page]) { // pages[i] is 1 if swapped in
				/* If so try to swap it in */
				if(!pagein(proc, page)) { //1 if pagein started, 0 if it can't start

					/* If you cannot swapon in a page, choose the lru to evict */
					int lru_time = tick; // placeholder
					for(cur_page=0; cur_page < q[proc].npages; cur_page++) {
						/* If the page is swapped in and it is older than the curent lru page*/
						if(q[proc].pages[cur_page] && timestamps[proc][cur_page] < lru_time) {
							/* Set the new lru time and the new lru page */
							lru_time = timestamps[proc][cur_page];
							lru_page = cur_page;
						}
					}
					pageout(proc, lru_page);
				}
			}
			timestamps[proc][page] = tick;
		}
	}

	/* advance time for next pageit iteration */
	tick++;
}
