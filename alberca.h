#ifndef ALBERCA_H
#define ALBERCA_H

#include <QWidget>
#include <QMainWindow>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QPushButton>
#include <QListWidgetItem>
#include "fichas.h"
#include "form.h"



namespace Ui {
class alberca;
}

class alberca : public QWidget
{
    Q_OBJECT

public:
    explicit alberca(Form *form, QString currentFaction, QString opponentFaction, QString currentUser, QString opponentName, QWidget *parent);
    ~alberca();

    QString getCurrentFaction() const { return currentFaction; }
    QString getOpponentFaction() const { return opponentFaction; }
    QString getCurrentUser() const { return currentUser; }
    QString getOpponentName() const { return opponentName; }

signals:
    void gameEnded(const QString &winner);


private slots:
    void handleGridButtonClick();              // Manejar clics en la cuadrícula
    void selectPad();                           // Seleccionar Pad
    void selectPeaShooter();                   // Seleccionar PeaShooter
    void selectSunFlower();                    // Seleccionar SunFlower

    void selectGraveStone();                   // Seleccionar GraveStone
    void selectSwimZombie();                   // Seleccionar SwimZombie
    void selectDuckieZombie();                 // Seleccionar DuckieZombie

    void handlePeaShooterShooting();           // Manejar los disparos de PeaShooters
    void moveZombies();                        // Mover los zombies
    void updateCountdown();                    // Actualizar cuenta regresiva
    void checkWinner();                        // Verificar ganador
    void stopGame(const QString &winner);      // Detener el juego y declarar ganador

    void on_mainMenu_clicked();

private:
    Ui::alberca *ui;

    // Datos del jugador
    QString currentFaction;                    // Facción del jugador actual
    QString opponentFaction;                   // Facción del oponente
    QString currentUser;                       // Nombre del usuario actual
    QString opponentName;                      // Nombre del oponente actual

    QString currentPlayer;                     // Jugador actual ("currentUser" o "opponent")
    QString selectedPiece;                     // Pieza seleccionada para colocar

    Fichas fichasLogic;                        // Lógica de las piezas

    // Temporizadores
    QTimer *shootingTimer;                     // Temporizador para disparos
    QTimer *zombieMovementTimer;               // Temporizador para movimiento de zombies
    QTimer *gameCountdownTimer;                // Temporizador para cuenta regresiva
    int remainingTime;                         // Tiempo restante en segundos

    // Posiciones y estado
    QVector<QPair<int, int>> peaShooterPositions; // Posiciones de los PeaShooters
    QVector<QPair<int, int>> zombiePositions;     // Posiciones de los zombies
    QMap<QPair<int, int>, QString> zombieTypeMap; // Tipo de zombie por posición
    QMap<QPair<int, int>, int> zombieImpactCount; // Contador de impactos por zombie
    QMap<QPair<int, int>, QTimer*> sunFlowerTimers; // Temporizadores para los SunFlowers
    QMap<QPair<int, int>, QTimer*> graveStoneTimers; // Temporizadores para los GraveStones

    // Métodos auxiliares
    void removeSunFlower(int row, int col);    // Eliminar SunFlower
    void removeGraveStone(int row, int col);   // Eliminar GraveStone
    void handleSunFlowerDestruction(int row, int col); // Manejar destrucción de SunFlower
    bool gameInProgress = true;               // Estado del juego
    Form *formInstance; // Puntero a la instancia de Form
    QPushButton *goodSunPic;
    QPushButton *badSunPic;

};

#endif // ALBERCA_H
