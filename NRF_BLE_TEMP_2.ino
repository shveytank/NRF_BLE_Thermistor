#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>

RF24 radio(9, 10); // CE, CSN
BTLE btle(&radio);

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.print("BLE Starting... ");
  Serial.println("Send Temperature Data over BTLE");
  btle.begin("NRF TMP");    // 8 chars max
  Serial.println("Successfully Started");
}
int i;
void loop() {  
  i=analogRead(A0);                                            
  float temp = (float)Thermister(i);   //read temperature data
  
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(temp);

  if (!btle.advertise(0x16, &buf, sizeof(buf))) {
    Serial.println("BTLE advertisement failed..!");
  }
  btle.hopChannel(); 
  delay(2000);
}


double Thermister(int data)
{
  double temp;
  temp=log(10000.0*((1024.0/data-1)));
  temp=1/(0.001129148+(0.000234125+(0.0000000876741*temp*temp))*temp);
  temp=temp-273.15;
  Serial.println(" ");
  Serial.print(temp);
  Serial.print(" Celcius");
  return temp;
  }
