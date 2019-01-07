int a(int num) {
	return num;
}

int main(void) {
	int a;
	a = 1;
	a = a(a);
	return a;
}