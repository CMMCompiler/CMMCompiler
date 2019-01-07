int a(int num) {
	return num;
}

int main(void) {
	int a;
	a = 1;
	while(a) {
		a = a(a);
	}
	
	return a;
}