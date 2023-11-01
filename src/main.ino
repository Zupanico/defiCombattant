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

// ALGO
int tour = 1;

// PID
long millisDebut = 0;
long millisFin = 0;
double KP = 0.000072;   // Robot B : 0.00005
double KI = 0.00000092; // Robot B : 0.0000018
double erreurKI = 0;
double erreurKP = 0;
double ValPID = 0;

// PORT
const int PORT_3_CAPTEUR = 8;
const int PORT_CAPTEUR_SEUL = 5;
const int PORT_BRUIT_AMBIANT = 0;
const int PORT_BRUIT_ENTENDUE = 1;

// Fonctions

double PID() // Fonctionel
{

  millisFin = millis();

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

void rotation180() // Fonctionel
{
  int valeurTourner = 3970; // TROUVER LA VALEUR POUR FAIRE UN 180
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

void reset() // Fonctionel
{

  MOTOR_SetSpeed(RIGHT, 0.0);
  MOTOR_SetSpeed(LEFT, 0.0);
  // Delais pour décharger le CPU
  delay(200);
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
}

// Fonctions de detection

void detectionVerre() // Fonctionel
{
  Serial.println(ROBUS_ReadIR(3));
  if (ROBUS_ReadIR(2) > 475)
  {
    tomberVerre();
  }
}

int detectionCouleur() // Fonctionel
{
  uint16_t red, green, blue, clear;
  colorSensor.getRawData(&red, &green, &blue, &clear);                              // Read RGB values to variables.
  unsigned int colortemp = colorSensor.calculateColorTemperature(red, green, blue); // Convert RGB values to color temperature.
  if (colortemp < 2000)
  {
    return 1; // Rouge
  }
  else if (colortemp < 3500)
  {
    return 2; // Jaune
  }
  else if (colortemp < 5000)
  {
    return 0; // blanc
  }
  else if (colortemp < 7000)
  {
    return 3; // Vert
  }
  else
  {
    return 4; // Bleu
  }
}

int detectionLigneNoir()
{

  // float valeur = analogRead(PORT_3_CAPTEUR);
  /*
    if (<= valeur <=) //R - J - B
    {
      return 1; // 1 - 0 - 0 Gauche
    }
    else if (<= valeur <=)
    {
      return 2; // 0 - 0 - 1 Droite
    }
    else if (<= valeur <=)
    {
      return 3; // 0 - 1 - 0 Millieu
    }
    else if (<= valeur <=)
    {
      return 4; // 1 - 1 - 0 Gauche Millieu
    }
    else if (<= valeur <=)
    {
      return 5; // 1 - 0 - 1 Gauche Droite
    }
    else if (<= valeur <=)
    {
      return 6; // 0 - 1 - 1 Millieu Droite
    }
    else if (<= valeur <=)
    {
      return 7; // 1 - 1 - 1 All
    }
    else if (<= valeur <=)
    {
      return 8; //0 - 0 - 0 Nul
    }
    else
    {
      return 0; //Erreur
    }
    */
}

void attraperBalle() // Fonctionel
{
  if (ROBUS_ReadIR(1) > 620)
  {
    SERVO_Enable(1);

    SERVO_SetAngle(1, 50);

    delay(2000);

    SERVO_SetAngle(1, 170);

    delay(1000);

    SERVO_Disable(1);
  }
}

void tomberVerre()
{
  MOTOR_SetSpeed(RIGHT, 0.0);
  MOTOR_SetSpeed(LEFT, 0.0);

  SERVO_Enable(0);

  SERVO_SetAngle(0, 170);

  delay(2000);

  SERVO_SetAngle(0, 15);

  delay(1000);

  SERVO_Disable(0);
}

void tournerGauche() // Fonctionel
{
  float distanceRestante = 0;
  float MOTORSPEED = 0;
  int valeurTourner = 1985;
  while (abs(ENCODER_Read(RIGHT)) <= valeurTourner) // Boucle qui s'arrête selon la valeur du compteur car les encodeur sont reset à chaque utiliation du pid
  {
    if ((abs(ENCODER_Read(RIGHT))) <= valeurTourner * 0.60)
    {
      MOTOR_SetSpeed(RIGHT, 0.35);             // Maitre
      MOTOR_SetSpeed(LEFT, (-(0.35 + PID()))); // Esclave
    }
    else
    {
      distanceRestante = 1 - (abs(ENCODER_Read(RIGHT)) / valeurTourner);
      MOTORSPEED = (-0.625 * distanceRestante) + 0.725;
      MOTOR_SetSpeed(RIGHT, MOTORSPEED); // Maitre
      MOTOR_SetSpeed(LEFT, -(MOTORSPEED + PID()));
    }
  }
  reset();
}

void tournerDroite() // Fonctionel
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

void reculer() // Fonctionel
{
  // Si le robot est sur une couleur, il va arreter d'avancer quand il n'en verra plus(Blanc ou noir)
  MOTOR_SetSpeed(RIGHT, -0.15);                 // Maitre
  MOTOR_SetSpeed(LEFT, (-0.15 - 0.01 - PID())); // Esclave
  delay(500);
  while (detectionCouleur() != 0)
  {
    detectionVerre();
    MOTOR_SetSpeed(RIGHT, -0.30);                 // Maitre
    MOTOR_SetSpeed(LEFT, (-0.30 - 0.01 - PID())); // Esclave
  }
  reset();
}

// Fonctions Avancer

void avancerDistance(int distance) // Fonctionel
{
  int valeurAvancer = distance * 134.8; // VALEUR A TROUVER POUR AVANCER JUSQU'A LIGNE NOIRE
  delay(110);
  while ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer)
  {
    if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.80)
    {
      MOTOR_SetSpeed(RIGHT, 0.35);                 // Maitre
      MOTOR_SetSpeed(LEFT, (0.35 + 0.01 + PID())); // Esclave
    }
    else
    {
      MOTOR_SetSpeed(RIGHT, 0.15); // Maitre
      MOTOR_SetSpeed(LEFT, (0.15 + 0.01 + PID()));
    }
  }
  reset();
}

void avancerLigneNoire()
{
  MOTOR_SetSpeed(RIGHT, 0.20);                 // Maitre
  MOTOR_SetSpeed(LEFT, (0.20 + 0.01 + PID())); // Esclave

  while (detectionLigneNoir() == 8)
  {
    MOTOR_SetSpeed(RIGHT, 0.30);                 // Maitre
    MOTOR_SetSpeed(LEFT, (0.30 + 0.01 + PID())); // Esclave
  }

  reset();
}

void avancerCouleur()
{
  // Si le robot est sur une couleur, il va arreter d'avancer quand il n'en verra plus(Blanc ou noir)
  MOTOR_SetSpeed(RIGHT, 0.15);          // Maitre
  MOTOR_SetSpeed(LEFT, (0.15 + PID())); // Esclave
  delay(500);
  while (detectionCouleur() != 0)
  {
    detectionVerre();
    MOTOR_SetSpeed(RIGHT, 0.30);          // Maitre
    MOTOR_SetSpeed(LEFT, (0.30 + PID())); // Esclave
    if (detectionCouleur() == 1)
    {
      MOTOR_SetSpeed(RIGHT, 0.30);  // Maitre
      MOTOR_SetSpeed(LEFT, (0.35)); // Esclave
      while (detectionCouleur() == 1)
      {
      }
    }
    if (detectionCouleur() == 4)
    {
      MOTOR_SetSpeed(RIGHT, 0.35); // Maitre
      MOTOR_SetSpeed(LEFT, 0.30);  // Esclave
      while (detectionCouleur() == 4)
      {
      }
    }
  }
  reset();
}

// Fonction Ligne noire

void realignement()
{
  while (detectionLigneNoir() != 8)
  {
    if (detectionLigneNoir() == (2 || 6 || 7))
    {
      MOTOR_SetSpeed(RIGHT, 0.2);
    }
    else
    {
      MOTOR_SetSpeed(RIGHT, 0.0);
    }
    if (detectionLigneNoir() == (1 || 4 || 7))
    {
      MOTOR_SetSpeed(LEFT, 0.2);
    }
    else
    {
      MOTOR_SetSpeed(LEFT, 0.0);
    }
    reset();
  }
}

void suivreLigneNoire()
{
  // Lire la valeur des trois capteurs

  float LectureMilieu = analogRead(PORT_3_CAPTEUR);
  float LectureDroite = digitalRead(PORT_CAPTEUR_SEUL);
  while (detectionCouleur() == 0)
  {

    if ((1.43 < LectureMilieu < 2) && (LectureDroite = 0))
    { // Rajouter les autre possibilité ou le milleu  détecte  la ligne du millieu

      MOTOR_SetSpeed(RIGHT, 0.37);
      delay(5);
    }
    else
    {

      MOTOR_SetSpeed(RIGHT, 0.35);
    }
    if (LectureDroite == 1 && ((1.43 < LectureMilieu < 2)))
    { // Rajouter les autre possibilité ou le milleu ne détecte pas la ligne du millieu

      MOTOR_SetSpeed(LEFT, 0.37);
      delay(5);
    }
    else
    {

      MOTOR_SetSpeed(LEFT, 0.35);
    }
  }
}

// Fonction Algorithme

void algorithme()
{
  int couleurMemoire = detectionCouleur();
  // avancerLigneNoire();
  avancerDistance(30);
  tournerDroite();

  if (couleurMemoire == 2)
  {
    avancerDistance(213);
    tournerDroite();
    avancerCouleur();
    avancerDistance(30);
    tournerDroite();
    avancerDistance(30);
    avancerLigneNoire();
    suivreLigneNoire();
  }
  else if (couleurMemoire == 3)
  {
    avancerDistance(142);
    tournerGauche();
    reculer();
    rotation180();
    avancerLigneNoire();
    suivreLigneNoire();
  }
}

void setup()
{
  BoardInit();

  pinMode(PORT_BRUIT_AMBIANT, INPUT);
  pinMode(PORT_BRUIT_ENTENDUE, INPUT);

  pinMode(PORT_3_CAPTEUR, INPUT);
  pinMode(PORT_CAPTEUR_SEUL, INPUT);

  colorSensor.begin();                    // Lorsque la LED du capteur de couleur est allumée, ledStatus = 1 ; Lorsque la LED du capteur de couleur est éteinte, ledStatus = 0.
  colorSensor.setGain(TCS34725_GAIN_60X); // Augmente la sensibilité du capteur de couleur (gain de 4x
  Serial.begin(9600);
  SERVO_Disable(0);
  while (!ROBUS_IsBumper(REAR))
  {
  }
  algorithme();
}

void loop()
{
}

/*void ALLER_LIGNER_NOIRE()


{
  AVANCER_HAUTEUR_LIGNE_NOIRE(); //Avancer jusqu'a hauteur ligne noire
  tournerDroite(); //Tourne droite de 90
  avancerLigneNoire(); //Avance jusqu'a capter ligne noire
}*/

/*void AVANCER_HAUTEUR_LIGNE_NOIRE()
{
  int valeurAvancer = 6682; // VALEUR A TROUVER POUR AVANCER JUSQU'A LIGNE NOIRE

  while ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer)
  {
    if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.40)
    {
      MOTOR_SetSpeed(RIGHT, 0.30);                 // Maitre
      MOTOR_SetSpeed(LEFT, (0.30 + 0.01 + PID())); // Esclave
    }
    else if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.70)
    {
      MOTOR_SetSpeed(RIGHT, 0.40);                 // Maitre
      MOTOR_SetSpeed(LEFT, (0.40 + 0.01 + PID())); // Esclave
    }
    else if ((abs(ENCODER_Read(RIGHT))) <= valeurAvancer * 0.90)
    {
      MOTOR_SetSpeed(RIGHT, 0.20); // Maitre
      MOTOR_SetSpeed(LEFT, (0.20 + 0.01 + PID()));
    }
    else
    {
      MOTOR_SetSpeed(RIGHT, 0.1); // Maitre
      MOTOR_SetSpeed(LEFT, (0.1 + 0.01 + PID()));
    }
  }
  reset();
}
*/