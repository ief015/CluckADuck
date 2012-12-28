#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

static void showErr(const char* fmt, ...)
{
	char* buf;
	va_list l;

	va_start(l, fmt);
	int len = vprintf(fmt, l);

	buf = new char[len+1];

	vsprintf(buf, fmt, l);
	MessageBox(NULL, buf, "Error", MB_ICONERROR|MB_OK);
	va_end(l);

	delete[] buf;
}

static void randomInit()
{
	srand((unsigned)time(NULL));
}
static void randomInit(unsigned int seed)
{
	srand(seed);
}

// random boolean
static inline bool randomb()
{
	return rand() >= RAND_MAX/2;
}

// random int
static int randomi()
{
	return rand();
}

// random int
static short randomi(int m)
{
	return rand() % m;
}

// random int
static short randomi(int min, int max)
{
	return min + (rand() % (max - min));
}

// random int (upper bound inclusive)
static short randomiinc(int m)
{
	return rand() % (m + 1);
}

// random int (upper bound inclusive)
static short randomiinc(int min, int max)
{
	return min + (rand() % ((max - min) + 1));
}

// random float
static float randomf()
{
	return (float)rand() / RAND_MAX;
}

// random double
static double randomd()
{
	return (double)(rand() | (rand()<<16)) / (0xFFFF | (RAND_MAX<<16));
}

// random float
static inline float randomf(float m)
{
	return randomf() * m;
}

// random double
static inline double randomd(double m)
{
	return randomd() * m;
}

// random float
static inline float randomf(float min, float max)
{
	return randomf() * (max - min) + min;
}

// random double
static inline double randomd(double min, double max)
{
	return randomd() * (max - min) + min;
}

// random float using exponential distribution
static float randomfexpd(float a)
{
	return powf(randomf(), a);
}

// random double using exponential distribution
static double randomdexpd(double a)
{
	return pow(randomd(), a);
}

// random float using exponential distribution
static inline float randomfexpd(float a, float m)
{
	return randomfexpd(a) * m;
}

// random double using exponential distribution
static inline double randomdexpd(double a, double m)
{
	return randomdexpd(a) * m;
}
