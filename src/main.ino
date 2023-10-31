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

// Variables globales et defines
// ============================

// Fonctions
// ============================

// Fonctions d'initialisation (setup)
// ============================

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
    
detectionCouleur();

delay(1000);

}
// Fin du programme
// ============================