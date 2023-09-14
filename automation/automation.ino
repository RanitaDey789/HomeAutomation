#include<WiFi.h>

const char* ssid="home_automation";
const char* password="00112233";

//extrea
//char servername[]="google.com";

//wifi server object making
WiFiServer server(80);
//taking pins of esp32 9taking random
const int switch1=15; //true value 13
const int switch2=14;  //true value 14
const int switch3=13;
const char* switch1_state="off";
const char* switch2_state="off";
const char* switch3_state="off";
String header;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(switch1,OUTPUT);
  pinMode(switch2,OUTPUT);
  pinMode(switch3,OUTPUT);
  digitalWrite(switch1,HIGH);
  digitalWrite(switch2,HIGH);
  digitalWrite(switch3,LOW);
  Serial.print("Starting wifi access point");
  WiFi.softAP(ssid,password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address");
  Serial.println(IP);

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(client){
    Serial.println("We got a new client");
    String curr_line = "";
    while(client.connected()){        //ip port
      if(client.available()){
        // Serial.println("Client is available");
        char c = client.read();
        Serial.print(c);
        header += c;
        if(c=='\n'){          //if the byte is a new line char(error might happen)
          if(curr_line.length()==0){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if(header.indexOf("GET /s1/on")>=0){
              switch1_state="on";
              digitalWrite(switch1,HIGH);
              Serial.println("switch is on");
            }
            else if(header.indexOf("GET /s1/off")>=0){
              switch1_state="off";
              digitalWrite(switch1,LOW);
              Serial.println("switch is off");
            }
            else if(header.indexOf("GET /s2/on")>=0){
              switch2_state="on";
              digitalWrite(switch2,HIGH);
            }
            else if(header.indexOf("GET /s2/off")>=0){
              switch2_state="off";
              digitalWrite(switch2,LOW);
            }

            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<style>");
            client.println(".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display:inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>ESP32 Home Automation</h1>");
            if(switch1_state=="on"){
              client.println("<a href='/s1/off'><button class='button'>off</button></a>");
            }
            else{
              client.println("<a href='/s1/on'><button class='button'>on</button></a>");
            }
            if(switch2_state=="on"){
              client.println("<a href='/s2/off'><button class='button'>off</button></a>");
            }
            else{
              client.println("<a href='/s2/on'><button class='button'>on</button></a>");
            }

            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          }
          else{
            curr_line="";
          }
        }
        else if(c!='\r'){
          curr_line += c;
        }
      }
    }
    header="";
    client.stop();
    Serial.print("Client is goneeee");
  }

}
