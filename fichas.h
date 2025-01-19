#ifndef FICHAS_H
#define FICHAS_H

#include <QString>
#include <map>
#include <QDebug>

class Fichas {
public:
    // Constructor
    Fichas() {
        // Inicializar los soles de cada bando
        sunCont = 50;
        badSunCont = 50;

        // Definir las piezas de las plantas y sus costos
        plantPieces["sunFlower"] = 50;
        plantPieces["peaShooter"] = 150;
        plantPieces["nuez"] = 100;

        // Definir las piezas de los zombies y sus costos
        zombiePieces["graveStone"] = 50;
        zombiePieces["zombieNormal"] = 100;
        zombiePieces["coneZombie"] = 150;

        // Definir las piezas de las plantas y sus costos (noche)
        nightPlantPieces["cuteMush"] = 50; // Equivalente a sunFlower
        nightPlantPieces["puff"] = 150;    // Equivalente a peaShooter
        nightPlantPieces["nuez"] = 100;    // Sin cambio

        // Definir las piezas de los zombies y sus costos (noche)
        nightZombiePieces["graveStone"] = 50;
        nightZombiePieces["oldZombie"] = 100;   // Equivalente a zombieNormal
        nightZombiePieces["sportZombie"] = 150; // Equivalente a coneZombie

        // Definir las piezas de alberca y sus costos
        poolPlantPieces["sunFlower"] = 50; // Igual que en el día
        poolPlantPieces["peaShooter"] = 150; // Igual que en el día
        poolPlantPieces["pad"] = 100; // Nueva pieza para alberca

        poolZombiePieces["graveStone"] = 50; // Igual que en el día
        poolZombiePieces["duckieZombie"] = 100; // Zombie flotante
        poolZombiePieces["swimZombie"] = 150; // Zombie nadador
    }

    // Obtener la cantidad de soles de cada bando
    int getSunCont() const { return sunCont; }
    int getBadSunCont() const { return badSunCont; }

    // Métodos para modificar soles
    void incrementSunCont(int amount) { sunCont += amount; }
    void incrementBadSunCont(int amount) { badSunCont += amount; }

    // Obtener los mapas de costos
    const std::map<QString, int>& getPlantPieces() const { return plantPieces; }
    const std::map<QString, int>& getZombiePieces() const { return zombiePieces; }
    const std::map<QString, int>& getNightPlantPieces() const { return nightPlantPieces; }
    const std::map<QString, int>& getNightZombiePieces() const { return nightZombiePieces; }
    const std::map<QString, int>& getPoolPlantPieces() const { return poolPlantPieces; }
    const std::map<QString, int>& getPoolZombiePieces() const { return poolZombiePieces; }


    // Métodos para verificar costos
    bool canPlacePlant(const QString& plantName) const {
        auto it = plantPieces.find(plantName);
        return it != plantPieces.end() && sunCont >= it->second;
    }

    bool canPlaceZombie(const QString& zombieName) const {
        auto it = zombiePieces.find(zombieName);
        return it != zombiePieces.end() && badSunCont >= it->second;
    }

    bool canPlaceNightPlant(const QString& plantName) const {
        auto it = nightPlantPieces.find(plantName);
        return it != nightPlantPieces.end() && sunCont >= it->second;
    }

    bool canPlaceNightZombie(const QString& zombieName) const {
        auto it = nightZombiePieces.find(zombieName);
        return it != nightZombiePieces.end() && badSunCont >= it->second;
    }

    bool canPlacePoolPlant(const QString& plantName) const {
        auto it = poolPlantPieces.find(plantName);
        return it != poolPlantPieces.end() && sunCont >= it->second;
    }

    bool canPlacePoolZombie(const QString& zombieName) const {
        auto it = poolZombiePieces.find(zombieName);
        return it != poolZombiePieces.end() && badSunCont >= it->second;
    }

private:
    int sunCont; // Soles de las plantas
    int badSunCont; // Soles de los zombies

    // Mapas para las piezas y sus costos
    std::map<QString, int> plantPieces;
    std::map<QString, int> zombiePieces;

    // Mapas para las piezas y sus costos (noche)
    std::map<QString, int> nightPlantPieces;
    std::map<QString, int> nightZombiePieces;

    std::map<QString, int> poolPlantPieces;   // Piezas de alberca
    std::map<QString, int> poolZombiePieces;  // Zombies de alberca
};

#endif // FICHAS_H
