#include<stdio.h>
//NOTICE:char type:-128~127 integer

typedef struct {
//char pointer;//栈指针
char row;//行坐标
char column;//列坐标
char square;//宫坐标
char value;
} NODE;

char WITCH_SQUARE(char i,char j) {//计算宫坐标
	char square;
	if     (i>=0&&i<=2&&j>=0&&j<=2) {square=0;}
	else if(i>=0&&i<=2&&j>=3&&j<=5) {square=1;}
	else if(i>=0&&i<=2&&j>=6&&j<=8) {square=2;}
	else if(i>=3&&i<=5&&j>=0&&j<=2) {square=3;}
	else if(i>=3&&i<=5&&j>=3&&j<=5) {square=4;}
	else if(i>=3&&i<=5&&j>=6&&j<=8) {square=5;}
	else if(i>=6&&i<=8&&j>=0&&j<=2) {square=6;}
	else if(i>=6&&i<=8&&j>=3&&j<=5) {square=7;}
	else if(i>=6&&i<=8&&j>=6&&j<=8) {square=8;}
	else { printf("Error：Coordinate overflow!i=%d j=%d\n",i,j);return 1;}
	return square;
}

void PRINT_TABLE(char C[][9][2]) {//打印九宫格
	int m,n;
	for(m=0;m<9;m++){
		for(n=0;n<9;n++) {
			printf("%d ",C[m][n][0]);
		}
		printf("\n");
	}
	printf("\n");
}

int FIND_NEXT_BLANK(char C[][9][2],char *i,char *j) {//查找未填项
	char m,n;
	for(m=0;m<9;m++){
		for(n=0;n<9;n++) {
			if(C[m][n][0]==0)break;
		}
		if(n==9)n--;//avoid C[8][9][0]==0
		if(C[m][n][0]==0)break;
	}
	if(m>=9){return 1;}
	else {
		*i=m;*j=n;
	}
	return 0;
}

int main(void) {

	NODE STACK[82];//建立遍历栈
	char ROW[9][9];//九行数字标志位，有9个数字，占有写1，未占有写0
	char COLUMN[9][9];//九列数字标志位，有9个数字，占有写1，未占有写0
	char SQUARE[9][9];//九宫数字标志位，有9个数字，占有写1，未占有写0
	char COORDINATE[9][9][2];//[r][c][0]记录九宫格内已存放数字;[r][c][1]记录是否是初始的数字，[r][c][1]=1代表是，[r][c][1]=0代表是后填入的;
	char N=0;//记录九宫格初始存放数字数量
	char N_EMPTY=0;//记录九宫格初始未存放数字数量
	long DONE=0;//记录九宫格的解的数量
	char i,j,k;//临时变量，i表示行，j表示列，k表示宫
	char tmp_input;//接收用户输入
	char POINTER=0;//栈指针
	char v=1;//数独取值1~9
	/*INITIALIZE BEGIN*/
	for(i=0;i<9;i++) {
		for(j=0;j<9;j++) {
			ROW[i][j]=0;
			COLUMN[i][j]=0;
			SQUARE[i][j]=0;
		}
	}
	STACK[0].row=99;
	STACK[0].column=99;
	STACK[0].square=99;
	STACK[0].value=99;
	
	for(i=0;i<9;i++) {
		for(j=0;j<9;j++) {
			do {scanf("%c",&tmp_input);}while(tmp_input==10||tmp_input==32);
			tmp_input=tmp_input-48;
			if(tmp_input==0){
				COORDINATE[i][j][1]=0;
			}
			else if(tmp_input>0&&tmp_input<=9){
				COORDINATE[i][j][1]=1;
				ROW[i][tmp_input]=1;
				COLUMN[j][tmp_input]=1;
				k=WITCH_SQUARE(i,j);
				SQUARE[k][tmp_input]=1;
				N=N+1;//、、、、、、、、、、、、、、、、
			}
			else { printf("Error:invalid number! %d\n",tmp_input);return 1;}
			COORDINATE[i][j][0]=tmp_input;
		}
	}
	/*INITIALIZE END*/
		//PRINT_TABLE(COORDINATE);
	N_EMPTY=81-N;
	if (FIND_NEXT_BLANK(COORDINATE,&i,&j)==1){printf("Error:Full Sudoku table!\n");return 1;}
	k=WITCH_SQUARE(i,j);
	//printf("\n%d numbers need be filled in.\n",N_EMPTY);
	/*TRAVERSAL BEGIN*/
	for(v=1;v<=9;v++){
		if(ROW[i][v]==1||COLUMN[j][v]==1||SQUARE[k][v]==1) {
			if(v<9){continue;}
			else{
				//出栈
				while(v==9){//如果出栈后，上一个数字依然是9则继续出栈。
					i=STACK[POINTER].row;j=STACK[POINTER].column;k=STACK[POINTER].square;
					v=STACK[POINTER].value;
					ROW[i][v]=0;COLUMN[j][v]=0;SQUARE[k][v]=0;//标志位清0
					COORDINATE[i][j][0]=0;
					POINTER--;
						/*
						如果POINTER==0,v=STACK[0].value=99，POINTER-- = -1; 此时v>9必然跳出2个循环。
						*/
					//出栈end
				}
					//v=v+1;//尝试下一个数字
				continue;			
			}
		} else {
			COORDINATE[i][j][0]=v;
			ROW[i][v]=1;COLUMN[j][v]=1;SQUARE[k][v]=1;
				//进栈
			POINTER++;
				//if(DONE==81){PRINT_TABLE(COORDINATE);}//遍历成功,打印九宫格
			STACK[POINTER].row=i;STACK[POINTER].column=j;STACK[POINTER].square=k;
			STACK[POINTER].value=v;
				//进栈end
			if(FIND_NEXT_BLANK(COORDINATE,&i,&j)==1){
				PRINT_TABLE(COORDINATE);
				DONE=DONE+1;
				i=STACK[POINTER].row;j=STACK[POINTER].column;k=STACK[POINTER].square;
				v=STACK[POINTER].value;
				ROW[i][v]=0;COLUMN[j][v]=0;SQUARE[k][v]=0;//标志位清0
				COORDINATE[i][j][0]=0;
				POINTER--;
				continue;
			}
			k=WITCH_SQUARE(i,j);
			v=0;
		}
	}
	/*TRAVERSAL END*/
	if(POINTER==-1){
		if(DONE==0){printf("Error:No solution!\n");return 1;}
		else {printf("Success:Complete search!\nTotal %ld Results\n",DONE);}
	}
	return 0;
	
}
