#pragma once

#include <string>
#include <vector>

class Highscores
{
public:
	Highscores();
	virtual ~Highscores();

	struct Score
	{
		std::string name;
		short gamemode;
		double time;
		unsigned long score;
	};

	enum ERROR_RES
	{
		ERR_OK = 0,
		ERR_INVALIDFILE,
		ERR_CORRUPTED,
		ERR_LOWMEM,
		ERR_CANNOTCREATE,
		ERR_CANNOTREAD,
		ERR_NOTLOADED
	};

	enum SORT
	{
		SORT_SCORE,
		SORT_TIME
	};

	ERROR_RES load(const char* file);
	ERROR_RES load(const char* data, size_t len);

	ERROR_RES createNewSaveFile();
	ERROR_RES saveScore(const Score* score);

	Score* addScore(const char* name, short gamemode, double time, unsigned long score);
	void clearScores();
	const Score& getScore(size_t idx);
	size_t getScoreCount() const;
	void sortScoreboard(SORT sort = SORT_SCORE);

private:
	std::vector<const Score*> scores;
	std::string filename;
	bool loaded;

	static bool compareByScore(const Score* left, const Score* right) { return left->score > right->score; }
	static bool compareByTime(const Score* left, const Score* right) { return left->time < right->time; }
};
