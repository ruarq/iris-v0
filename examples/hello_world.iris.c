#define true 1
#define false 0
#include <stdio.h>
#define puti(x) printf("%d\n", (x))
#include <stdint.h>
#include <stddef.h>

int32_t fib_rec(int32_t num);
int32_t fib(int32_t num);
void print_fib_seq(int32_t from, int32_t to);
int32_t main();

int32_t fib_rec(int32_t num) {
	if ((num) < (1)) {
		return 0;
	}
	else if ((num) == (1)) {
		return 1;
	}
	return (fib(((num) - (1)))) + (fib(((num) - (2))));
}

int32_t fib(int32_t num) {
	int32_t a = 0;
	int32_t b = 1;
	while ((num) > (0)) {
		int32_t const c = (a) + (b);
		(a) = (b);
		(b) = (c);
		(num) = ((num) - (1));
	}
	return b;
}

void print_fib_seq(int32_t from, int32_t to) {
	while ((from) < (to)) {
		int32_t const r = (fib)((from));
		puti(r);
		(from) = ((from) + (1));
	}
}

int32_t main() {
	void(*fib_prog)(int32_t, int32_t) = print_fib_seq;
	(fib_prog)((10), (15));
	uint8_t const a = 1;
	return 0;
}

