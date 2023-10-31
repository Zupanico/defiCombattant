/*****************************************************************************
Projet: Defit Du Combattant
Equipe: P6
Auteurs: Bakayoko Kanvali
Description: Programmation Generale du Fonctionnnement du Robot
Date: 22 Octorbre 2023
******************************************************************************/

/* ****************************************************************************
          Inclure les librairies de functions que vous voulez utiliserS
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS

#include <Adafruit_TCS34725.h>

double correctionSuiveurDeligne = 0;
double correction = 0;
int port3Capteur = 40;
int portCapteurSeul = 5;
// Fonctions
// ============================

double erreurKP = 0;
double erreurKI = 0;

long millisDebut = 0;
long millisFin = 0;

double KP = 0.000072;   // Robot B : 0.00005
double KI = 0.00000092; // Robot B : 0.0000018
double ratio = 1;

double PID()
{

    millisFin = millis();

    if ((millisFin - millisDebut) >= 50.0)
    {

        int valeurEncodeurDroit = abs(ENCODER_Read(RIGHT)) * ratio;
        int valeurEncodeurGauche = abs(ENCODER_Read(LEFT));

        erreurKP = valeurEncodeurDroit - valeurEncodeurGauche;
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

int detectionLigneNoir()
{

    float valeur = analogRead(port3Capteur);
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
        return 8; //0 - 0 - 0 Nil
      }
      else
      {
        return 0; //Erreur
      }
      */
}

void suiveurDeLigne()
{
    // Lire la valeur des trois capteurs

    float LectureMilieu = analogRead(port3Capteur);
    float LectureDroite = digitalRead(portCapteurSeul);

    if ((1.43 < LectureMilieu < 2) && (LectureDroite = 0))
    { // Rajouter les autre possibilité ou le milleu  détecte  la ligne du millieu

        MOTOR_SetSpeed(RIGHT, 0.4);
        delay(5);
    }
    else if (LectureDroite == 1 && ((1.43 < LectureMilieu < 2)))
    { // Rajouter les autre possibilité ou le milleu ne détecte pas la ligne du millieu

        MOTOR_SetSpeed(LEFT, 0.4);
        delay(5);
    }
}
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


Adafruit_TCS34725 colorsensor;

int detectionCouleur()
{

    float rouge, vert, bleu, clear;
    colorsensor.getRGB(&rouge, &vert, &bleu);
    Serial.println("Rouge ");
    Serial.println(rouge);
    Serial.println("Vert ");
    Serial.println(vert);
    Serial.println("Bleu ");
    Serial.println(bleu);
    colorsensor.clearInterrupt();

    pinMode(port3Capteur, INPUT);
    pinMode(portCapteurSeul, INPUT);

    Serial.begin(9600);
    if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) // Si Rouge
    {
        return 1;
    }
    else if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) // Si Jaune
    {
        return 2;
    }
    else if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) // Si Vert
    {
        return 3;
    }
    else if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) // Si Bleu
    {
        return 4;
    }
    else // Si Autre Chose
    {
        return 0;
    }
}

void avancer(int distance)
{

    while (ENCODER_Read(RIGHT) <= distance)
    { // Avancer jusqu'au bleu + extra
    }
}

void tourner90()
{
}
void tournerArc(){};

void shortCut()
{
    tourner90();
    avancer(1);
    tourner90();
    tournerArc();
    avancer(1); // Tapis
    tournerArc();
    avancer(1); // Jusqu'à le trou, détection capteur de distance
    tourner90();
    avancer(1); // Jusqu'à vert
    tourner90();
    avancer(1);
}

void reset()
{

    MOTOR_SetSpeed(RIGHT, 0.0);
    MOTOR_SetSpeed(LEFT, 0.0);
    // Delais pour décharger le CPU
    delay(10);
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
}

void setup()
{
    BoardInit();
    Serial.begin(9600);
    Serial.println("t");
    colorsensor.begin();
}

// Fonctions de boucle infini (loop())
// ============================

void loop()
{

    Serial.println(analogRead(port3Capteur));
    delay(1500);

    detectionCouleur();

    delay(1000);
}