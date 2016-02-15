#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ArraySize 100

void GetDbName(char *DBModelName);

int main()
{
/*
    int arrayA[ArraySize];
    int originalArraySize = ArraySize;
    
    for(int i = 0; i < originalArraySize; i++) {
        arrayA[i] = i;
    }
        
    int startIndex = 0;
    int readSize = 60;
    
    printf("======= 원래 배열 읽기 =======\n");
    for(int i = startIndex; i < readSize; i++) {
        printf("Array[%2d] = %2d ", i, arrayA[i]);
        if(((i + 1) % 5) == 0) printf("\n");
    }
    printf("\n\n");
    
    int remainSize = originalArraySize - readSize;
    int *copyArray = (int *)malloc(sizeof(int) * remainSize);
    memcpy(copyArray, &arrayA[readSize], sizeof(int) * remainSize); // 남은 부분 복사   
    
    memset(arrayA, 0, originalArraySize);   // 기존 배열 해제
    
    printf("======= 읽고 남은 배열 =======\n");
    for(int i = 0; i < remainSize; i++) {   // 복사 확인
        printf("Array[%2d] = %2d ",i , copyArray[i]);
        if(((i + 1) % 5) == 0) printf("\n");
    }
    
    free(copyArray);
*/    
	
	char temp[32];
	memset(temp, 0x00, sizeof(temp));
	GetDbName(temp);
	
	printf("DbName(%s), strlen(%lu)\n", temp, strlen(temp));
	
    return 0;
}

void GetDbName(char *DBModelName)
{
	char DbName[] = "10CNT_NA1160";
	char Len[2];
	memset(Len, 0x00, sizeof(Len));
	memcpy(Len, DbName, sizeof(Len));
	
	int Length = atoi(Len);		
	memcpy(DBModelName, DbName + 2, Length);
	
	return;
}

// int GetDbNameLength()
// {
	
// }
