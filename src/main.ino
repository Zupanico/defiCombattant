/*
Projet: Defi du combattant
Equipe: P6
Auteurs: Evan Frappier, Nicolas Garant, Félix Bordeleau, Kanvali Bakayoko, Antoine Caya, Louis Dumas, Kabongo Badibanga
Description: Permet de guider le robot dans le labyrinthe
Date: 21/10/2023
*/

// Librairies
// ============================
#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 colorSensor;
// Variables globales et defines
// ============================

int tour = 1;
long millisDebut = 0;
long millisFin = 0;
double KP = 0.000072; // Robot B : 0.00005
double KI = 0.00000092; // Robot B : 0.0000018
double erreurKI;
double erreurKP;

float capteurLigneNoire = 0;
bool capteurVerre = false;

// Fonctions
// ============================
double PID()
{

  millisFin = millis();
  // Serial.println(ValPID);
  double ValPID = 0;

  if ((millisFin - millisDebut) >= 50.0)
  {
    int valeurEncodeurDroit = abs(ENCODER_Read(RIGHT));
    int valeurEncodeurGauche = abs(ENCODER_Read(LEFT));

    erreurKP = valeurEncodeurDroit - valeurEncodeurGauche;
    erreurKI += erreurKP;

    ValPID = (1 / 0.05) * (erreurKP * KP + erreurKI * KI);

    Serial.println(erreurKP);
    Serial.println(ValPID, 10);
    Serial.println("");
    Serial.println(abs(ENCODER_Read(RIGHT)));
    Serial.println(abs(ENCODER_Read(LEFT)));
    Serial.println("");
    delay(1);
    millisDebut = millis();
  }

  return ValPID;
}

void rotation_180()
{
  int valeurTourner = 3970; //TROUVER LA VALEUR POUR FAIRE UN 180
  float distanceRestante = 0;
  float MOTORSPEED = 0;
 
  while (abs(ENCODER_Read(RIGHT)) <= valeurTourner) // Boucle qui s'arrête selon la valeur du compteur car les encodeur sont reset à chaque utiliation du pid
  {
    if ((abs(ENCODER_Read(RIGHT))) <= valeurTourner * 0.60)
    {
      MOTOR_SetSpeed(RIGHT, -0.35);         // Maitre
      MOTOR_SetSpeed(LEFT, (0.35 + PID())); // Esclave
    }
    else
    {
      distanceRestante = 1 - (abs(ENCODER_Read(RIGHT)) / valeurTourner);
      MOTORSPEED = (-0.625 * distanceRestante) + 0.725;
      MOTOR_SetSpeed(RIGHT, -(MOTORSPEED)); // Maitre
      MOTOR_SetSpeed(LEFT, MOTORSPEED + PID());
    }
  }
  reset();
}
void RECULER()
{
  MOTOR_SetSpeed(RIGHT, -0.15);          // Maitre
  MOTOR_SetSpeed(LEFT, (-0.15 + PID())); // Esclave
  delay(500);
 
  while (capteurVerre != 1) // Boucle qui s'arrete quand il capte le verre
  {
    MOTOR_SetSpeed(RIGHT, -0.30);          // Maitre
    MOTOR_SetSpeed(LEFT, (-0.30 + PID())); // Esclave
  }
  reset();
}

void reset()
{

  MOTOR_SetSpeed(RIGHT, 0.0);
  MOTOR_SetSpeed(LEFT, 0.0);
  // Delais pour décharger le CPU
  delay(200);
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
}

int detectionCouleur()
{

  uint16_t red, green, blue, clear;
 
  colorSensor.begin(); // Lorsque la LED du capteur de couleur est allumée, ledStatus = 1 ; Lorsque la LED du capteur de couleur est éteinte, ledStatus = 0.
  colorSensor.getRawData(&red, &green, &blue, &clear);    //Read RGB values to variables.
  unsigned int color = colorSensor.calculateColorTemperature(red, green, blue); //Convert RGB values to color temperature.
  //Serial.println(color);
  if (color < 2000)
  {
    return 1; // Rouge
  }
  else if (color < 3500)
  {
    return 2; // Jaune
  }
  else if (color < 5000)
  {
    return 0; // blanc
  }
  else if (color < 7000)
  {
    return 3; // Vert
  }
  else
  {
    return 4; // Bleu
  }
}

void rotation_droite()
{
  Serial.println(1);
  double rayon = 0;
  double distancetourner = 0;
  int couleur = detectionCouleur();
  switch (couleur)
  {
    case 4:
      rayon = 15.0;
      distancetourner = 10000;
      Serial.println("default");
      break;
 
    case 3:
      rayon = 34.0;
      distancetourner = 10000;
      Serial.println("default");
      break;
    case 2:
      rayon = 76.0;
      distancetourner = 10000;
      Serial.println("jaune");
      break;
    case 1:
      rayon = 105.0;
      distancetourner = 1000;
      Serial.println("default");
      break;
    default:
      // Handle the case where couleur is not 1, 2, 3, or 4
      Serial.println("default");
      break;
  }
  Serial.println(3);
  while (ENCODER_Read(RIGHT) <= distancetourner){
    Serial.println(4);
    MOTOR_SetSpeed(LEFT, 0.4);
    MOTOR_SetSpeed(RIGHT, 0.2 + PID_rotation(rayon));
  }
  reset();
}
 
double PID_rotation(double rayon)
{

  millisFin = millis();

  if ((millisFin - millisDebut) >= 50.0)
  {
    double arc = PI * (rayon/2.0);
    const float distanceRoues = 19.0;

    int valeurEncodeurDroit = (abs(ENCODER_Read(LEFT)))*(distanceRoues/arc);

    //int valeurEncodeurDroit = abs(ENCODER_Read(RIGHT));
    //int valeurEncodeurGauche = abs(ENCODER_Read(LEFT));

    erreurKP = valeurEncodeurDroit - abs(ENCODER_Read(RIGHT));
    erreurKI += erreurKP;

    double ValPID = (1 / 0.05) * (erreurKP * KP + erreurKI * KI);

    Serial.println(erreurKP);
    Serial.println(ValPID, 10);
    Serial.println("");
    Serial.println(abs(ENCODER_Read(RIGHT)));
    Serial.println(abs(ENCODER_Read(LEFT)));
    Serial.println("");
  delay(1);
    millisDebut = millis();
  }
}

void AVANCER()
{
  //Si le robot est sur une couleur, il va arreter d'avancer quand il n'en verra plus(Blanc ou noir)
  if(detectionCouleur() != 0)
  {
    MOTOR_SetSpeed(RIGHT, 0.15);          // Maitre
    MOTOR_SetSpeed(LEFT, (0.15 + PID())); // Esclave
    delay(500);
    while (detectionCouleur() == 0)
    {
      MOTOR_SetSpeed(RIGHT, 0.30);          // Maitre
      MOTOR_SetSpeed(LEFT, (0.30 + PID())); // Esclave
    }
  }
  //Si le robot n'est pas sur une couleur, il va arreter d'avancer quand il en verra une
  if(detectionCouleur() == 0)
  {
    MOTOR_SetSpeed(RIGHT, 0.15);          // Maitre
    MOTOR_SetSpeed(LEFT, (0.15 + PID())); // Esclave
    delay(500);
    while (detectionCouleur() != 0)
    {
      MOTOR_SetSpeed(RIGHT, 0.30);          // Maitre
      MOTOR_SetSpeed(LEFT, (0.30 + PID())); // Esclave
    }
  }
  reset();
}
 
void AVANCER_HAUTEUR_LIGNE_NOIRE()
{
  int valeurAvancer = 6682; //VALEUR A TROUVER POUR AVANCER JUSQU'A LIGNE NOIRE
 
  while ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer)
  {
    if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.40)
    {
      MOTOR_SetSpeed(RIGHT, 0.30);          // Maitre
      MOTOR_SetSpeed(LEFT, (0.30+0.01 + PID())); // Esclave
    }
    else if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.70)
    {
      MOTOR_SetSpeed(RIGHT, 0.40);          // Maitre
      MOTOR_SetSpeed(LEFT, (0.40+0.01 + PID())); // Esclave
    }
    else if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.90)
    {
      MOTOR_SetSpeed(RIGHT, 0.20); // Maitre
      MOTOR_SetSpeed(LEFT, (0.20+0.01 + PID()));
    }
    else
    {
      MOTOR_SetSpeed(RIGHT, 0.1); // Maitre
      MOTOR_SetSpeed(LEFT, (0.1+0.01 + PID()));
    }
  }
  reset();
}
 
void TOURNERDROITE()
{
  float distanceRestante = 0;
  float MOTORSPEED = 0;
  int valeurTourner = 1985;
 
  while (abs(ENCODER_Read(RIGHT)) <= valeurTourner) // Boucle qui s'arrête selon la valeur du compteur car les encodeur sont reset à chaque utiliation du pid
  {
    if ((abs(ENCODER_Read(RIGHT))) <= valeurTourner * 0.60)
    {
      MOTOR_SetSpeed(RIGHT, -0.35);         // Maitre
      MOTOR_SetSpeed(LEFT, (0.35 + PID())); // Esclave
    }
    else
    {
      distanceRestante = 1 - (abs(ENCODER_Read(RIGHT)) / valeurTourner);
      MOTORSPEED = (-0.625 * distanceRestante) + 0.725;
      MOTOR_SetSpeed(RIGHT, -(MOTORSPEED)); // Maitre
      MOTOR_SetSpeed(LEFT, MOTORSPEED + PID());
    }
  }
  reset();
}
 
void AVANCER_LIGNE_NOIRE()
{
  MOTOR_SetSpeed(RIGHT, 0.20);          // Maitre
  MOTOR_SetSpeed(LEFT, (0.20+0.01 + PID())); // Esclave
 
  while (capteurLigneNoire == 0)
  {
    MOTOR_SetSpeed(RIGHT, 0.30);          // Maitre
    MOTOR_SetSpeed(LEFT, (0.30+0.01 + PID())); // Esclave
  }
  reset();
}

void ALLER_LIGNER_NOIRE()
{
  AVANCER_HAUTEUR_LIGNE_NOIRE(); //Avancer jusqu'a hauteur ligne noire
  TOURNERDROITE(); //Tourne droite de 90
  AVANCER_LIGNE_NOIRE(); //Avance jusqu'a capter ligne noire
}

int capteurLigne()
{
  if (capteurLigneNoire == 0)
  {
    //MOTOR_SetSpeed(RIGHT, 0.20);          // Maitre
    //MOTOR_SetSpeed(LEFT, (0.20+0.01 + PID())); // Esclave
  }
}

void algorithme(int tour)
{
  int bloc = 0; // Initialiser le point de depart

  bloc++; //On se retrouve maintenant dans le bloc 1, là où il y a la courbure
 
 
  //rotation_droite(detectionCouleur()); // L'angle de rotation sera different selon la couleur
  bloc++; //On se retrouve maintenant dans le bloc 2, là où il y a le tapis

  while ( ENCODER_Read(RIGHT)<=8420) //Ceci est la valeur théorique pour parcourir le tapis de 60cm / 2pied
  {
    AVANCER();
  }
  reset();
  bloc++; //On se retrouve au bloc 3, la cpurbure

  //rotation_droite(detectionCouleur()); // L'angle de rotation sera different selon la couleur
  bloc++; //On se situe maintenant dans le bloc 4

 
}
 

 

// Fonctions d'initialisation (setup)
// ============================

void setup()
{
  BoardInit();
  
  //pinMode(portBruitAmbiant, INPUT);
  //pinMode(portBruitEntendue, INPUT);

  //pinMode(portDetecteurProximVert, INPUT);
  //pinMode(portDetecteurProximRouge, INPUT);

  colorSensor.begin(); // Lorsque la LED du capteur de couleur est allumée, ledStatus = 1 ; Lorsque la LED du capteur de couleur est éteinte, ledStatus = 0.
  colorSensor.setGain(TCS34725_GAIN_60X); // Augmente la sensibilité du capteur de couleur (gain de 4x
  Serial.begin(9600);
}

// Fonctions de boucle infini (loop())
// ============================

void loop()
{
  rotation_droite();  
}
// Fin du programme
// ============================