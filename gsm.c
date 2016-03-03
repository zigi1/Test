#include <SoftwareSerial.h>
#include <ctype.h>
#include <string.h>

int i , j , k , l , n;
unsigned char gprsbuffer[200] ; 
String content;
SoftwareSerial serialGPRS(7, 8);
int count=0;     // counter for buffer array 

void setup()
{  
    Serial.begin(9600);             

  serialGPRS.begin(4800);               
  delay(500);
}
void loop()
{
  
  if (Serial.available())
    switch(Serial.read())
   {
     case 't':
       arcmr();
       break;
     case 'd':
       cmism();
       break;
   }
  delay(200);
  //content = "";
  if (serialGPRS.available())          
    {
    //delay(0);
    while(serialGPRS.available())          
    {
       gprsbuffer[count++]=serialGPRS.read();     
    }
    delay(100);
    Serial.write(gprsbuffer,count);    
     //delay(100);
     
    for (i = 0; i <= count; i++) {
      if(gprsbuffer[i] == 10 || gprsbuffer[i] == 13)
        gprsbuffer[i] = 44 ;
   }
   
  content = "";
    for (i = 0; i <= count; i++) {
      content += char(gprsbuffer[i]);
   }
  contentcleaner();
  delay(100);
  Serial.println();
  delay(100);
  Serial.print("Content:" + content );
  delay(100);
  if(content.indexOf("ERROR",0) >= 0 ){
    content = "";
  }else if(content.indexOf("+CMGR:") >= 0 ){
    int lastcommaIndex = content.lastIndexOf(',');

    String lastValue = content.substring(lastcommaIndex+1);
    Serial.println();
    delay(200);
    Serial.println("The content inside cmgr is :"+content);
    delay(200);
    Serial.println("The indexs is :" + String(lastcommaIndex+1));
    delay(200);
    Serial.println("The lastValue is :" + lastValue);   
    
  } else if(content.indexOf("+CMTI:") >= 0 ){ //+CMTI: "SM",3

    int commaIndex = content.indexOf(',');
    int secondCommaIndex = content.indexOf(',', commaIndex+1);
    String firstValue = content.substring(0, commaIndex);
    String secondValue = content.substring(commaIndex+1, secondCommaIndex);
    content = 0;
    Serial.println();
    delay(100);
    serialGPRS.println("AT+CMGR="+secondValue);
    delay(1000);
    
  }else {
     Serial.println("##");
     delay(100);
  }
   clearBufferArray();               // call clearBufferArray function to clear the storaged data from the array
    count = 0;
  }
  
   //content = "";
  if (serialGPRS.available())
    Serial.write(serialGPRS.read()); 
   delay(1000); 
}

void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { gprsbuffer[i]=NULL;}                  // clear all index of array with command NULL
} //
 
void arcmr()
{
  serialGPRS.println("AT+CMGR=1");
  delay(100);
  Serial.println();
} 
 
void cmism()
{
  serialGPRS.println("+CMTI: \"SM\",1");
  delay(100);
  Serial.println();
} 

void contentcleaner(){
  
   content.replace('\n',0);//   
    content.replace(",,,,",",");//   
       
    content.replace(",,,",",");//   
           
    content.replace(",,",",");//   

    content.replace(",OK,",0);//   
    if(content[0] == ',' )
      content[0] =0;
    if(content[count] == ',' )
      content[count] =0;
    //Serial.println();       
    //Serial.print("content after replace : "); 
    //Serial.println(content);
}
