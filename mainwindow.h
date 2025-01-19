#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>
#include <QLabel>
#include "fichas.h"
#include <QVector> // For storing PeaShooter positions
#include <QStringList>
#include <QListWidgetItem>
#include "form.h"





QT_BEGIN_NAMESPACE
namespace Ui {
class timerLabel;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Form *formInstance, QStringList users, QString currentUser, QWidget *parent = nullptr);
    ~MainWindow();

    // Getters para las variables privadas
    QString getCurrentFaction() const { return currentFaction; }
    QString getOpponentFaction() const { return opponentFaction; }
    QString getCurrentUser() const { return currentUser; }
    QString getOpponentName() const { return opponentName; }
    bool isGameInProgress() const { return gameInProgress; }




private slots:

    void on_oponentOption_clicked();

    void on_AIOption_clicked();

    void handleGridButtonClick();
    void selectSunFlower();
    void selectNuez();
    void selectPeaShooter();

    void selectGraveStone();
    void selectConeZombie();
    void selectNormalZombie();
    void handlePeaShooterShooting();
    void moveZombies(); // New slot for moving zombies
    void onOpponentSelected(QListWidgetItem *item);
    void updateCountdown();
    //void gameEnded(const QString &winner);

    void on_mainMenu_clicked();
    void performAIMove();
    void performPlantsAIMove();
    void performZombiesAIMove();



signals:
    void gameEnded(const QString &winner);
    void proceedToAlberca(); // Señal para indicar que se debe abrir Alberca
    void proceedToNoche(); // Señal para abrir noche.ui




private:

    Ui::timerLabel *ui;
    Form *formInstance; // Referencia a la instancia de Form

    QStringList registeredUsers;
    QString currentUser;
    QPushButton *selectedButton = nullptr; // Botón seleccionado en el tablero
    QString selectedPiece; // Variable para almacenar la pieza seleccionada

    Fichas fichasLogic; // Instancia de la clase Fichas
    QTimer *shootingTimer; // Add the shooting timer
    QTimer *zombieMovementTimer; // Timer for handling zombie movement

    // New members for the countdown timer
    QTimer *gameCountdownTimer; // Timer for the 2-minute countdown
    int remainingTime;          // Remaining time in seconds for the countdown


    QVector<QPair<int, int>> peaShooterPositions; // Store PeaShooter positions
    QVector<QPair<int, int>> zombiePositions; // Store Zombie positions


    void verificarDisparosPeaShootersEnFila(QPushButton *zombieButton);
    void mostrarDisparo(int fila, int col);

    QMap<QPair<int, int>, QString> zombieTypeMap; // Mapa que almacena el tipo de zombie por posición (fila, columna)
   // bool isPeaShooterIcon(const QIcon &icon);
    QMap<QPair<int, int>, int> zombieImpactCount;
    QMap<QPair<int, int>, bool> peaShooterActiveStatus; // Mapa para rastrear el estado de los PeaShooters
    QVector<QPair<int, int>> removedPeaShooterPositions; // Almacena posiciones de PeaShooters eliminados
    QMap<QPair<int, int>, QTimer*> sunFlowerTimers; // Mapa de temporizadores para cada SunFlower
    // Declaraciones de los métodos que causan el error
    void removeSunFlower(int row, int col);
    void handleSunFlowerDestruction(int row, int col);

    QMap<QPair<int, int>, QTimer*> graveStoneTimers; // Mapa para temporizadores de graveStone
    void removeGraveStone(int row, int col);         // Método para eliminar un graveStone

    QString currentFaction;        // Stores the current user's selected faction ("Plants" or "Zombies")
    QString opponentFaction;       // Stores the opponent's faction
    QString currentPlayer;         // Tracks whose turn it is ("currentUser" or "opponent")
    QString opponentName; // To store the selected opponent's name

    bool gameInProgress = true; // To track if the game is still ongoing
    void checkWinner();         // Method to check for a winner
    void stopGame(const QString &winner); // Method to stop the game and declare the winner
    QPushButton *goodSunPic;
    QPushButton *badSunPic;



};
#endif // MAINWINDOW_H
