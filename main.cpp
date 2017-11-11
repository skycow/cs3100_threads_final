#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cmath>

/* uncomment the following line to use 'long long' integers */
/* #define HAS_LONG_LONG */
//#ifdef HAS_LONG_LONG
#define mul_mod(a,b,m) (( (long long) (a) * (long long) (b) ) % (m))
//#else
//    #define mul_mod(a,b,m) std::fmod( (double) a * (double) b, m)
//#endif
/* return the inverse of x mod y */
int inv_mod(int x, int y){int q, u, v, a, c, t;u = x;v = y;c = 1;a = 0;do {q = v / u;
t = c; c = a - q * c; a = t; t = u; u = v - q * u; v = t; } while (u != 0); a = a % y; if (a < 0)a = y + a; return a; }/* return (a^b) mod m */int pow_mod(int a, int b, int m) { int r, aa; r = 1; aa = a; while (1) { if (b & 1)r = mul_mod(r, aa, m); b = b >> 1; if (b == 0)break; aa = mul_mod(aa, aa, m); }return r; }/* return true if n is prime */int is_prime(int n) { int r, i; if ((n % 2) == 0)return 0; r = (int)(sqrt(n)); for (i = 3; i <= r; i += 2)if ((n % i) == 0)return 0; return 1; }/* return the prime number immediatly after n */int next_prime(int n) { do { n++; } while (!is_prime(n)); return n; }
unsigned int computePiDigit(int n) {
	int av, a, vmax, N, num, den, k, kq, kq2, t, v, s, i; double sum = 0; N = (int)((n + 20) * std::log(10) / std::log(2)); for (a = 3; a <= (2 * N); a = next_prime(a)) {
		vmax = (int)(std::log(2 * N) / std::log(a)); av = 1; for (i = 0; i < vmax; i++)av = av * a; s = 0; num = 1; den = 1; v = 0; kq = 1; kq2 = 1; for (k = 1; k <= N; k++) {
			t = k; if (kq >= a) { do { t = t / a; v--; } while ((t % a) == 0); kq = 0; }kq++; num = mul_mod(num, t, av); t = (2 * k - 1); if (kq2 >= a) { if (kq2 == a) { do { t = t / a; v++; } while ((t % a) == 0); }kq2 -= a; }den = mul_mod(den, t, av); kq2 += 2; if (v > 0) {
				t = inv_mod(den, av); t = mul_mod(t, num, av); t = mul_mod(t, k, av); for (i = v; i < vmax; i++)t = mul_mod(t, a, av);
				s += t; if (s >= av)s -= av;
			}
		}t = pow_mod(10, n - 1, av); s = mul_mod(s, t, av); sum = std::fmod(sum + (double)s / (double)av, 1.0);
	}return static_cast<unsigned int>(sum * 1e9 / 100000000);
}

class SafeQueue
{
private:
	std::mutex mutex;
	std::queue<int> theQueue;

public:

	SafeQueue() {}

	SafeQueue(int num_digits)
	{
		for (int i = 0; i < num_digits; i++)
		{
			theQueue.push(i+1
			);
		}
	}

	bool getNext(int &val)
	{
		bool done;
		{
			std::lock_guard<std::mutex> lock(mutex);

			if (theQueue.empty())
			{
				done = false;
			}
			else
			{
				val = theQueue.front();
				theQueue.pop();
				done = true;
			}
		}
		return done;
	}
};

class SafeMap
{
private:
	std::mutex mutex2;
	std::unordered_map<int, int> theMap;

public:
	SafeMap() {}

	void safeInsert(int key, int val)
	{
		std::lock_guard<std::mutex> lock(mutex2);
		theMap.insert(std::make_pair(int(key), int(val)));
	}

	std::unordered_map<int, int> getMap()
	{
		return theMap;
	}
};


void calcPi(SafeQueue &inQueue, SafeMap &resMap)
{
	int nextVal;
	while (inQueue.getNext(nextVal))
	{
		//compute pi
		int calc = computePiDigit(nextVal);
		resMap.safeInsert(nextVal, calc);
	}
	return;
}

int main()
{
	SafeQueue digitQueue(1000);
	SafeMap digitMap;

	int num_ts = std::thread::hardware_concurrency();

	std::vector<std::thread> threads(num_ts);

	for (int i = 0; i<num_ts; ++i)
		threads[i] = std::thread(calcPi, std::ref(digitQueue), std::ref(digitMap));

	for (auto& th : threads) th.join();

	for (int i = 1; i <= 1000; i++)
	{
		std::cout << digitMap.getMap().find(i)->second;
	}

	return 0;
}

