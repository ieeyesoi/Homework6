//postfix.c
//
//School of Computer Science
//Chungbuk National University
//

// 컴파일 전 소스를 확장하도록 지시하기 위해 네 가지 라이브러리를 포함
#include<stdio.h>   
#include<stdlib.h>  
#include<string.h> 
#include<ctype.h> // isdigit의 사용을 위함

#define MAX_STACK_SIZE 10 // /MAX_STACK_SIZE(stack의 최대 사이즈)를 10으로 초기화하고 상수로 지정함
#define MAX_EXPRESSION_SIZE 20 // MAX_EXPRESSION_SIZE(수식의 최대 사이즈)를 20으로 초기화하고 상수로 지정함

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;   //구조체 precedence로 우선 순위를 정함


// 수식을 저장하고 stack을 조작하기 위해 전역변수를 선언하는 과정
char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

// 함수들의 원형을 선언함
void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
//precedence getToken(char symbol);
precedence getPriority(char symbol);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main() //메인 함수의 시작
{
    int count = 0;  //Infix 전 Postfix나 Eval을 할 수 없도록 count를 선언함
	char command; // 사용자로부터 입력받을 command를 선언함

    printf("[----- [leeyesol] [2023070002] -----]");    //이름과 학번 출력

    //command가 q나 Q일 때까지 반복을 실행하는 반복문
	do{ //메뉴를 출력함
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //사용자로부터 command를 입력받음

		switch(command) { // 사용자로부터 입력받은 command 값에 따라 이용할 함수를 결정함
		case 'i': case 'I': //command가 i 또는 I일 때
			getInfix();
			break;
		case 'p': case 'P': //commandrk p 또는 P일 때
			toPostfix();
			break;
		case 'e': case 'E': //command가 e 또는 E일 때
			evaluation();
			break;
		case 'd': case 'D': //command가 d 또는 D일 때
			debug();
			break;
		case 'r': case 'R': //command가 r 또는 R일 때
			reset();
			break;
		case 'q': case 'Q': //command가 q 또는 Q일 때
			break;
		default:    //command를 잘못 입력했을 때
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 해당 경고 문구 출력
			break;
		}

	}while(command != 'q' && command != 'Q'); // q나 Q를 입력하기 전까지 위를 반복함
	
	return 1;
}

void postfixPush(char x)
{   //postfixstack의 Top에서 삽입하는 함수
    postfixStack[++postfixStackTop] = x; // 인덱스 = postfix stack top + 1, 해당 인덱스를 가진 stack에 x의 값을 집어넣음 
}

char postfixPop()
{   //postfixstack의 Top에서 삭제하는 함수
    char x;
    if(postfixStackTop == -1) // stack이 비어있을 때,
        return '\0';
    else {  //stack이 차있을 때, 
    	x = postfixStack[postfixStackTop--]; // 현재 stack의 top 인덱스 값을 x에 삽입하고 Top에서 삭제 후 Top--
    }
    return x; // 위 코드에 결과를 리턴
}

// 계산 값을 삽입하는 함수
void evalPush(int x)
{
    evalStack[++evalStackTop] = x; // ++Top 후 입력 받은 연산 값을 Top에서 삽입함
}
// 계산 값을 삭제하는 함수
int evalPop()
{
    if(evalStackTop == -1)	// stack이 비어있을 때,
        return -1;  //-1을 반환함
    else    //stack이 차있을 때, 
        return evalStack[evalStackTop--]; // Top에서 삭제하고 값을 반환 후 Top--
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{   //사용자로부터 infix를 입력받는 함수
    printf("Type the expression >>> "); //해당 문구를 출력함
    scanf("%s",infixExp);   //infixExp에 문자열 형식으로 저장받음
}
// 우선 순위를 입력 받은 연산자에 맞게 반환해주는 함수 
precedence getPriority(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}
/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{   // 문자를 postfixExp에 옮겨주는 함수
	if (postfixExp == '\0')	// postfixExp가 null값이 아닐 때
		strncpy(postfixExp, c, 1); // c를 postfixExp에 복사함
	else    //postfixExp가 널값일 때
		strncat(postfixExp, c, 1); // 널값을 postfixExp에 추가함
}
/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix() {
    char *exp = infixExp; // infix 표현식 포인터
    char output[MAX_EXPRESSION_SIZE] = {0}; // 출력 문자열
    int outPos = 0; // 출력 문자열의 위치
    char stack[MAX_STACK_SIZE]; // 스택
    int top = -1; // 스택의 최상위 인덱스

    // 인픽스 표현식 순회
    while (*exp) {
        if (isdigit(*exp)) { // 숫자인 경우
            output[outPos++] = *exp; // 결과 문자열에 추가
        } else if (*exp == '(') { // '(' 인 경우
            stack[++top] = *exp; // 스택에 푸시
        } else if (*exp == ')') { // ')' 인 경우
            while (top != -1 && stack[top] != '(') {
                output[outPos++] = stack[top--]; // 스택에서 팝하여 결과 문자열에 추가
            }
            top--; // '(' 제거
        } else { // 연산자인 경우
            while (top != -1 && getPriority(stack[top]) >= getPriority(*exp)) {
                output[outPos++] = stack[top--]; // 연산자 팝
            }
            stack[++top] = *exp; // 현재 연산자 푸시
        }
        exp++; // 다음 문자로 이동
    }

    // 스택에 남은 모든 연산자를 출력 문자열에 추가
    while (top != -1) {
        output[outPos++] = stack[top--];
    }

    strcpy(postfixExp, output); // 완성된 포스트픽스 표현식을 저장
}
// 변수 및 스택의 현재 상태를 출력하고 디버그를 실행하는 함수(infixExp,postfixExp,eavl result,postfixStack 출력)
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}
// infixExp,postfixExp,eavl result,postfixStack(변수들)을 전부 0으로 초기화하고 스택을 처음상태로 리셋시키는 함수
void reset()
{
	infixExp[0] = '\0';	// 널값을 통해 리셋함
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}
// postfix의 산술식 계산을 하는 함수
void evaluation() {
    int numStack[MAX_STACK_SIZE]; // 숫자를 저장할 스택
    int numTop = -1; // 스택의 최상위 인덱스
    char *token = postfixExp; // 탐색 포인터

    // postfix 표현식을 순회하면서 계산
    while (*token) {
        if (isdigit(*token)) { // 숫자인 경우
            numStack[++numTop] = *token - '0'; // 스택에 숫자를 푸시
        } else { // 연산자인 경우
            int op2 = numStack[numTop--]; // 두 번째 피연산자 팝
            int op1 = numStack[numTop--]; // 첫 번째 피연산자 팝
            switch (*token) { // 연산 수행
                case '+': numStack[++numTop] = op1 + op2; break;
                case '-': numStack[++numTop] = op1 - op2; break;
                case '*': numStack[++numTop] = op1 * op2; break;
                case '/':
                    if (op2 == 0) {
                        printf("Division by zero error.\n");
                        return;
                    }
                    numStack[++numTop] = op1 / op2;
                    break;
            }
        }
        token++; // 다음 토큰으로 이동
    }
    evalResult = numStack[numTop--]; // 최종 결과를 추출
}