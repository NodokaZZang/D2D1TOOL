# Map Tool 개발 방향

## Menu 구성

+ Map : .map 확장자 BinaryFile 실제로 게임에서 MapData로 사용되어짐 

+ Object : .obj 확장자 BinaryFile 실제로 게임에서 사용되는 Object 클래스를 저장하는 파일

+ Script : 프로그램이 로딩될때 기본적으로 하드코딩되어 들어가는 메뉴 EX. WALL, EMPTY, ENEMY, ...

## DataSet 

.map 파일 저장할떄 Data Set 
```C++
int xSize;
int ySize;
int mapdata [xSize][ySize];
```


.obj 파일 저장할때 Data Set
```C++
char imgFileName [256];
int  xPos;
int  yPos;
int  width;
int  height;
int  objectCode;
```

## 맵툴 비즈니스 로직
### New Map 
1. 다이얼로그 생성 xSize, ySize 입력받음

2. CreateGrid 

3. ImgFileLoad

4. Object 배치, Event 배치, Mapdata 처리 

5. MapData 저장

6. 트리뷰 화면에 갱신시켜줌

###  Object 배치, Event 배치, Mapdata 처리 
1. Object 배치시에 트리뷰에 원하는 Object 클릭, Event도 이와 같음

2. 해당 Object를 선택한 상태에서 MainWnd 화면에 올라가면 Obj가 마우스 커서를 따라다님

3. 마우스 좌 클릭시 해당 오브젝트 Code가 mapData에 반영되고 더이상 마우스 커서를 따라다니지않음 mapGrid에 놓여짐
