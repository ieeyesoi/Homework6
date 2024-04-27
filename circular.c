//circularQ.c
//
//School of Computer Science
//Chungbuk National University
//

#include <stdio.h>  
#include <stdlib.h> // <stdio.h>와 <stdlib.h> 라이브러리를 포함해 컴파일 전 소스 확장

#define MAX_QUEUE_SIZE 4	// define 함수를 통해 원형큐 크기의 최대값을 4로 정의함(MAX_QUEUE_SIZE를 상수로 지정하여 4를 저장)

// 큐 구조체의 타입을 정의함
typedef char element;		// 큐에 저장할 데이터를 char타입으로 정의함, element 구조체 선언
typedef struct {	
	element queue[MAX_QUEUE_SIZE];
	int front, rear;    // front와 rear 선언
}QueueType; // QueueType 구조체 선언

QueueType *createQueue();	// 큐를 생성하는 함수를 선언함
int freeQueue(QueueType *cQ); // 동적으로 할당된 큐의 메모리를 해제하는 함수를 선언함
int isEmpty(QueueType *cQ); // 큐가 비어있음을 확인하는 함수를 선언함
int isFull(QueueType *cQ);	// 큐가 가득 채워져있는지 확인하는 함수를 선언함
void enQueue(QueueType *cQ, element item); // 큐에 데이터를 추가하는 함수를 선언함
void deQueue(QueueType *cQ); // 큐 데이터를 삭제하고 그 값을 반환하는 함수를 선언함
void printQ(QueueType *cQ); // 큐의 모든 데이터 값을 출력하는 함수를 선언함
void debugQ(QueueType *cQ); // 큐 디버그용 함수를 선언함
element getElement(); // 사용자로부터 element 값을 입력받아 반환하는 함수를 선언함

int main(void)
{	// 메인 함수의 시작
    
	QueueType *cQ = createQueue(); // 큐를 생성함(front와 rear만 초기화하였기 때문에 큐에 출력 시 지장이 없는 쓰레기값 저장됨)
	element data; // 사용자로부터 입력받은 element 값을 저장하는 변수
	char command;	// 사용자의 입력 명령을 저장하는 변수

    printf("[----- [leeyesol] [2023070002] -----]");    // 학번과 이름 출력

	do{	// do while 문을 이용해 아래 내용 띄움
		
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");   // 해당 문구 출력
		scanf(" %c", &command);	// 사용자로부터 commad를 입력받음

		switch(command) {	// 입력 받은 command 값으로 각각에 할당한 함수를 실행함
		case 'i': case 'I': //command가 i 또는 I일 때,(command를 입력받을 때 대소문자를 구분하지 않음)
			data = getElement();    // 사용자로부터 element를 입력받음
			enQueue(cQ, data);  //큐에 element를 추가함
			break;
		case 'd': case 'D': //command가 d 또는 D일 때,
			deQueue(cQ); //큐에서 element를 삭제하고 그 값을 data에 저장함
			break;
		case 'p': case 'P': //command가 p 또는 P일 때,
			printQ(cQ); //큐의 모든 데이터를 출력함
			break;
		case 'b': case 'B': //command가 b 또는 B일 때,
			debugQ(cQ); //디버그용 정보들을 출력함
			break;
		case 'q': case 'Q': //command가 q 또는 Q일 때, 
   	  freeQueue(cQ);
			break;
		default:	// 사용자가 command를 잘못 입력했을 때
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");  //해당 문구를 출력
			break;
		}
        // i or I = insert, d or D = delete, p or P = print, b or B = debug, q or Q = quit

	}while(command != 'q' && command != 'Q'); // Q 값을 호출하기 전까지 반복

	return 1; // return을 이용해 1을 반환
}

QueueType *createQueue() // 원형 큐를 생성하는 함수
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 원형 큐 크기만큼의 메모리를 할당함(초기화하지 않아 쓰레기 값 출력됨)
	cQ->front = 0;	// front 값을 0으로 초기화함
	cQ->rear = 0;	// rear 값을 0으로 초기화함
	return cQ;
}

int freeQueue(QueueType *cQ) // 동적으로 할당된 원형 큐의 메모리를 해제하는 함수
{
    if(cQ == NULL)  // cQ가 비어있을 때 1을 반환함
        return 1;
    free(cQ);   //cQ가 비어있지 않을 때 free함
    return 1;
}

// 사용자로부터 하나의 element를 입력받아 반환함
element getElement()	// 요소를 입력받아 큐에 저장하는 함수
{
	element item;
	printf("Input element = "); //헤당 문구를 출력
	scanf(" %c", &item);	// char타입의 문자형에 저장함
	return item;	// 사용자로부터 입력 받은 문자를 반환함
}


//큐가 비어있는지 확인하는 함수
int isEmpty(QueueType *cQ)
{
		if (cQ->front == cQ->rear) { // 큐가 비어있을 때 (front, rear의 값이 같음 = 큐에 요소가 존재하지 X)
			printf("원형 큐가 비어있습니다");   
            return 1; // 큐가 빔
		} else {
			return 0; // 같지 않다면 0을 리턴함
			} 
}

//큐가 가득 채워져있는지 확인하는 함수
int isFull(QueueType *cQ)
{	
	int nextRear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
    if (nextRear == cQ->front) {
        printf("원형 큐가 가득 찼습니다.\n");
        return 1;
    } else {
        return 0; // 0 리턴 (거짓)
    }
}


// 큐에 요소 추가하는 함수
void enQueue(QueueType *cQ, element data)
{
	int nextrear = (cQ->rear + 1) % MAX_QUEUE_SIZE;
	if (isFull(cQ)) { // 큐가 가득 찼을 때 
		printf("큐가 가득 찼습니다.\n");	// 큐가 가득차있으면 경고를 출력함
	} else {

		cQ->rear = nextrear;	// rear를 다음으로 이동시킴, 크기 초과하면 배열의 처음으로 인덱스 변경함

		cQ->queue[cQ->rear] = data; // 현 rear 위치에 새로운 요소 저장
	}
}

//큐에서 요소를 제거하는 함수
void deQueue(QueueType *cQ)
{	
	if(isEmpty(cQ)) //큐가 비어있을 때
    {
        printf("큐가 비어있습니다.");
				return;
    } 
	
	else {  //큐가 비어있지 않을 때 큐 삭제
	cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; 
	//front 다음을 현재의 front로 할당
    return; 
	}
}

//큐의 내용을 출력하는 함수
void printQ(QueueType *cQ)
{
	int i, first, last;	

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //cQ의 first의 현재 인덱스 위치
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	//cQ의 rear의 현재 인덱스 위치

	printf("Circular Queue : [");   // 해당 문구를 출력함

	i = first;	// 반복문에서 변수 i를 first의 값으로 초기화
	while(i != last){ // 변수 i가 last에 도달할 떄까지 계속 돌린다
		printf("%3c", cQ->queue[i]); // 간격을 %3c으로  두어 큐의 맨 처음 값을 출력함
		i = (i + 1)%MAX_QUEUE_SIZE; // i에 1을 더하되, 사이즈보다 크면 맨 처음으로 돌아감
	}
	printf(" ]\n");
}

//큐 내부 상태를 출력하는 함수(디버그 함수)
void debugQ(QueueType *cQ)  //큐에 문제가 있는지 검사하는 함수
{
	printf("\n---DEBUG\n"); //해당 문구를 출력
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // 큐 배열 전체를 0부터 확인함
	{
		if(i == cQ->front) { // 현재 인덱스의 값 = front의 인덱스 값이면,
			printf("  [%d] = front\n", i); // front의 위치를 출력함
			continue; // 출력 후 현재 반복문 건너뛰고 인덱스 값 증가시킴
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // 현재 인덱스의 큐 요소를 출력함
	}

	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //front와 rear의 인덱스 위치를 출력함
}