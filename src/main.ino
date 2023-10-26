/*****************************************************************************
Projet: Defit Du Combattant 
Equipe: P6
Auteurs: Bakayoko Kanvali
Description: Programmation Generale du Fonctionnnement du Robot
Date: 22 Octorbre 2023
******************************************************************************/

/* ****************************************************************************
          Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Adafruit_TCS34725.h> // Essencielle pour ..... 
#include <Wire.h> // Essentielle pour .....

/******************************************************************************
                          Inclure les Constantes
*******************************************************************************/


/* ****************************************************************************
                      Variables globales et defines
**************************************************************************** */

// -> defines...
// L'ensemble des fonctions y ont acces

int portBruitAmbiant = 4; // Signal de 1K Hz
int portBruitEntendue = 5; // Signal de 5k Hz

int portDetecteurProximiteVert = 48;
int portDetecteurProximiteRouge = 49;

int setPoint = 0; // Point de consigne (Milieu de la ligne)


double KP = 0.00005; // Constant Proportionnelle
double KI = 0.0000018; // Constant Integrale

/*int capteur1 = 0; // Premier capteur branché a la broche X
int capteur2 = 0; // Deuxieme capteur branché a la broche Y
int capteur3 = 0; // Troisieme capteur branché a la broche Z */


int portSommecapteur = 40; // Le Vout des Capteurs 

double integral = 0;

long millisDebut = 0;
long millisFin = 0;

double correctionSuiveurDeligne = 0; // La correction du suiveur de ligne
double correction = 0; // Valeur de sortie du PID

int valeurAvancerAuDepart = 4085; // Position 1 ou 2 : 4085; Position 3 ou 4 : 8170; Position 5 ou 6 : 12255; Position 7 ou 8 : 16338
int valeurDistanceSurTapis = 8170; // valeur de la distance sur le tapis

int valeurAvancerVirageVert = 9618; // Valeur d'avancement dans le virage dans le vert
int valeurAvancerVirageJaune = 16032; // Valeur d'avancement dans le virage dans le Jaune

int distance_1_LigneBlanche = 17800; // Premier distance 
int distance_2_LigneBlanche = 10600; // Deuxieme distance 
int distance_3_LigneBlanche = 17800; // Troisieme distance 

int valeurTourner = 1007; // Valeur de rotation

double vitesseAvancer = 0.25; // Vitesse des moteurs pour avancer
double correctionPID = 0; // Correction initiale basée sur le PID

/* ****************************************************************************
                    Vos propres fonctions sont creees ici
**************************************************************************** */

// Calcul du PID 

double PID()
{
  millisFin = millis();

  if (millisFin - millisDebut >= 50.0) 
  {
    // Lire les valeurs des encodeurs (nombre de pas parcourus)

    int leftSteps = abs(ENCODER_Read(LEFT));
    int rightSteps = abs(ENCODER_Read(RIGHT));

    // Calcul de l'erreur en fonction des encodeurs 

    int error = leftSteps - rightSteps;

    // Calcul des termes PID

    int P = KP * error;

    integral += error;
    int I = KI * integral;

    // Calcul de la Correction 

    correction = 20.0*(P + I);

    Serial.println(error);
    Serial.println(correction, 10);
    Serial.println("");

    Serial.println(abs(ENCODER_Read(RIGHT)));
    Serial.println(abs(ENCODER_Read(LEFT)));
    Serial.println("");

    millisDebut = millis();
  }
  
  return correction;

}

// Suiveur de ligne

void suiveurDeLigne()
{
  // Lire la valeur des trois capteurs

  float Lecture = analogRead(portSommecapteur); 

  // Logique de suivre la ligne Defit du Combattant

  if (Lecture <= 2.1413 && Lecture >= 2.1321)
  {
    correctionSuiveurDeligne = 0.5;
  }
  else if (Lecture <= 4.2826 && Lecture >= 4.2358)
  {
    correctionSuiveurDeligne = -0.5;
  }
  else if (Lecture <= 5 && Lecture >= 4.9290)
  {
    correctionSuiveurDeligne = 0;
  }
  else
  {
    ;
  }
  
  
  // Logique de suivre la ligne Projet final

/*if (Lecture <= 2.8550 && Lecture >= 2.8332) 
  {
    correctionSuiveurDeligne = 0.05; // Capteur 1 a detecté la ligne 
  }
  else if (Lecture <= 1.4275 && Lecture >= 1.4133) 
  {
    correctionSuiveurDeligne = 0.0; // Capteur 2 a detecté la ligne 
  }
  else if (Lecture <= 0.7259 && Lecture >= 0.7107) 
  {
    correctionSuiveurDeligne = -0.05; // Capteur 3 a detecté la ligne
  }
  else if (Lecture <= 4.2826 && Lecture >= 4.2358) 
  {
    correctionSuiveurDeligne = 0.025; // Capteur 1 et Capteur 2 ont detectées la ligne
  }
  else if (Lecture <= 2.1413 && Lecture >= 2.1321) 
  {
    correctionSuiveurDeligne = -0.025; // Capteur 2 et Capteur 3 ont detectées la ligne
  }
  else if (Lecture <= 3.5689 && Lecture >= 3.5497) 
  {
    correctionSuiveurDeligne = 0.05; // Capteur 1 et Capteur 3 ont detectées la ligne
  }
  else if (Lecture <= 5 && Lecture >= 4.9290) 
  {
    correctionSuiveurDeligne = 0.025; // Capteur 1, 2, et 3 ont detectées
  }
  else
  {
    correctionSuiveurDeligne = 0.0; 
  }*/
}

// Les de rotation de 45 degrées

void rotation_45_Degree()
{
 // Mettez à jour la correction basée sur le PID à chaque itération
    
  correctionPID = PID();

  MOTOR_SetSpeed(RIGHT, -0.35 );
  MOTOR_SetSpeed(LEFT, (0.35 + 0.01 + correctionPID ));

  while (abs(ENCODER_Read(RIGHT)) <= valeurTourner)
  {
    true;
  }
  reset();
} 

// Avance le Robot sur la ligne blanche etape 1

void distance_1()
{
 // Mettez à jour la correction basée sur le PID à chaque itération
    
  correctionPID = PID();
  suiveurDeLigne();

  // Ajustez les vitesses des moteurs en fonction de la correction PID
  MOTOR_SetSpeed(RIGHT, vitesseAvancer + (correctionPID + 0.01) - correctionSuiveurDeligne);
  MOTOR_SetSpeed(LEFT, vitesseAvancer - (correctionPID + 0.01) + correctionSuiveurDeligne);

  while (abs(ENCODER_Read(RIGHT)) <= distance_1_LigneBlanche)/*!suiveurDeLigne*/
  {
    true;
  }
  reset();
} 

// Avance le Robot sur la ligne blanche etape 2

void distance_2()
{
 // Mettez à jour la correction basée sur le PID à chaque itération
    
  correctionPID = PID();
  suiveurDeLigne();

  // Ajustez les vitesses des moteurs en fonction de la correction PID
  MOTOR_SetSpeed(RIGHT, vitesseAvancer + (correctionPID + 0.01) - correctionSuiveurDeligne);
  MOTOR_SetSpeed(LEFT, vitesseAvancer - (correctionPID + 0.01) + correctionSuiveurDeligne);

  while (abs(ENCODER_Read(RIGHT)) <= distance_2_LigneBlanche)/*!suiveurDeLigne*/
  {
    true;
  }
  reset();
} 

// Avance le Robot sur la ligne blanche etape 3

void distance_3()
{
 // Mettez à jour la correction basée sur le PID à chaque itération
    
  correctionPID = PID();
  suiveurDeLigne();

  // Ajustez les vitesses des moteurs en fonction de la correction PID
  MOTOR_SetSpeed(RIGHT, vitesseAvancer + (correctionPID + 0.01) - correctionSuiveurDeligne);
  MOTOR_SetSpeed(LEFT, vitesseAvancer - (correctionPID + 0.01) + correctionSuiveurDeligne);

  while (abs(ENCODER_Read(RIGHT)) <= distance_3_LigneBlanche)/*!suiveurDeLigne*/
  {
    true;
  }
  reset();
} 

// Distance sur le tapis

void roulerSurTapis()
{ 
  MOTOR_SetSpeed(LEFT, 0.25 + PID()); // Esclave
  MOTOR_SetSpeed(RIGHT, 0.25); // Maitre
  
  while ((abs(ENCODER_Read(RIGHT)) <= valeurDistanceSurTapis))
  {
    true;
  }
}

// Au virage 1 et 2

void tournerSurVirageAvecligneVert()
{
  MOTOR_SetSpeed(LEFT, 0.35 + 0.01   + PID());  // Esclave
  MOTOR_SetSpeed(RIGHT, 0.25); // Maitre

  while (abs(ENCODER_Read(RIGHT)) <= valeurAvancerVirageVert)
  {
    true;
  }
}

// Au virage 1 et 2

void tournerSurVirageAvecligneJaune()
{
  MOTOR_SetSpeed(LEFT, 0.35 + 0.01 + PID());  // Esclave
  MOTOR_SetSpeed(RIGHT, 0.25); // Maitre

  while (abs(ENCODER_Read(RIGHT)) <= valeurAvancerVirageJaune)
  {
    true;
  }
}

// Detection de Couleur

void detectionCouleur()
{
  uint16_t red, green, blue, clear;
  Adafruit_TCS34725 colorSensor;

  colorSensor.begin(); // Lorsque la LED du capteur de couleur est allumée, ledStatus = 1 ; Lorsque la LED du capteur de couleur est éteinte, ledStatus = 0.
  
  while(1)
  {
    colorSensor.getRawData(&red, &green, &blue, &clear);    //Read RGB values to variables.

    delay(300);

    Serial.print("The RGB value are: RGB( ");
    Serial.print(red,DEC);
    Serial.print(", ");
    Serial.print(green,DEC);
    Serial.print(", ");
    Serial.print(blue,DEC);
    Serial.println(" )");
    colorSensor.clearInterrupt();
  }   
}

// Detection de ligne noir avant le point de depart

bool detectionLigneNoir()
{
  // Lire la valeur des trois capteurs

  float Lecture2 = analogRead(portSommecapteur); 

  // Logique de suivre la ligne

  while (Lecture2 == 0) 
  {
    return true;
  }
}

// Detecteur de Sifllet

void detectionSifflet()
{
  double A4 = 0, A5 = 0;
  for (int i = 0; i < 500; i++)
  {
    while ((A5 - A4) < 110) // Robot A:220 Robot B : 110
    {
      Serial.println(A5 - A4);
      A4 = analogRead(portBruitAmbiant);
      A5 = analogRead(portBruitEntendue);
      delay(1);
    }
  }
}

// Servomoteur 

void Servomoteur()

{
  SERVO_Enable(0);
  SERVO_Enable(1);
if (abs(ENCODER_Read(RIGHT) <= ))
{
  /* code */
}

  SERVO_SetAngle(0, 45);
  SERVO_SetAngle(1, 45);

}

// Detecteur de Proximité

bool detection()
{
  return (digitalRead(portDetecteurProximiteVert) == 0 && digitalRead(portDetecteurProximiteRouge) == 0); // Renvoie vrai si il détecte quelque chose
}

// Remise a Zero  

void reset()
{
  // Du moteur
  
  MOTOR_SetSpeed(RIGHT, 0.0);
  MOTOR_SetSpeed(LEFT, 0.0);

  // Delais pour décharger le CPU (Processeur)

  delay(150);

  // De l'encodeur

  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
}


/* ****************************************************************************
                    Fonctions d'initialisation (setup)
**************************************************************************** */

// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales


void setup()
{
  BoardInit();

  pinMode(portSommecapteur, INPUT);

  Wire.begin();

  Serial.begin(9600);

  /*ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
 
  rotation_45_Degree();
  distance_1();
  rotation_45_Degree();
  distance_2();
  rotation_45_Degree();
  distance_3();
  // Servomoteur();

  tournerSurVirageAvecligneVert();
  roulerSurTapis();
  tournerSurVirageAvecligneVert();*/
 
}


/* ****************************************************************************
                    Fonctions de boucle infini (loop())
**************************************************************************** */

// -> Se fait appeler perpetuellement suite au "setup"

void loop() 
{
  // SOFT_TIMER_Update(); // A decommenter pour utiliser des compteurs logiciels

  delay(10);  // Delais pour décharger le CPU
}