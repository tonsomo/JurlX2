/*
 *                               Alizarin Tetris
 * For the management of high scores.
 *
 * Copyright 2000, Kiri Wagstaff & Westley Weimer
 */

#ifndef __HIGHSCORE_H
#define __HIGHSCORE_H

#define MAX_HISCORES 10
typedef struct {
	char name[3];
	// player's name
	int score;
	// their score
} SCORE; 

short int loadHiScores(SCORE *hiscores);
short int saveHiScores(SCORE *hiscores);
short int createHiScoreTable(void);

#endif
