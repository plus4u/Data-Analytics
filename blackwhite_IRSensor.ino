#include <Servo.h>
Servo myservo;
Servo myservo2;

int servo1 = 5;  //바둑알 밀어내는 서보모터
int servo2 = 6;  //바둑알 구분하는 서보모터
int button = 3;  //시작 종료 버튼
int irSensor = A0;  //적외선 IR 센서 연결

int maxIrValue = 500;  //IR 센서의 검은색 구분값
int minIrValue = 499;  //IR 센서의 흰색 구분값
  
bool isStart = false;  //분류 시작 유무

unsigned long currentTime = 0;   //현재시간
unsigned long previousTime = 0;   //시간 기억 변수


///////  상태에 맞게 초기값을 변경하면서 맞춰주세요.  ///////////
int initServo_1 = 120;   //servo1 의 초기각도  
int moveServo_1 = 95;   //servo1 의 움직이는 각도

int initServo_2 = 95;   //servo2 의 초기각도
int moveServo_21 = 5;  //servo2 의 왼쪽 이동 각도
int moveServo_22 = 185;  //servo2 의 오른쪽 이동 각도

int firstMotionDelay = 200;  //servo1 밀어낸후 대기시간
int secondMotionDelay = 100;  //servo2 바둑알 옳긴 후 대기시간
///////  상태에 맞게 초기값을 변경하면서 맞춰주세요.  ///////////



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
    
  myservo.attach(servo1); 
  myservo2.attach(servo2); 

  myservo.write(initServo_1);
  myservo2.write(initServo_2);

  //버튼제어
  pinMode(button,INPUT_PULLUP);
  attachInterrupt(1, chkbutton, CHANGE);
}

void loop() {
  //현재시각 구하기 >> 버튼 연속 두번 클릭된 경우 방지 위해 사용
  currentTime = millis();
  
  //버튼이 눌려 시작
  if (isStart) {
    
    //IR 센서 값을 읽어서 sValue 변수에 담는다.
    int sValue = analogRead(irSensor);


    ///////////////  흰색, 검은색에 맞게 바둑알 이동시켜준다. //////////////////
    //흰색, 검은색 적외선 범위인 경우
    if (sValue >= maxIrValue || sValue <= minIrValue) {
      if (sValue >= maxIrValue) {  //검은색
        fncServo2_Left();  //왼쪽으로
      } else {  //흰색
        fncServo2_Right();  //오른쪽으로
      }    
      //Serial.println(sValue);  
      delay(secondMotionDelay);   //흑백에 맞게 바둑알 이동 후 대기
      ///////////////  흰색, 검은색에 맞게 바둑알 이동시켜준다. //////////////////
      
      
      ///////////////  바둑알 밀어준다  //////////////////
      fncServo1();  //바둑알을 밀어준다.
      delay(firstMotionDelay);  //밀고 대기중
      ///////////////  바둑알 밀어준다  //////////////////
    }
  } else {  //종료
    delay(100);
  }
  
}

//servo1의 움직임
void fncServo1() {
  myservo.write(moveServo_1);
  delay(100);
  myservo.write(initServo_1);
  
}
//servo2의 왼쪽 움직임
void fncServo2_Left() {  
  for(int i=initServo_2; i>=moveServo_21; i-=3) {
    myservo2.write(i);
    delay(8);
  }    
  for(int i=moveServo_21; i<=initServo_2; i+=5) {
    myservo2.write(i);
    delay(5);
  }  
}
//servo2의 오른쪽 움직임
void fncServo2_Right() {
  for(int i=initServo_2; i<=moveServo_22; i+=3) {
    myservo2.write(i);
    delay(8);
  }    
  for(int i=moveServo_22; i>=initServo_2; i-=5) {
    myservo2.write(i);
    delay(5);
  }  
}
//버튼 클릭 이벤트
void chkbutton() { 
   
  int bValue = digitalRead(button);
  Serial.print("bValue - ");
  Serial.println(bValue);
  //currentTime > previousTime + 100 >> 버튼한번 눌려도 거의 동시간대에 2,3번 눌린효과가 간혹 발생하는것 차단
  if (bValue == 0 && currentTime > previousTime + 100) {  //버튼 눌리면    
    Serial.println("button click");  
    isStart = !isStart;
    
    //현재시각을 버튼 눌린 시간으로 갱신
    previousTime = currentTime;
  }
}
