
// upload on COM port 5
// Board ardino UNO
// Include the required Wire library for I2C

///////////////////////////////////////////////
//          setup communications             //    
///////////////////////////////////////////////

#include <Wire.h>
static int a[4];
static unsigned long int flag=0;


/********************************
 *          setup
 *******************************/
void setup() {
  setup_light();     // setup lights section 
  Serial.begin(9600);
  // Define the LED pin as Output
  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
//  Wire.onRequest(requestEvent);     // Register a function to be called when a master requests data from this slave device

}
void receiveEvent(int bytes) {
  flag = Wire.read();
  a[0] = Wire.read();    // read one character from the I2C
  a[1] = Wire.read();
  a[2] = Wire.read();
  a[3] = Wire.read();
}

void received_data(){
  Serial.print("\n[ INFO ]The value received is: [");
  Serial.print(a[0]);Serial.print(", ");
  Serial.print(a[1]);Serial.print(", ");
  Serial.print(a[2]);Serial.print(", ");
  Serial.print(a[3]);Serial.print("]");
}

/********************************
 *          main
 *******************************/

void loop() {
  //check_up();
  Serial.print("\n[ INFO ] loop running ");                                                                                                                                                                                                                                                                   
  if (flag){
    Serial.print("\n[ LOOP ] connecting to  Master ardino...");
    received_data();
    Serial.print("\n[ LOOP ] connected");
    dynamic(a);
  }    
  else{
    Serial.print("\n[ Error ] Unable to communicate ");
    factory();
  }
   
}

///////////////////////////////////////////////
//              setup lights                 //    
///////////////////////////////////////////////


#define N 4   // no of traffic lights
int pin[N][3]={{8,9,10},{13,12,11},{7,6,5},{4,3,2}};


void set_all_pin_to_low(){
  for(int i=0;i<N;i++)
    for (int j=0;j<3;j++) 
      digitalWrite(pin[i][j],0);
}

void set_other_red(int cur){
  for(int i=0;i<N;i++){
    if (i == cur)
      digitalWrite(pin[i][0],0);
    else
      digitalWrite(pin[i][0],1);
  }
}

void check_up(){
  for(int i=0;i<N;i++){
    for (int j=00;j<3;j++){        
    delay(1000);
    digitalWrite(pin[i][j],1);
    delay(1000);
    digitalWrite(pin[i][j],0);}
    }  
}


void setup_light()
{
  for(int i=0;i<N;i++)
    for (int j=0;j<3;j++)
      pinMode(pin[i][j], OUTPUT);
  
  set_all_pin_to_low();
}


void active(int pin_no,int gap){
  digitalWrite(pin_no,1);
    delay(gap);
    digitalWrite(pin_no,0);

}

void factory(){
  Serial.print("\n[ INFO ] Default mode active...");
  for(int i=0;i<N;i++){
    set_all_pin_to_low();
    set_other_red(i);
    active(pin[i][2],6000);//green
    active(pin[i][1],1000);//yellow
  }
  Serial.print("\n[ INFO ] Default mode end...");
}


/*
0 //no one there      0 sec
1 //1-3 range low   5 sec
2 //4-8 range mid   7 sec
3 //9>    high    9 sec
*/

int clock(int t,int x){
  if(t==0)
      return 0;
    else if(t<x)
      return 5000;
    else if(t<x*2)
      return 7000;
    else 
      return 9000;
}

void dynamic(int data[]){
  Serial.print("\n[ INFO ] Dynamic mode started...");
    flag=0;
    int p=0;
    int large=data[0];
    for (int i=1;i<N;i++){
    if (large<data[i])
      large=data[i];
    }
    Serial.println("[INfo] Data");
    Serial.println(data[0]);
    Serial.println(data[1]);
    Serial.println(data[2]);
    Serial.println(data[3]);
    p=large/(N-1);
    Serial.println("[INfo] threshold:");
    Serial.println(p);
    Serial.println(2*p);
    for(int i=0;i<N;i++){
      int time=clock(data[i],p);
      Serial.print("[Info] light[");
      Serial.print(i+1);
      Serial.print("]:");
      Serial.print("time = ");
      Serial.println(time/1000);
      if(time==0) 
        continue;  
      set_all_pin_to_low();
      set_other_red(i);
      active(pin[i][2],time-1000);//green
      active(pin[i][1],1000);//yellow
    }  
  Serial.print("\n[ INFO ] Dynamic mode end");  
}
