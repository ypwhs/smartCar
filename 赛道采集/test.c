#include <stdio.h>
#define WIDTH 160
#define HEIGHT 119
int IMG[WIDTH][HEIGHT]={0};
int white[HEIGHT];
bool crossflag = false;
bool rectflag = false;
void findType();

#define DELTA_MAX 5
int main()
{
	FILE *p = fopen("/Users/ypw/Desktop/智能车2/smartCar/赛道采集/直角弯.txt", "r");
	
	char tmp;
	int x,y;
	for (y = 0; y < HEIGHT; ++y){
		for (x = 0; x < WIDTH; ++x){
			fscanf(p, "%c", &tmp);
			while(tmp!='1'&&tmp!='0')
				fscanf(p, "%c", &tmp);
			if(tmp=='1')IMG[x][y]=1;
			else IMG[x][y]=0;
		}
	}

	findType();

	printf("\n--------\n");
	
	for (y = 0; y < HEIGHT; ++y)
	{
		for (x = 0; x < WIDTH; ++x)
		{
			if(IMG[x][y]==2)printf("😭");
			else printf("%d", IMG[x][y]);
		}
		printf("\n");
	}

    printf("crossflag:%d,rectflag:%d\n", crossflag, rectflag);

}

void fixLine(int black, int y, bool isblack){
    //宽度大于5的黑条或白条
    if(white[y]>WIDTH-5)crossflag=true;else rectflag=false;

    int crossstart = y+black+15;
    int crossend = y-10;
    if(crossend<0)crossend=0;
    if(crossstart>HEIGHT)crossstart=WIDTH-1;
    if(white[crossstart]>10&&white[crossend]>10){

        //起始位置从中心向两边搜索边界
        int left1 = WIDTH/2;
        int right1 = WIDTH/2;
        while(left1){
            if(IMG[left1][crossstart] == 1 && (IMG[left1+1][crossstart] == 0))break;
            left1--;
        }
        while(right1 < WIDTH-1){
            if(IMG[right1][crossstart] == 0 && (IMG[right1+1][crossstart] == 1))break;
            right1++;
        }
        //printf("left1=%d,right1=%d\n", left1, right1);

        //终止位置从中心向两边搜索边界
        int left2;
        int right2;
        float k1 = 0;
        float k2 = 0;
        
        left2 = WIDTH/2;
        right2 = WIDTH/2;
        while(left2){
            if(IMG[left2][crossend] == 1 && (IMG[left2+1][crossend] == 0))break;
            left2--;
        }
        while(right2 < WIDTH-1){
            if(IMG[right2][crossend] == 0 && (IMG[right2+1][crossend] == 1))break;
            right2++;
        }
        IMG[left2][crossend] = 2;
        IMG[right2][crossend] = 2;

        k1 = (float)(left2-left1)/(crossend-crossstart);
        k2 = (float)(right2-right1)/(crossend-crossstart);
        //f(k1<0 && k2>0)break;

        //两边补线
        for(int i=0;i<(crossstart-crossend) && crossend+i < HEIGHT-1;i++){
            IMG[(int)(left2+k1*i)][crossend+i] = isblack?2:0;
            IMG[(int)(right2+k2*i)][crossend+i] = isblack?2:0;
        }
    }
}

void findType(){
    int x,y;
    for(y=0;y<HEIGHT;y++){
        int whitedots=0;
        for(x=0;x<WIDTH;x++){
            if(IMG[x][y]==0)whitedots++;
        }
        white[y] = whitedots;
    }
    //统计白点个数

    int black=0;bool lastblack=false;
    int whitesum=0;bool lastwhite=false;
    
    for(y=HEIGHT;y>0;y--){

        if(white[y]<3){
            //一条黑条
            if(lastblack)black++;   //上一次也是黑,则黑++
            else black=1;   //第一次黑
            lastblack=true;
        }else{
            lastblack=false;
            if(black>5){    //黑大于5次
                rectflag=true;  //直角标志
                fixLine(black, y, false);
            }else{
                rectflag = false;
            }
            black=0;
        }

        if(white[y]>WIDTH-3){
            //一条白
            if(lastwhite)whitesum++;   //上一次也是黑,则黑++
            else whitesum=1;   //第一次黑
            lastwhite=true;
        }else{
            lastwhite=true;
            if(whitesum>5){
                crossflag=true;
                fixLine(whitesum, y, true);
            }else{
                crossflag = false;
            }
            whitesum=0;
        }

    }

}