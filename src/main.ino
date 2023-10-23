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
#include <GroveColorSensor.h>
// Variables globales et defines
// ============================

// Fonctions
// ============================

// Fonctions d'initialisation (setup)
// ============================

GroveColorSensor colorSensor;

int detectionCouleur()
{

    colorSensor.ledStatus = 1; // Débute le color sensor
    int rouge, vert, bleu;
    colorSensor.readRGB(&rouge, &vert, &bleu); // Lit les valeur du color sensor

    Serial.println("Rouge ");
    Serial.print(rouge);
    Serial.println("Vert ");
    Serial.print(vert);
    Serial.println("Bleu ");
    Serial.print(bleu);

    if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) //Si Rouge
    {
        return 1;
    }
    else if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) //Si Jaune
    {
        return 2;
    }
    else if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) //Si Vert
    {
        return 3;
    }
    else if (0 > rouge > 0 && 0 > vert > 0 && 0 > bleu > 0) //Si Bleu
    {
        return 4;
    }
    else //Si Autre Chose
    {
        return 0;
    }
}

void setup()
{
    Serial.begin(115200);
}

// Fonctions de boucle infini (loop())
// ============================

void loop()
{
}
// Fin du programme
// ============================