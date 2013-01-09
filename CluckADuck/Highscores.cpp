#include "Highscores.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>


Highscores::Highscores()
{
	loaded = false;
}


Highscores::~Highscores()
{
	this->clearScores();
}


void Highscores::clearScores()
{
	for (size_t i = 0, sz = scores.size(); i < sz; ++i)
		delete scores[i];
	scores.clear();
}


/**
	
	HIGHSCORE FILE FORMAT

	-- Header --

	[8 BYTES]			Signature: 'q', 'U', 'a', 'c', 'K', 0, 1, 2


	-- Body --

	For every score entry [15+ BYTES]
	{
		[1 BYTE - byte]		Expected name string length.
		[variable]			Name string. (Null-terminator is excluded)
		[2 BYTES - short]	Gamemode ID.
		[8 BYTES - double]	Elapsed game time in seconds.
		[4 BYTES - ulong]	Score.
	}

*/


Highscores::ERROR_RES Highscores::load(const char* file)
{
	// Set highscore filename.
	this->filename = file;

	// Open highscore file.
	FILE* f = fopen(filename.c_str(), "r");
	if (!f)
	{
		// Highscore file probably doesn't exist, so let's create a new one.
		ERROR_RES res = this->createNewSaveFile();
		if (res)
			return res;

		// Retry.
		f = fopen(filename.c_str(), "r");
		if (!f)
			return ERR_CANNOTREAD; // Something went terribly wrong.
	}

	// Get size of file.
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	fseek(f, 0, SEEK_SET);

	// Load file into memory.
	char* buf = (char*)malloc(len);
	if (!buf)
	{
		fclose(f);
		return ERR_LOWMEM;
	}
	fread(buf, 1, len, f);

	// Done reading from file.
	fclose(f);

	// Read highscore data.
	ERROR_RES res = this->load(buf, len);

	// Free resources.
	free(buf);

	return res;
}


Highscores::ERROR_RES Highscores::load(const char* data, size_t len)
{
	unsigned long cursor = 0;

	// Check if invalid file. (8 is header signature size)
	if (len < 8)
		return ERR_INVALIDFILE;

	// Check header signature.
	if (data[cursor++] != 'q') return ERR_INVALIDFILE;
	if (data[cursor++] != 'U') return ERR_INVALIDFILE;
	if (data[cursor++] != 'a') return ERR_INVALIDFILE;
	if (data[cursor++] != 'c') return ERR_INVALIDFILE;
	if (data[cursor++] != 'K') return ERR_INVALIDFILE;
	if (data[cursor++] != 0) return ERR_INVALIDFILE;
	if (data[cursor++] != 1) return ERR_INVALIDFILE;
	if (data[cursor++] != 2) return ERR_INVALIDFILE;

	// Clear highscore data.
	this->clearScores();

	// Read score data.
	while (cursor < len-1)
	{
		Score* score = new Score();

		// Check if entry is valid size.
		if (cursor - len < 15)
			return ERR_CORRUPTED;

		// Get name length.
		char name_len = data[cursor++];

		// Get name string.
		char* name = (char*)malloc(static_cast<size_t>(name_len)+1);
		if (!name)
			return ERR_LOWMEM;
		memcpy(name, &data[cursor], name_len);
		name[name_len] = 0;
		score->name = name;
		cursor += name_len;

		// Get gamemode ID.
		score->gamemode = *(short*)&data[cursor];
		cursor += 2;

		// Get elapsed time.
		score->time = *(double*)&data[cursor];
		cursor += 8;

		// Get score.
		score->score = *(unsigned long*)&data[cursor];
		cursor += 4;

		// Add score to scoreboard.
		this->scores.push_back(score);

		// Clean resources.
		delete name;
	}

	// All's good.
	loaded = true;
	return ERR_OK;
}

Highscores::ERROR_RES Highscores::createNewSaveFile()
{
	// Create and write to a new file.
	FILE* f = fopen(filename.c_str(), "w");
	if (!f)
		return ERR_CANNOTCREATE;

	// Write signature.
	char buf[8] = { 'q', 'U', 'a', 'c' , 'K', 0, 1, 2 };
	fwrite(buf, 1, 8, f);
	
	// We're done.
	fclose(f);
	return ERR_OK;
}

Highscores::ERROR_RES Highscores::saveScore(const Score* score)
{
	FILE* f = NULL;

	// A highscore file needs to be loaded.
	if (!loaded)
		return ERR_NOTLOADED;

	// Open highscore file for reading if it's valid.
	f = fopen(filename.c_str(), "r");
	
	if (!f)
	{
		// Highscore file doesn't exist, so let's create a new one.
		ERROR_RES res = this->createNewSaveFile();
		if (res)
			return res;
	}
	else
	{
		// Highscore file exists.

		// Check if valid size.
		size_t len;
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		if (len < 8)
			return ERR_INVALIDFILE;

		// Check for signature.
		char buf[8];
		fread(buf, 1, 8, f);

		if (buf[0] != 'q') return ERR_INVALIDFILE;
		if (buf[1] != 'U') return ERR_INVALIDFILE;
		if (buf[2] != 'a') return ERR_INVALIDFILE;
		if (buf[3] != 'c') return ERR_INVALIDFILE;
		if (buf[4] != 'K') return ERR_INVALIDFILE;
		if (buf[5] != 0) return ERR_INVALIDFILE;
		if (buf[6] != 1) return ERR_INVALIDFILE;
		if (buf[7] != 2) return ERR_INVALIDFILE;

		// We're done.
		fclose(f);
	}

	// Open highscore file for appending score.
	f = fopen(filename.c_str(), "a");

	// Write name string length.
	char name_len = score->name.length();
	fwrite(&name_len, 1, 1, f);

	// Write name string.
	fwrite(score->name.c_str(), 1, name_len, f);

	// Write gamemode ID.
	fwrite(&score->gamemode, 2, 1, f);

	// Write elapsed game time.
	fwrite(&score->time, 8, 1, f);

	// Write score.
	fwrite(&score->score, 4, 1, f);

	// Close file, we're done here.
	fclose(f);
	return ERR_OK;
}


Highscores::Score* Highscores::addScore(const char* name, short gamemode, double time, unsigned long score)
{
	// Create new score.
	Score* s = new Score();
	s->name = name;
	s->gamemode = gamemode;
	s->time = time;
	s->score = score;

	// Add it to the scoreboard.
	scores.push_back(s);

	// Return new score object.
	return s;
}


const Highscores::Score& Highscores::getScore(size_t idx)
{
	return *scores[idx];
}


size_t Highscores::getScoreCount() const
{
	return scores.size();
}


void Highscores::sortScoreboard(SORT sort)
{
	switch (sort)
	{

	// Sort by score.
	case SORT_SCORE:
		std::sort(scores.begin(), scores.end(), &Highscores::compareByScore);
		break;

	// Sort by time.
	case SORT_TIME:
		std::sort(scores.begin(), scores.end(), &Highscores::compareByTime);
		break;

	}
}
