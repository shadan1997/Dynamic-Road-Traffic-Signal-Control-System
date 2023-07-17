//   upload on COM port 3
// Board ardino Ethernet  UNO
// C++ code
// ARDINO WITH ETHERNET



/********************************
 *         Ethernet connection
 *******************************/
 
#include <Ethernet.h>

/* Enter a MAC address for your controller below */
/* Newer Ethernet shields have a MAC address printed on a sticker on the shield */
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xE2, 0xCD };

void ethernet_setup(){
  //At home
  IPAddress ip(192, 168, 200, 145);
  IPAddress myDns(8,8,8,8);
  IPAddress gateway(192,168,200,1);
  IPAddress subnet(255,255,255,0);
  
  /*//At jamia research Lab
  IPAddress ip(10, 32, 0, 137); 
  IPAddress myDns(10,2,0,10);
  IPAddress gateway(10,32,0,1);
  IPAddress subnet(255,255,0,0);
  */
  /*  //At jamia 
  IPAddress ip(10, 32, 1, 37); //ip(192, 168, 200, 145)
  IPAddress myDns(8,8,8,8);
  IPAddress gateway(10,32,0,1);
  IPAddress subnet(255,255,0,0);
  */

  
    /* start the Ethernet connection */
  Serial.println("[INFO] Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("[Error] Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("[Error] Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("[Error] Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns , gateway, subnet);
   } else {
    Serial.print("[INFO]  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
    Serial.println(Ethernet.dnsServerIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}



/********************************
 *         Ubidots
 *******************************/
#include <SPI.h>
#include <UbidotsEthernet.h>

/* Assigns the Ubidots parameters */

char const * TOKEN = "BBFF-oridfscoPfCkZmL2301z4cqZIHVgPS"; // Assign your Ubidots TOKEN
char const * DEVICE_LABEL = "traffic"; // Assign the unique device label

char const * VARIABLE_LABEL_1 = "path1"; // Assign the unique variable label to get the last value
char const * VARIABLE_LABEL_2 = "path2"; 
char const * VARIABLE_LABEL_3 = "path3"; 
char const * VARIABLE_LABEL_4 = "path4";
char const * VARIABLE_LABEL_5 = "flag"; 


/* initialize the instance */
Ubidots client(TOKEN);

unsigned long int id=0;
int check_data_on_server(){
  unsigned long int new_id=client.getValue(DEVICE_LABEL, VARIABLE_LABEL_5) ;
  //delay(1);
  if (id<new_id){
    id=new_id;
    Serial.println(F("[INFO] New Data available "));
    return 1;
  }
  Serial.print(F("[ERROR] Data not available "));
  return 0;
}

int* read_data()
{
  /* initialize the instance */
  /* Getting the last value from a variable */
  //unsigned long int flag = (client.getValue(DEVICE_LABEL, VARIABLE_LABEL_5));
  delay(1000);
  int a[4];
  a[0] = client.getValue(DEVICE_LABEL, VARIABLE_LABEL_1);
  delay(1000);
  a[1] = client.getValue(DEVICE_LABEL, VARIABLE_LABEL_2);
  delay(1000);
  a[2] = client.getValue(DEVICE_LABEL, VARIABLE_LABEL_3);
  delay(1000);
  a[3] = client.getValue(DEVICE_LABEL, VARIABLE_LABEL_4);
  delay(1000);
 
  /* Print the value obtained */
  Serial.print(F("[INFO] Value received : ["));
  Serial.print(id);Serial.print(", ");
  Serial.print(a[0]);Serial.print(", ");
  Serial.print(a[1]);Serial.print(", ");
  Serial.print(a[2]);Serial.print(", ");
  Serial.print(a[3]);Serial.print("]");
  
  return a;
}


////////////////////////////////////////////////////////////
//             Master slave communication                 //
//////////////////////////////////////////////////////////*/

#include<Wire.h>

void sender(int* a){
  Serial.print(F("[INFO] Data sent to slave."));
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(id);
  Wire.write(a[0]);              // sends x 
  Wire.write(a[1]); 
  Wire.write(a[2]); 
  Wire.write(a[3]); 
  Wire.endTransmission();    // stop transmitting
}



/********************************
 *          setup
 *******************************/
void setup() {
  // Start the I2C Bus as Master
  Serial.begin(9600);
  ethernet_setup();
  client.setDebug(true);
  Wire.begin(); 
  Serial.println(F("\n[INFO] Setup Done."));
}

/********************************
 * main
 *******************************/

void loop() {
  Ethernet.maintain();
  Serial.println(F("\n[INFO] EThernet maintain Done."));
  //if (check_data_on_server()){
  if (true){
  int* data  = read_data();
  sender(data);
  }
  Serial.println(F("\n[INFO] Loop end."));
  delay(4000);
}

/*
void loop1() {
  int a[4] = {11,2,32,4};
  sender(a);
  delay(5000);
}*/
