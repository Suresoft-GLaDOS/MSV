

int glob = 1;
int main(void)
{
	int a = 0, b = 0;
	while (a < 10) {
		a += b;
		b += 1;
		test_assert(glob >= 1);
		glob++;
	}

	return 0;
}
