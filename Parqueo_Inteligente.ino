/*************************************PARQUEO INTELIGENTE CON SENSOR DACTILAR******************************************************
*                                                                                                                                 *
*     Este sistema depende de una confirmacion de una huella ya almacenada en el sensor dactilar, si la huella coincide           *
*  dara la apertura del servomotor por 10 segundos. Luego de los diez segundos el servomotor bloqueara el paso.                   *
*                                                                                                                                 *
*  Para lograr esto necesitaremos la libreria del sensor dactilar que nos proporciona Adafruit Adafruit_Fingerprint.h             *
*                                                                                                                                 *
*  Tambien la libreria SoftwareSerial y Servo que incluye ya el IDE de Arduino                                                                  *
*                                                                                                                                 *
*  Primero cargaremos el codigo "Enrolar Huella" de la libreria Adafruit_Fingerprint.h para poder almacenar nuestras huellas   *
*  tenemos un espacio de 162 huellas diferentes.                                                                                  *
*                                                                                                                                 *
*  Luego de cargar el codigo de "Enrolar Huella" ahora si ya cargaremos nuestro codigo.                                                   *
*                                                                                                                                 *
*                                                                                                                                 *
*                                                 #NuncaDejenDeCrear                                                              *
***********************************************************************************************************************************/                                                                                                                              

/**********LIBRERIAS************/
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

int getFingerprintIDez();

/*************************OBJETOS****************************/
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo puerta;

void setup() {
  puerta.attach(9);                   //Indicamos que conectaremos el Servo al pin 9 
  puerta.write(90);                   //Le damos una posicion Inicial de Cerrado
  Serial.begin(9600);                 //Iniciamos la Comunicacion Serial
  Serial.println("prueba de dedo");
  finger.begin(57600);                //Iniciamos la Comunicacion con el Sensor 
  //Verificamos que el sensor este conectado c:
  if (finger.verifyPassword()) {
    Serial.println("Sensor de huella digital encontrado!");
  } else {
    Serial.println("No encontré el sensor de huellas dactilares:(");
    while (1);
  }
}

void loop() {
  //Llamamos el metodo Obtener ID Huella Digital 
    getFingerprintIDez();     
    delay(50);      
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen Tomada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Sin Dedo Detectado");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de Comunicacion");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Error de Imagen");
      return p;
    default:
      Serial.println("Error Desconocido");
      return p;
  }
  // OK, Exito!
  
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagen Convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagen Demasiado Sucia");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Error de Comunicacion");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("No he podido encontrar las caracteristicas de la huella dactilar");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("No he podido encontrar las caracteristicas de la huella dactilar");
      return p;
    default:
      Serial.println("Error Desconocido");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Encontré una coincidencia de huellas!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Error de Comunicacion");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("No se encontro coincidencia");
    return p;
  } else {
    Serial.println("Error Desconocido");
    return p;
  } 
  
  // Encontré una coincidencia.
  Serial.print("Encontrado ID #"); Serial.print(finger.fingerID); 
  Serial.print(" Con confidencia de: "); Serial.println(finger.confidence); 
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  //Aqui se hacen las comparaciones para abrir 
  //colocaran las huellas que quieren que Aperturen el Servomotor
  if(finger.fingerID == 1){
  Serial.println("Bienvenido"); 
  puerta.write(0);   //Aperturamos el Servo 
  }
    if(finger.fingerID == 2){
  Serial.println("Bienvenido");  
  puerta.write(90);  //Luego lo regresamos a su posicion de Cerrado
  }

  
  // Encontré una coincidencia.
  Serial.print("Encontrado ID #"); Serial.print(finger.fingerID); 
  Serial.print(" Con confidencia de: "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
