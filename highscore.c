#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "highscore.h"

static const char *scorefile = "jurl-hiscore.dat";

short int loadHiScores(SCORE *hiscores) {
	short int success = 1;
	FILE *f;

	if ((f = fopen(scorefile,"rb")) == NULL) {
		success = 0;
	} else {
		if (fread(hiscores,sizeof(SCORE),MAX_HISCORES,f) != MAX_HISCORES) {
			success = 0;
		}

		fclose(f);
	}

	return success;
}

short int saveHiScores(SCORE *hiscores) {
	short int success = 1;
	FILE *f;

	if ((f = fopen(scorefile,"wb")) == NULL) {
		success = 0;
	} else {
		if (fwrite(hiscores,sizeof(SCORE),MAX_HISCORES,f) != MAX_HISCORES) {
			success = 0;
		}

		fclose(f);
	}

	return success;
}

short int createHiScoreTable(void) {
	SCORE board[MAX_HISCORES];

	memset(board,0,sizeof(SCORE) * MAX_HISCORES);

	sprintf(board[0].name,"%s","JCB");
	board[0].score=1000;
	sprintf(board[1].name,"%s","AAR");
	board[1].score=900;
	sprintf(board[2].name,"%s","DWM");
	board[2].score=800;
	sprintf(board[3].name,"%s","BMP");
	board[3].score=700;
	sprintf(board[4].name,"%s","JAY");
	board[4].score=600;
	sprintf(board[5].name,"%s","T J");
	board[5].score=500;
	sprintf(board[6].name,"%s","SIM");
	board[6].score=400;
	sprintf(board[7].name,"%s","S B");
	board[7].score=300;
	sprintf(board[8].name,"%s","P J");
	board[8].score=400;
	sprintf(board[9].name,"%s","DAM");
	board[9].score=300;

	return (saveHiScores(board));
}

short int needScoreFile(void) {
	FILE *f = fopen(scorefile,"rb");
	short int needed = 0;

	if (f == NULL) {
		needed = 1;
	} else {
		fclose(f);
	}

	return needed;
}
