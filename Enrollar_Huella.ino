/******************Almacenar Huellas en el Sensor Dactilar*******************************************
 *                                                                                                  *
 *    Este sketch nos servira para poder almacenar nuestras huellas dactilares en nuestro Sensor    *
 *  tendremos espacio para 162 huellas diferentes que es lo que nos permite.                        * 
 *                                                                                                  *
 *  Este codigo lo podemos encontrar como ejemplo de la libreria Adafruit_Fingerprint.h             *
 ****************************************************************************************************/

/***********LIBRERIAS************/
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

uint8_t huella(int num);
/************OBJETOS**************/
SoftwareSerial puerto(7,6);
Adafruit_Fingerprint lector = Adafruit_Fingerprint(&puerto);

void setup(){
  Serial.begin(9600);                 //Iniciamos la Comunicacion Serial
  Serial.println("Buscando Sensor");  
  lector.begin(57600);                //Iniciamos la Comunicacion con el Sensor De Huellas
  
  //Verificamos si el sensor de huellas esta conectado
  if(lector.verifyPassword()){    
  Serial.println("Sensor de huella encontrado :D !!");  
  }
  else{
  Serial.println("Sensor de Huella no encontrado :c !!");
  while(1);
  }
}

void loop(){
  Serial.println("Escriba el # ID que desea guardar");  
  int num = 0;  
  while(true){  
    while(! Serial.available());
    char c = Serial.read();   //cambiar a entero
    if(! isdigit(c)) break;
    num *= 10;
    num += c - '0';
  }
  Serial.print("Almacenando # ID");
  Serial.println(num);
    
  while(! huella(num));        
  }
  
  uint8_t huella(int num){  
    int i = -1;    
    Serial.println("Esperando huella");
    
    while(i != FINGERPRINT_OK){      
    i = lector.getImage();    
    
    switch(i){
    
      case FINGERPRINT_OK:
      Serial.println("Huella leida");
      break;
      
      case FINGERPRINT_NOFINGER:
      Serial.println("Esperando Huella...");
      break;
      
      case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de Comunicacion");
      break;
      
      case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error de huella");
      break;
      
      default: 
      Serial.println("Error desconocido");
      break;
    }
    }
    
    i = lector.image2Tz(1);
    
    switch(i){
    
      case FINGERPRINT_OK:
      Serial.println("Huella Convertida");
      break;
      
      case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen demasiado Sucia");
      return i;
      
      case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      return i;
      
      case FINGERPRINT_FEATUREFAIL:
      Serial.println("Caracteristicas de huella no encontradas");
      return i;
      
      case FINGERPRINT_INVALIDIMAGE:      
      Serial.println("Caracteristicas de huella no encontradas");
      return i;
      
      default: 
      Serial.println("Error desconocido");
      return i;      
    }
    
    Serial.println("Remueva el dedo");
    delay(2000);
    i = 0;
   
    while(i != FINGERPRINT_NOFINGER){
      i = lector.getImage();    
    }
    
    i = -1;
    Serial.println("Coloque de nuevo el dedo ");
    
    while(i != FINGERPRINT_OK){      
    i = lector.getImage();
    
    switch(i){
    
      case FINGERPRINT_OK:
      Serial.println("Huella tomada");
      break;
      
      case FINGERPRINT_NOFINGER:
      Serial.println("Esperando huella");
      break;
      
      case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de comunicacion");
      break;
      
      case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error de HUELLA");
      break;
      
      default:
      Serial.println("Error Desconocido");
      break;      
    }
    }
    
    i = lector.image2Tz(2);
    
    switch(i) {
    
     case FINGERPRINT_OK:
     Serial.println("Huella tomada"); 
     break;
     
     case FINGERPRINT_IMAGEMESS:
     Serial.println("Imagen demasiado Sucia");
     return i;
     
     case FINGERPRINT_PACKETRECIEVEERR:
     Serial.println("Error de Comunicacion");
     return i;
     
     case FINGERPRINT_FEATUREFAIL:
     Serial.println("Caracteristicas de huella no encontradas");
     return i;
     
     case FINGERPRINT_INVALIDIMAGE:     
     Serial.println("Caracteristicas de huella no encontradas");
     return i;
     
     default:
     Serial.println("Error desconocido");
     return i;     
    }
  
     i = lector.createModel();
  
     if(i == FINGERPRINT_OK){
     Serial.println("Las huellas coinciden");
     }
     else if(i == FINGERPRINT_PACKETRECIEVEERR){
     Serial.println("Error de Comunicacion");
     return i;
     }
     else if(i == FINGERPRINT_ENROLLMISMATCH){
     Serial.println("Las huellas dactilares no coinciden");
      return i;  
      }
      else{
      Serial.println("Error desconocido");
      return i;  
      }
      
      Serial.print("ID "); Serial.println(num);
      i = lector.storeModel(num);
      
      if(i == FINGERPRINT_OK){
        Serial.println("Huella Almacenada");      
      }
      else if(i == FINGERPRINT_PACKETRECIEVEERR){
      Serial.println("Error de Comunicacion");
      return i;     
      }
      else if(i == FINGERPRINT_BADLOCATION){
      Serial.println("No se pudo almacenar en esa ubicacion");
      return i;
      }
      else if(i == FINGERPRINT_FLASHERR){
        Serial.println("Error al escribir para flashear");
        return i;            
      }
      else{
      Serial.println("Error desconocido");
      return i;
      } 
  }   
  
