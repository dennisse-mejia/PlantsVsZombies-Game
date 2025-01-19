#ifndef NOCHE_H
#define NOCHE_H

#include <QMainWindow>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QPushButton>
#include <QListWidgetItem>
#include "fichas.h"
#include "form.h"


namespace Ui {
class noche;
}

class noche : public QWidget
{
    Q_OBJECT

public:
    explicit noche(Form *formInstance,QString currentFaction, QString opponentFaction, QString currentUser, QString opponentName, QWidget *parent = nullptr);
    ~noche();

    QString getCurrentFaction() const { return currentFaction; }
    QString getOpponentFaction() const { return opponentFaction; }
    QString getCurrentUser() const { return currentUser; }
    QString getOpponentName() const { return opponentName; }


signals:
    void gameEnded(const QString &winner);
    void proceedToAlberca(); // Señal para abrir alberca.ui


private slots:

    void handleGridButtonClick();
    void selectCuteMush();
    void selectPuff();
    void selectNuez();

    void selectGraveStone();
    void selectOldZombie();
    void selectSportZombie();

    void handlePuffShooting();
    void moveZombies();
    void updateCountdown();
    void checkWinner();
    void stopGame(const QString &winner);


    void on_mainMenu_clicked();
    // void performAIMove();
    // void performPlantsAIMove();
    // void performZombiesAIMove();

private:
    Ui::noche *ui;

    // Datos del jugador
    QString currentFaction;  // Facción del jugador actual
    QString opponentFaction; // Facción del oponente
    QString currentUser;     // Nombre del usuario actual
    QString opponentName;    // Nombre del oponente actual

    QString currentPlayer;   // Jugador actual ("currentUser" o "opponent")
    QString selectedPiece;   // Pieza seleccionada para colocar

    Fichas fichasLogic;      // Lógica de las piezas

    // Temporizadores
    QTimer *shootingTimer;         // Temporizador para disparos
    QTimer *zombieMovementTimer;   // Temporizador para movimiento de zombies
    QTimer *gameCountdownTimer;    // Temporizador para cuenta regresiva
    int remainingTime;             // Tiempo restante en segundos

    // Posiciones y estado
    QVector<QPair<int, int>> puffPositions;       // Posiciones de los Puff (equivalente a PeaShooters)
    QVector<QPair<int, int>> zombiePositions;    // Posiciones de los zombies
    QMap<QPair<int, int>, QString> zombieTypeMap; // Tipo de zombie por posición
    QMap<QPair<int, int>, int> zombieImpactCount; // Contador de impactos por zombie
    QMap<QPair<int, int>, QTimer*> cuteMushTimers; // Temporizadores para los CuteMush
    QMap<QPair<int, int>, QTimer*> graveStoneTimers; // Temporizadores para los GraveStones

    // Métodos auxiliares
    void removeCuteMush(int row, int col);
    void removeGraveStone(int row, int col);
    void handleCuteMushDestruction(int row, int col);
    bool gameInProgress = true;
    Form *formInstance;
    QPushButton *goodSunPic;
    QPushButton *badSunPic;
};

#endif // NOCHE_H
