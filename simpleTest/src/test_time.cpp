#include <stdio.h>
#include <time.h>

int main() {
	time_t now = (time_t)1449400546;
	struct tm t;

	// UTC를 국가 시간으로 변환
    t = *localtime(&now);

	// 1970년 1월 1일 0시를 기준.
	printf("현재 국가 시간 : %d/%02d/%02d %02d:%02d:%02d\n",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
		t.tm_hour, t.tm_min, t.tm_sec);

	t = *gmtime(&now);

	printf("세계 표준 시간 : %d/%02d/%02d %02d:%02d:%02d\n",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
		t.tm_hour, t.tm_min, t.tm_sec);

	return 0;
}
