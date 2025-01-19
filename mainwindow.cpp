#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "globals.h"

#include <QDebug>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QTimer>
#include <QMap>



MainWindow::MainWindow(Form *form, QStringList users, QString currentUser, QWidget *parent)
    : QMainWindow(parent), formInstance(form)
    , ui(new Ui::timerLabel)
    , registeredUsers(users)
    , currentUser(currentUser)
    , selectedPiece("")
    , shootingTimer(new QTimer(this)) // Initialize the timer
    , zombieMovementTimer(new QTimer(this))
    , gameCountdownTimer(new QTimer(this)) // Add the timer
    , remainingTime(180) // 3 minutes in seconds


{
    setObjectName("MainWindow"); // Configura un nombre para identificar esta ventana

    ui->setupUi(this);
    ui->opponentsListWidget->hide(); // Ocultar la lista al inicio
    ui->sunCont->setStyleSheet(
        "QLineEdit {"
        "    background-color: transparent;" // Fondo transparente
        "    border: none;"                  // Sin borde
        "    color: #2f8112;"                // Color del texto (puedes cambiarlo si deseas)

        "}"
        );
    ui->badSunCont->setStyleSheet(
        "QLineEdit {"
        "    background-color: transparent;" // Fondo transparente
        "    border: none;"                  // Sin borde
        "    color: #2f8112;"                // Color del texto (puedes cambiarlo si deseas)

        "}"
        );

    ui->mainMenu->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        );


    ui->oponentOption->setStyleSheet("QPushButton {"
                                    "background-color: transparent;" // Eliminar el fondo
                                    "border: none;"                  // Eliminar el borde
                                    "}");

    ui->AIOption->setStyleSheet("QPushButton {"
                                         "background-color: transparent;" // Eliminar el fondo
                                         "border: none;"                  // Eliminar el borde
                                         "}");

    ui->opponentsListWidget->setStyleSheet(
        "QListWidget {"
        "    background-color: #79300d;" // Color de fondo (gris oscuro)
        "    color: #388012;"           // Color del texto (blanco)
        "}"
        );

    // Configurar estilo para los botones de PLANTAS
    ui->sunFlowerBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;" // Fondo transparente
        "    border: none;"                  // Sin borde
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo amarillo con transparencia al presionar
        "    border-radius: 5px;"                      // Borde redondeado (opcional)
        "    border: 1px solid #FFD700;"               // Borde dorado para destacar
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo blanco tenue al pasar el ratón
        "    border-radius: 5px;"                        // Igual que el estilo presionado
        "}"
        );
    ui->nuezBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo amarillo con transparencia al presionar
        "    border-radius: 5px;"                      // Borde redondeado (opcional)
        "    border: 1px solid #FFD700;"               // Borde dorado para destacar
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo blanco tenue al pasar el ratón
        "    border-radius: 5px;"                        // Igual que el estilo presionado
        "}"
        );
    ui->peaShooterBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo amarillo con transparencia al presionar
        "    border-radius: 5px;"                      // Borde redondeado (opcional)
        "    border: 1px solid #FFD700;"               // Borde dorado para destacar
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo blanco tenue al pasar el ratón
        "    border-radius: 5px;"                        // Igual que el estilo presionado
        "}"
        );

    // Configurar estilo para los botones de ZOMBIES
    ui->graveStoneBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo amarillo con transparencia al presionar
        "    border-radius: 5px;"                      // Borde redondeado
        "    border: 1px solid #FFD700;"               // Borde dorado para destacar
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo blanco tenue al pasar el ratón
        "    border-radius: 5px;"                        // Igual que el estilo presionado
        "}"
        );
    ui->coneZBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "    color: #FFFFFF;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo amarillo con transparencia al presionar
        "    border-radius: 5px;"                      // Borde redondeado
        "    border: 1px solid #FFD700;"               // Borde dorado para destacar
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo blanco tenue al pasar el ratón
        "    border-radius: 5px;"                        // Igual que el estilo presionado
        "}"
        );
    ui->normalZBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo amarillo con transparencia al presionar
        "    border-radius: 5px;"                      // Borde redondeado
        "    border: 1px solid #FFD700;"               // Borde dorado para destacar
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);" // Fondo blanco tenue al pasar el ratón
        "    border-radius: 5px;"                        // Igual que el estilo presionado
        "}"
        );



    // Inicializa goodSunPic
    QPushButton *goodSunPic = ui->goodSunPic;
    // Configuración opcional del estilo
    if (goodSunPic) {
        goodSunPic->setStyleSheet("border: none; padding: 0; background-color: transparent;");
    }

    QPushButton *badSunPic = ui->badSunPic;
    // Configuración opcional del estilo
    if (badSunPic) {
        badSunPic->setStyleSheet("border: none; padding: 0; background-color: transparent;");
    }

    // Initialize the current player label
    ui->currentPlayerName->setText("Turno actual: " + currentUser);
    ui->currentPlayerName->setStyleSheet("font-size: 16px; font-weight: bold;");


    // Connect the countdown timer to its slot
    connect(gameCountdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdown);

    // Connect the signal for item clicks in the opponents list
    connect(ui->opponentsListWidget, &QListWidget::itemClicked, this, &MainWindow::onOpponentSelected);


    // Debugging: Confirm registeredUsers and currentUser are received
    qDebug() << "MainWindow initialized with registeredUsers:" << registeredUsers;
    qDebug() << "Current user:" << currentUser;

    // Configura un temporizador para imprimir las posiciones de los PeaShooters eliminados
    QTimer *debugTimer = new QTimer(this);
    connect(debugTimer, &QTimer::timeout, this, [=]() {
        qDebug() << "Removed PeaShooter Positions:";
        for (const auto &pos : removedPeaShooterPositions) {
            qDebug() << "PeaShooter at (" << pos.first << ", " << pos.second << ")";
        }
    });
    debugTimer->start(1000);



    // Inicializar el contador de soles
    ui->sunCont->setText(QString::number(fichasLogic.getSunCont()));
    ui->badSunCont->setText(QString::number(fichasLogic.getBadSunCont()));

    //ui->badSunCont->setText(QString::number(50));


    // Lista de botones específicos para colocar la imagen "motoSierra"
    QList<QPushButton*> motoSButtons = {
        ui->motoS1, ui->motoS2, ui->motoS3, ui->motoS4, ui->motoS5
    };

    // Asignar la imagen "motoSierra" a cada botón con el tamaño 82x92
    QIcon motoSierraIcon(":/Resources/motoSierra.png");
    for (QPushButton* button : motoSButtons) {
        if (button) {
            button->setIcon(motoSierraIcon);
            button->setIconSize(QSize(82, 92)); // Tamaño exacto de 82x92
            button->setStyleSheet("border: none; padding: 0;"); // Asegura que la imagen ocupe todo el espacio
            button->setMinimumSize(82, 92); // Ajusta el tamaño mínimo del botón
            button->setMaximumSize(82, 92); // Ajusta el tamaño máximo del botón
        }
    }

    // Conectar botones PLANTAS a sus ranuras
    connect(ui->sunFlowerBtn, &QPushButton::clicked, this, &MainWindow::selectSunFlower);
    connect(ui->nuezBtn, &QPushButton::clicked, this, &MainWindow::selectNuez);
    connect(ui->peaShooterBtn, &QPushButton::clicked, this, &MainWindow::selectPeaShooter);

    // Conectar botones ZOMBIES a sus ranuras
    connect(ui->graveStoneBtn, &QPushButton::clicked, this, &MainWindow::selectGraveStone);
    connect(ui->coneZBtn, &QPushButton::clicked, this, &MainWindow::selectConeZombie);
    connect(ui->normalZBtn, &QPushButton::clicked, this, &MainWindow::selectNormalZombie);


    // Conectar cada botón de la cuadrícula a la ranura de selección
    for (int i = 0; i < ui->gridLayout->count(); ++i) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
            connect(button, &QPushButton::clicked, this, &MainWindow::handleGridButtonClick);

            // Ajustar el tamaño de los botones para que sean más proporcionales
            button->setMinimumSize(82, 92); // Tamaño mínimo más proporcionado
            //button->setMaximumSize(92, 176); // Tamaño máximo para limitar la expansión
            button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            // Aplicar estilo para hacerlos transparentes
            button->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;" // Fondo transparente
                "    border: none;"                  // Sin bordes
                "}"
                );
        }
    }

    // Set up shooting timer
    connect(shootingTimer, &QTimer::timeout, this, &MainWindow::handlePeaShooterShooting);

    // Configuración del temporizador de movimiento de zombies
    connect(zombieMovementTimer, &QTimer::timeout, this, &MainWindow::moveZombies);
    zombieMovementTimer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveZombies()
{
    for (int i = 0; i < zombiePositions.size(); ++i) {
        int row = zombiePositions[i].first;
        int col = zombiePositions[i].second;

        // Verifica si el zombie puede moverse a la izquierda
        if (col > 0) {
            QPushButton *currentButton = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(row, col)->widget());
            QPushButton *nextButton = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(row, col - 1)->widget());

            if (currentButton && nextButton) {

                if (nextButton->objectName().startsWith("motoS")) {
                    // If the motoSierra button is empty (no icon), move the zombie onto it
                    if (nextButton->icon().isNull()) {
                        QIcon icon = currentButton->icon();
                        currentButton->setIcon(QIcon());
                        nextButton->setIcon(icon);
                        nextButton->setIconSize(QSize(nextButton->width() * 0.9, nextButton->height() * 0.9));
                        nextButton->setStyleSheet("border: none; padding: 0;");

                        // Update zombie position
                        zombiePositions[i].second -= 1;

                        // Stop the game and declare victory for zombies
                        gameCountdownTimer->stop(); // Stop the countdown timer
                        stopGame("Zombies"); // Declare zombies as winners
                        return;
                    }

                    // If the motoSierra button is occupied, destroy both the zombie and the motoSierra
                    currentButton->setIcon(QIcon());
                    nextButton->setIcon(QIcon());

                    // Remove the zombie position from the list
                    zombiePositions.removeAt(i);
                    --i; // Adjust the index after removal

                    qDebug() << "Zombie at (" << row << ", " << col << ") was destroyed by motoSierra.";
                    // Check if plants win immediately after the zombie is destroyed
                    if (fichasLogic.getBadSunCont() <= 0 && zombiePositions.isEmpty() && graveStoneTimers.isEmpty()) {
                        gameCountdownTimer->stop(); // Stop the countdown timer
                        stopGame("Plants"); // Declare plants as winners
                        return; // Exit the function
                    }

                    continue; // Skip further processing for this zombie
                }



                // Verifica si hay un PeaShooter en la posición a la que el zombie se va a mover
                auto it = std::find(peaShooterPositions.begin(), peaShooterPositions.end(), QPair<int, int>(row, col - 1));
                if (it != peaShooterPositions.end()) {
                    // Elimina el ícono del PeaShooter y remuévelo de la lista
                    nextButton->setIcon(QIcon());
                    peaShooterPositions.erase(it);
                    qDebug() << "PeaShooter at (" << row << ", " << col - 1 << ") has been removed by a zombie.";
                }

                // Verifica si hay un SunFlower en la posición a la que el zombie se va a mover
                QPair<int, int> position(row, col - 1);
                if (sunFlowerTimers.contains(position)) {
                    // Elimina el SunFlower (icono y temporizador)
                    nextButton->setIcon(QIcon());
                    removeSunFlower(row, col - 1); // Detener el temporizador asociado
                    qDebug() << "SunFlower at (" << row << ", " << col - 1 << ") has been destroyed by a zombie.";
                }
                // Mueve el zombie a la nueva posición
                QIcon icon = currentButton->icon();
                currentButton->setIcon(QIcon());
                nextButton->setIcon(icon);
                nextButton->setIconSize(QSize(nextButton->width() * 0.9, nextButton->height() * 0.9));
                nextButton->setStyleSheet("border: none; padding: 0;");

                // Actualiza el mapa con el nuevo tipo de zombie
                QPair<int, int> oldPosition(row, col);
                QPair<int, int> newPosition(row, col - 1);
                zombieTypeMap[newPosition] = zombieTypeMap[oldPosition];
                zombieTypeMap.remove(oldPosition);

                // Mueve el contador de impactos si es necesario
                if (zombieImpactCount.contains(oldPosition)) {
                    zombieImpactCount[newPosition] = zombieImpactCount[oldPosition];
                    zombieImpactCount.remove(oldPosition);
                }

                // Actualiza la posición en el QVector
                zombiePositions[i].second -= 1;


            }
        }
    }
}



void MainWindow::handleGridButtonClick()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    // if (button && !selectedPiece.isEmpty()) {
    //     // Verifica si el botón ya tiene un ícono asignado
    //     if (!button->icon().isNull()) {
    //         return; // Sale de la función sin realizar ningún cambio
    //     }

    if (!button || selectedPiece.isEmpty()) {
        return; // No button or piece selected
    }

    // Check turn logic
    if (currentPlayer == "currentUser") {
        // Ensure the current user can only place pieces from their faction
        if ((currentFaction == "Plants" && (selectedPiece == "zombieNormal" || selectedPiece == "coneZombie" || selectedPiece == "graveStone")) ||
            (currentFaction == "Zombies" && (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "nuez"))) {
            QMessageBox::warning(this, "Invalid Move", "You can only place pieces from your faction.");
            return;
        }
    } else if (currentPlayer == "opponent") {
        // Ensure the opponent can only place pieces from their faction
        if ((opponentFaction == "Plants" && (selectedPiece == "zombieNormal" || selectedPiece == "coneZombie" || selectedPiece == "graveStone")) ||
            (opponentFaction == "Zombies" && (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "nuez"))) {
            QMessageBox::warning(this, "Invalid Move", "The opponent can only place pieces from their faction.");
            return;
        }
    } else {
        QMessageBox::warning(this, "Invalid Turn", "It's not your turn!");
        return;
    }

    // Check if the button already has an icon
    if (!button->icon().isNull()) {
        return; // Exit the function without making any changes
    }

        // Listas de botones permitidos
        QList<QPushButton*> allowedGraveStoneButtons = {
            ui->pushButton_10, ui->pushButton_28, ui->pushButton_19,
            ui->pushButton_37, ui->pushButton_46
        };
        QList<QPushButton*> allowedZombieButtons = {
            ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
            ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
            ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
        };

        // Verifica si la pieza seleccionada es graveStone y si el botón es permitido
        if (selectedPiece == "graveStone" && !allowedGraveStoneButtons.contains(button)) {
            return; // Sale de la función si el botón no está permitido para graveStone
        }

        // Verifica si la pieza seleccionada es zombieNormal o coneZombie y si el botón es permitido
        if ((selectedPiece == "zombieNormal" || selectedPiece == "coneZombie") && !allowedZombieButtons.contains(button)) {
            return; // Sale de la función si el botón no está permitido para estos zombies
        }
        QIcon icon;

        // Validar y manejar plantas
        if (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "nuez") {
            // Validar costos usando getPlantPieces()
            auto plantCosts = fichasLogic.getPlantPieces();
            if (plantCosts.find(selectedPiece) == plantCosts.end()) {
                qDebug() << "Invalid plant selected: " << selectedPiece;
                return; // No hacer nada si la planta no existe
            }

            int plantCost = plantCosts.at(selectedPiece);

            // Verificar si hay suficientes soles
            if (fichasLogic.getSunCont() < plantCost) {
                qDebug() << "Not enough suns to place " << selectedPiece;
                QMessageBox::warning(this, "Not Enough Suns", "You don't have enough suns to place this plant.");
                return; // No permitir la colocación
            }

            // Restar el costo al contador de soles
            fichasLogic.incrementSunCont(-plantCost);
            ui->sunCont->setText(QString::number(fichasLogic.getSunCont()));
        }

        // Validar y manejar zombies
        if (selectedPiece == "graveStone" || selectedPiece == "zombieNormal" || selectedPiece == "coneZombie") {
            auto zombieCosts = fichasLogic.getZombiePieces();
            if (zombieCosts.find(selectedPiece) == zombieCosts.end()) {
                qDebug() << "Invalid zombie selected: " << selectedPiece;
                return; // No hacer nada si el zombie no existe
            }

            int zombieCost = zombieCosts.at(selectedPiece);

            if (fichasLogic.getBadSunCont() < zombieCost) {
                qDebug() << "Not enough suns to place " << selectedPiece;
                QMessageBox::warning(this, "Not Enough Suns", "You don't have enough suns to place this zombie.");
                return;
            }

            fichasLogic.incrementBadSunCont(-zombieCost);
            ui->badSunCont->setText(QString::number(fichasLogic.getBadSunCont()));
        }


        if (selectedPiece == "sunFlower") {
            icon = QIcon(":/Resources/sunFlower.png");

            // Obtener la posición del botón en la cuadrícula
            int index = ui->gridLayout->indexOf(button);
            int row, col, rowSpan, colSpan;
            ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);

            // Crear un temporizador para este SunFlower
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]() {
                fichasLogic.incrementSunCont(50); // Incrementar los soles
                ui->sunCont->setText(QString::number(fichasLogic.getSunCont())); // Actualizar la UI
                // Mostrar la imagen de goodSun en el botón
                if (ui->goodSunPic) {
                    QIcon goodSunIcon(":/Resources/goodSun.png");
                    ui->goodSunPic->setIcon(goodSunIcon);
                    ui->goodSunPic->setIconSize(QSize(ui->goodSunPic->width(), ui->goodSunPic->height()));
                    ui->goodSunPic->setStyleSheet("border: none; padding: 0;");

                    // Eliminar la imagen después de 2 segundos
                    QTimer::singleShot(2000, this, [=]() {
                        ui->goodSunPic->setIcon(QIcon());
                    });
                }
                qDebug() << "SunFlower at (" << row << ", " << col << ") generated 50 suns. Total suns: " << fichasLogic.getSunCont();
            });
            timer->start(5000); // Generar soles cada 5 segundos

            // Guardar el temporizador en el mapa
            sunFlowerTimers[QPair<int, int>(row, col)] = timer;

            qDebug() << "SunFlower placed at (" << row << ", " << col << ")";


        } else if (selectedPiece == "nuez") {
            icon = QIcon(":/Resources/nuez.png");
        } else if (selectedPiece == "peaShooter") {
            icon = QIcon(":/Resources/peaShooter.png");
            int index = ui->gridLayout->indexOf(button);
            int row, col, rowSpan, colSpan;
            ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);
            peaShooterPositions.push_back({row, col});

            qDebug() << "PeaShooter placed at (" << row << ", " << col << ")";

            // Start the timer if it's not already running
            if (!shootingTimer->isActive()) {
                shootingTimer->start(1000); // Start timer to shoot every second
                qDebug() << "Shooting timer started.";
            }

        } else if (selectedPiece == "zombieNormal" || selectedPiece == "coneZombie") {
            // Asigna el ícono adecuado para los zombies que se mueven
            if (selectedPiece == "zombieNormal") {
                icon = QIcon(":/Resources/zombieNormal.png");
            } else if (selectedPiece == "coneZombie") {
                icon = QIcon(":/Resources/coneZombie.png");
            }

            // Registra la posición del zombie en el QVector
            int index = ui->gridLayout->indexOf(button);
            int row, col, rowSpan, colSpan;
            ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);
            zombiePositions.push_back({row, col});

            // Actualiza el mapa con el tipo de zombie
            zombieTypeMap[QPair<int, int>(row, col)] = selectedPiece;

            qDebug() << "Zombie placed at (" << row << ", " << col << ")";
        } else if (selectedPiece == "graveStone") {
            // Asigna el ícono pero no lo agrega a zombiePositions
            icon = QIcon(":/Resources/graveStone.png");
            // Obtener la posición del botón en la cuadrícula
            int index = ui->gridLayout->indexOf(button);
            int row, col, rowSpan, colSpan;
            ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);

            // Verifica si ya existe un graveStone en esta posición
            if (graveStoneTimers.contains(QPair<int, int>(row, col))) {
                qDebug() << "graveStone already exists at (" << row << ", " << col << ").";
                return; // No agregar otro temporizador
            }

            // Crear un temporizador para este graveStone
            QTimer *timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]() {
                fichasLogic.incrementBadSunCont(50); // Incrementar los soles
                ui->badSunCont->setText(QString::number(fichasLogic.getBadSunCont())); // Actualizar la UI
                // Mostrar la imagen de goodSun en el botón
                if (ui->badSunPic) {
                    QIcon badSunIcon(":/Resources/badSun.png");
                    ui->badSunPic->setIcon(badSunIcon);
                    ui->badSunPic->setIconSize(QSize(ui->badSunPic->width(), ui->badSunPic->height()));
                    ui->badSunPic->setStyleSheet("border: none; padding: 0;");

                    // Eliminar la imagen después de 2 segundos
                    QTimer::singleShot(2000, this, [=]() {
                        ui->badSunPic->setIcon(QIcon());
                    });
                }

                qDebug() << "graveStone at (" << row << ", " << col << ") generated 50 suns. Total suns: " << fichasLogic.getBadSunCont();
            });
            timer->start(5000); // Generar soles cada 5 segundos

            // Guardar el temporizador en el mapa
            graveStoneTimers[QPair<int, int>(row, col)] = timer;

            // **Aquí registramos la posición del graveStone en zombieTypeMap**
            zombieTypeMap[QPair<int, int>(row, col)] = "graveStone";

            qDebug() << "graveStone placed at (" << row << ", " << col << ")";

        }


        if (!icon.isNull()) {
            button->setIcon(icon);
            button->setIconSize(QSize(button->width() * 0.9, button->height() * 0.9)); // Tamaño ajustado al 90% del botón
            button->setStyleSheet("border: none; padding: 0;"); // Asegura que la imagen ocupe todo el espacio

        }

        selectedPiece.clear(); // Limpiar la selección después de colocar la pieza
        // Switch the turn to the other player
        currentPlayer = (currentPlayer == "currentUser") ? "opponent" : "currentUser";
        // Update the label with the new player's name
        QString currentPlayerDisplayName = (currentPlayer == "currentUser") ? currentUser : opponentName;
        ui->currentPlayerName->setText("Turno actual: " + currentPlayerDisplayName);
        // Check for winner
        checkWinner();
        qDebug() << "Turn switched. Current player: " << currentPlayer;

}


void MainWindow::removeGraveStone(int row, int col)
{
    QPair<int, int> position(row, col);
    if (graveStoneTimers.contains(position)) {
        QTimer *timer = graveStoneTimers[position];
        timer->stop();        // Detener el temporizador
        timer->deleteLater(); // Liberar memoria del temporizador
        graveStoneTimers.remove(position); // Remover del mapa

        qDebug() << "graveStone at (" << row << ", " << col << ") has been removed. Timer stopped.";
        // Check if plants win after removing a graveStone
        if (fichasLogic.getBadSunCont() <= 0 && zombiePositions.isEmpty() && graveStoneTimers.isEmpty()) {
            gameCountdownTimer->stop(); // Stop the countdown timer
            stopGame("Plants"); // Declare plants as winners
        }
    } else {
        qDebug() << "No graveStone timer found at (" << row << ", " << col << ").";
    }
}


void MainWindow::removeSunFlower(int row, int col)
{
    QPair<int, int> position(row, col);
    if (sunFlowerTimers.contains(position)) {
        QTimer *timer = sunFlowerTimers[position];
        timer->stop();        // Detener el temporizador
        timer->deleteLater(); // Liberar memoria del temporizador
        sunFlowerTimers.remove(position); // Remover del mapa

        qDebug() << "SunFlower at (" << row << ", " << col << ") has been removed. Timer stopped.";
    } else {
        qDebug() << "No SunFlower timer found at (" << row << ", " << col << ").";
    }
}


void MainWindow::handleSunFlowerDestruction(int row, int col)
{
    QPushButton *button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(row, col)->widget());
    if (button && !button->icon().isNull()) {
        button->setIcon(QIcon()); // Eliminar el ícono visual
        removeSunFlower(row, col); // Limpiar el temporizador asociado
        qDebug() << "SunFlower at (" << row << ", " << col << ") has been destroyed.";
    } else {
        qDebug() << "No SunFlower found at (" << row << ", " << col << ").";
    }
}


// Mapa para rastrear los impactos recibidos por cada zombieNormal en el tablero
//QMap<QPair<int, int>, int> zombieImpactCount;

void MainWindow::handlePeaShooterShooting()
{
    if (peaShooterPositions.isEmpty()) {
        qDebug() << "No PeaShooters on the board to shoot.";
        return;
    }

    for (const auto& pos : peaShooterPositions) {
        int row = pos.first;
        int col = pos.second;
        bool targetInRow = false;
        int closestTargetCol = -1;

        // Buscar el objetivo más cercano (zombie o graveStone) en la misma fila hacia la derecha
        for (auto it = zombieTypeMap.begin(); it != zombieTypeMap.end(); ++it) {
            if (it.key().first == row && it.key().second > col &&
                (it.value() == "zombieNormal" || it.value() == "coneZombie" || it.value() == "graveStone")) {
                targetInRow = true;
                if (closestTargetCol == -1 || it.key().second < closestTargetCol) {
                    closestTargetCol = it.key().second;
                }
            }
        }

        if (!targetInRow) {
            qDebug() << "No targets in row " << row << ". PeaShooter will not shoot.";
            continue;
        }

        // Obtener el widget en la posición del PeaShooter
        QWidget *widget = ui->gridLayout->itemAtPosition(row, col) ? ui->gridLayout->itemAtPosition(row, col)->widget() : nullptr;
        if (!widget) {
            qDebug() << "No widget found at position (" << row << ", " << col << ")";
            continue;
        }

        // Posición inicial del PeaShooter
        QPoint startPos = widget->mapTo(this, QPoint(0, 0));

        // Crear un QLabel para el disparo
        QLabel *shootieLabel = new QLabel(this);
        shootieLabel->setPixmap(QPixmap(":/Resources/shootie.png"));
        shootieLabel->setGeometry(startPos.x() + widget->width(), startPos.y(), 30, 30);
        shootieLabel->show();

        qDebug() << "PeaShooter at (" << row << ", " << col << ") is shooting.";

        QWidget *targetWidget = ui->gridLayout->itemAtPosition(row, closestTargetCol) ? ui->gridLayout->itemAtPosition(row, closestTargetCol)->widget() : nullptr;
        if (targetWidget) {
            QPoint endPos = targetWidget->mapTo(this, QPoint(0, 0));

            int baseDurationPerPixel = 2; // Ajusta este valor según sea necesario
            int distance = std::abs(endPos.x() - startPos.x());
            int totalDuration = distance * baseDurationPerPixel;

            // Animar el movimiento del disparo
            QPropertyAnimation *animation = new QPropertyAnimation(shootieLabel, "geometry");
            animation->setDuration(totalDuration);
            animation->setStartValue(shootieLabel->geometry());
            animation->setEndValue(QRect(endPos.x(), shootieLabel->y(), 30, 30));

            connect(animation, &QPropertyAnimation::finished, this, [=]() {
                shootieLabel->deleteLater();

                // Verificar si aún hay un objetivo en la posición cuando el disparo llega
                QPushButton *targetButton = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(row, closestTargetCol) ? ui->gridLayout->itemAtPosition(row, closestTargetCol)->widget() : nullptr);
                if (targetButton && !targetButton->icon().isNull()) {
                    QPair<int, int> position(row, closestTargetCol);
                    QString targetType = zombieTypeMap.value(position);

                    if (targetType == "graveStone") {
                        // Incrementar el contador de impactos para el graveStone
                        zombieImpactCount[position]++;
                        qDebug() << "graveStone at (" << row << ", " << closestTargetCol << ") has been hit " << zombieImpactCount[position] << " times.";

                        // Verificar si el graveStone ha recibido 10 impactos
                        if (zombieImpactCount[position] >= 10) {
                            qDebug() << "graveStone at (" << row << ", " << closestTargetCol << ") has been destroyed.";
                            targetButton->setIcon(QIcon()); // Eliminar el ícono del graveStone
                            zombieTypeMap.remove(position); // Eliminar la posición del mapa
                            zombieImpactCount.remove(position); // Eliminar el contador de impactos

                            // Eliminar la posición del graveStone de zombiePositions
                            for (int i = 0; i < zombiePositions.size(); ++i) {
                                if (zombiePositions[i] == position) {
                                    zombiePositions.removeAt(i);
                                    break;
                                }
                            }

                            // Detener y eliminar el temporizador asociado al graveStone
                            removeGraveStone(row, closestTargetCol);
                            // Verificar si las plantas ganaron
                            if (fichasLogic.getBadSunCont() <= 0 && zombiePositions.isEmpty() && graveStoneTimers.isEmpty()) {
                                gameCountdownTimer->stop(); // Stop the countdown timer
                                stopGame("Plants"); // Declare plants as winners
                                return;
                            }
                        } else {
                            QIcon shadedGraveIcon(":/Resources/shadedgraveStone.png");
                            targetButton->setIcon(shadedGraveIcon);

                            QTimer::singleShot(500, this, [=]() {
                                if (targetButton && !targetButton->icon().isNull()) {
                                    targetButton->setIcon(QIcon(":/Resources/graveStone.png"));
                                }
                            });
                        }
                    } else if (targetType == "coneZombie") {
                            // Incrementar el contador de impactos para el coneZombie
                            zombieImpactCount[position]++;
                            qDebug() << "coneZombie at (" << row << ", " << closestTargetCol << ") has been hit " << zombieImpactCount[position] << " times.";

                            // Verificar si el coneZombie ha recibido 15 impactos
                            if (zombieImpactCount[position] >= 15) {
                                qDebug() << "coneZombie at (" << row << ", " << closestTargetCol << ") has been destroyed.";
                                targetButton->setIcon(QIcon()); // Eliminar el ícono del coneZombie
                                zombieTypeMap.remove(position); // Eliminar la posición del mapa
                                zombieImpactCount.remove(position); // Eliminar el contador de impactos

                                // Eliminar la posición del coneZombie de zombiePositions
                                for (int i = 0; i < zombiePositions.size(); ++i) {
                                    if (zombiePositions[i] == position) {
                                        zombiePositions.removeAt(i);
                                        break;
                                    }
                                }
                            } else {
                                QIcon shadedConeIcon(":/Resources/shadedConeZ.png");
                                targetButton->setIcon(shadedConeIcon);

                                QTimer::singleShot(500, this, [=]() {
                                    if (targetButton && !targetButton->icon().isNull()) {
                                        targetButton->setIcon(QIcon(":/Resources/coneZombie.png"));
                                    }
                                });
                            }

                    } else if (targetType == "zombieNormal") {
                        // Incrementar el contador de impactos para el zombieNormal
                        zombieImpactCount[position]++;
                        qDebug() << "zombieNormal at (" << row << ", " << closestTargetCol << ") has been hit " << zombieImpactCount[position] << " times.";

                        // Verificar si el zombieNormal ha recibido 10 impactos
                        if (zombieImpactCount[position] >= 10) {
                            qDebug() << "zombieNormal at (" << row << ", " << closestTargetCol << ") has been destroyed.";
                            targetButton->setIcon(QIcon()); // Eliminar el ícono del zombieNormal
                            zombieTypeMap.remove(position); // Eliminar la posición del mapa
                            zombieImpactCount.remove(position); // Eliminar el contador de impactos

                            // Eliminar la posición del zombie de zombiePositions
                            for (int i = 0; i < zombiePositions.size(); ++i) {
                                if (zombiePositions[i] == position) {
                                    zombiePositions.removeAt(i);
                                    break;
                                }
                            }
                        } else {
                            QIcon shadedIcon(":/Resources/shadedNormalZombie.png");
                            targetButton->setIcon(shadedIcon);

                            QTimer::singleShot(500, this, [=]() {
                                if (targetButton && !targetButton->icon().isNull()) {
                                    targetButton->setIcon(QIcon(":/Resources/zombieNormal.png"));
                                }
                            });
                        }
                    }
                } else {
                    qDebug() << "No target found at (" << row << ", " << closestTargetCol << ") when the shootie arrived.";
                }
            });

            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}



void MainWindow::selectSunFlower()
{
    selectedPiece = "sunFlower";
}

void MainWindow::selectNuez()
{
    selectedPiece = "nuez";
}

void MainWindow::selectPeaShooter()
{
    selectedPiece = "peaShooter";
}

void MainWindow::selectGraveStone()
{
    selectedPiece = "graveStone";
}

void MainWindow::selectConeZombie()
{
    selectedPiece = "coneZombie";
}

void MainWindow::selectNormalZombie()
{
    selectedPiece = "zombieNormal";
}


void MainWindow::on_oponentOption_clicked()
{
    if (ui->plantsOpt->isChecked() || ui->zombiesOpt->isChecked()) {
        if (ui->opponentsListWidget->isVisible()) {
            // Si la lista ya está visible, la ocultamos
            ui->opponentsListWidget->hide();
        } else {
            // Si no está visible, la mostramos
            ui->opponentsListWidget->show();
        qDebug() << "Faction selected. Preparing opponent list.";
        qDebug() << "Registered users before filtering:" << registeredUsers;
        qDebug() << "Current user:" << currentUser;

        QStringList opponents = registeredUsers;
        opponents.removeAll(currentUser); // Exclude the current user
        qDebug() << "Filtered opponents (excluding current user):" << opponents;

        if (opponents.isEmpty()) {
            QMessageBox::information(this, "No Opponents", "No opponents are available at the moment.");
        } else {
            ui->opponentsListWidget->clear(); // Clear any existing items
            for (const QString &opponent : opponents) {
                ui->opponentsListWidget->addItem(opponent); // Add each opponent to the list
                qDebug() << "Added opponent to list:" << opponent;
            }

            // Connect the itemClicked signal to a new slot
            connect(ui->opponentsListWidget, &QListWidget::itemClicked, this, &MainWindow::onOpponentSelected);
        }
        }
    } else {
        QMessageBox::warning(this, "Action Required", "Please select a faction (Plants or Zombies) before proceeding.");
    }
}


void MainWindow::onOpponentSelected(QListWidgetItem *item)
{
    if (!item) {
        return; // No item selected
    }

    opponentName = item->text(); // Store the selected opponent's name
    qDebug() << "Opponent selected:" << opponentName;

    // Determine factions for current user and opponent
    if (ui->plantsOpt->isChecked()) {
        currentFaction = "Plants";
        opponentFaction = "Zombies";
    } else {
        currentFaction = "Zombies";
        opponentFaction = "Plants";
    }

    // Set initial turn to current user
    currentPlayer = "currentUser";

    // Update the label to show the current turn
    ui->currentPlayerName->setText("Turno actual: " + currentUser);

    qDebug() << "Game initialized. Current player: " << currentPlayer;
    qDebug() << "Current faction: " << currentFaction << ", Opponent faction: " << opponentFaction;

    // Configure the cronometerLbl
    remainingTime = 180; // 3 minutos en segundos
    ui->cronometerLbl->setText("03:00"); // Establece el formato inicial del temporizador
    gameCountdownTimer->start(1000); // Configura el temporizador para que se actualice cada segundo


    // Transition to the game page
    ui->stackedWidget->setCurrentIndex(1);


}

void MainWindow::updateCountdown()
{
    if (remainingTime > 0) {
        --remainingTime;

        // Calcula los minutos y segundos restantes
        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        // Actualiza el texto del temporizador en formato mm:ss
        ui->cronometerLbl->setText(QString("%1:%2")
                                       .arg(minutes, 2, 10, QChar('0')) // Formato de 2 dígitos para minutos
                                       .arg(seconds, 2, 10, QChar('0'))); // Formato de 2 dígitos para segundos
    } else {
        // Cuando el tiempo se agota, se verifica el ganador
        checkWinner();
    }
}


void MainWindow::checkWinner()
{
    if (!gameInProgress) {
        return; // Don't check if the game is already over
    }


    // Check if zombies reached the other side and there are no motoSierra buttons
    QList<QPushButton *> finalRowMotosierraButtons = {
        ui->motoS1, ui->motoS2, ui->motoS3, ui->motoS4, ui->motoS5
    };

    bool motoSierraExists = false;
    for (QPushButton *button : finalRowMotosierraButtons) {
        if (!button->icon().isNull()) {
            motoSierraExists = true;
            break;
        }
    }

    for (const auto &zombie : zombiePositions) {
        int col = zombie.second;
        if (col == 0 && !motoSierraExists) { // Zombies reached the end and no motoSierra exists
            stopGame("Zombies");
            return;
        }
    }

    // Check if no suns left and no SunFlowers on the board
    if (fichasLogic.getSunCont() <= 0 && sunFlowerTimers.isEmpty()) {
        stopGame("Zombies");
        return;
    }

    // Check if badSunCont is 0 and no zombies or graveStones are on the board
    if (fichasLogic.getBadSunCont() <= 0 && zombiePositions.isEmpty() && graveStoneTimers.isEmpty()) {
        stopGame("Plants");
        return;
    }


    // Check if the timer expired and plants win
    if (remainingTime <= 0) {
        stopGame("Plants");
    }
}

void MainWindow::stopGame(const QString &winner)
{
    gameInProgress = false;

    // Detener todos los temporizadores existentes
    if (gameCountdownTimer->isActive()) {
        gameCountdownTimer->stop();
    }
    if (shootingTimer->isActive()) {
        shootingTimer->stop();
    }
    if (zombieMovementTimer->isActive()) {
        zombieMovementTimer->stop();
    }

    // Detener temporizadores de SunFlowers y graveStones
    for (auto &timer : sunFlowerTimers) {
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    sunFlowerTimers.clear();

    for (auto &timer : graveStoneTimers) {
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    graveStoneTimers.clear();

    // Detener y eliminar debugTimer
    QList<QTimer *> allTimers = findChildren<QTimer *>();
    for (QTimer *timer : allTimers) {
        if (timer->isActive()) {
            timer->stop();
            timer->deleteLater();
        }
    }

    // Mostrar un mensaje del ganador
    QMessageBox::information(this, "Game Over", "The winner is: " + winner + "!");

    // Emitir señales basadas en el ganador
    if (winner == currentFaction) {
        qDebug() << "El usuario ganó. Procediendo a noche.ui.";
        emit proceedToNoche(); // Señal para abrir noche.ui
    } else {
        qDebug() << "El usuario perdió. Regresando a Form.";
        emit gameEnded(winner); // Señal para regresar a Form
    }
    // Cerrar la ventana
    this->close();
}

void MainWindow::performAIMove() {
    qDebug() << "AI Turn: Evaluating best move";

    if (!gameInProgress) {
        return; // Stop if the game is already over
    }

    if (opponentFaction == "Plants") {
        // Lógica de la AI para Plants
        performPlantsAIMove();
    } else if (opponentFaction == "Zombies") {
        // Lógica de la AI para Zombies
        performZombiesAIMove();
    }
}

void MainWindow::performPlantsAIMove() {
    qDebug() << "AI Turn: Evaluating best move";
    if (!gameInProgress) {
        return; // Stop if the game is already over
    }

    QList<QPushButton *> availableButtons;
    QMap<QPushButton *, QPair<int, int>> buttonPositions;

    // Collect all empty buttons and their positions
    for (int i = 0; i < ui->gridLayout->count(); ++i) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
            if (button->icon().isNull()) {
                availableButtons.append(button);
                int row, col, rowSpan, colSpan;
                ui->gridLayout->getItemPosition(i, &row, &col, &rowSpan, &colSpan);
                buttonPositions[button] = QPair<int, int>(row, col);
            }
        }
    }
    if (availableButtons.isEmpty()) {
        qDebug() << "No available buttons for AI to place a piece.";
        return; // Game should end if there are no available slots
    }

    QPushButton *selectedButton = nullptr;
    QString aiPiece;
    int aiSuns = fichasLogic.getSunCont();

    // Track zombies in each row and their positions
    QMap<int, int> zombiesPerRow; // Row -> Zombie Count
    QMap<int, int> zombieClosestToEnd; // Row -> Closest Zombie Column
    QSet<int> rowsWithPeaShooters; // Rows that already have a PeaShooter


    for (auto it = zombieTypeMap.begin(); it != zombieTypeMap.end(); ++it) {
        QPair<int, int> position = it.key();
        QString zombieType = it.value();

        if (zombieType == "zombieNormal" || zombieType == "coneZombie") {
            int row = position.first;
            int col = position.second;

            zombiesPerRow[row]++;
            if (!zombieClosestToEnd.contains(row) || col < zombieClosestToEnd[row]) {
                zombieClosestToEnd[row] = col; // Track closest zombie to the end
            }
        }
    }


    // Check rows that already have PeaShooters
    for (auto it = peaShooterPositions.begin(); it != peaShooterPositions.end(); ++it) {
        rowsWithPeaShooters.insert(it->first);
    }

    // Step 1: Place PeaShooters in rows with zombies, prioritizing columns 2-4
    for (int row : zombieClosestToEnd.keys()) {
        int zombieCount = zombiesPerRow[row];

        // Ensure only one PeaShooter per row unless there are multiple zombies
        if (!rowsWithPeaShooters.contains(row) || zombieCount > 1) {
            if (aiSuns >= 150) { // Place only if suns are enough
                // Try to place in columns 2, 3, 4 (in that order)
                for (int col = 1; col <= 4; ++col) {
                    for (QPushButton *button : availableButtons) {
                        int buttonRow = buttonPositions[button].first;
                        int buttonCol = buttonPositions[button].second;

                        if (buttonRow == row && buttonCol == col) {
                            selectedButton = button;
                            aiPiece = "peaShooter";
                            break;
                        }
                    }
                    if (selectedButton) break;
                }
            }
        }
        if (selectedButton) break;
    }

    // Step 2: If no zombies are present, place a Nuez
    if (!selectedButton && zombieClosestToEnd.isEmpty() && aiSuns >= 100) {
        for (QPushButton *button : availableButtons) {
            int col = buttonPositions[button].second;
            if (col >= 3 && col <= 4) { // Place Nuez in defensive columns
                selectedButton = button;
                aiPiece = "nuez";
                break;
            }
        }
    }


    // Step 2: Place SunFlowers if no zombies or resources are limited
    if (!selectedButton && aiSuns >= 50) {
        for (QPushButton *button : availableButtons) {
            int col = buttonPositions[button].second;
            if (col == 1) { // Column 1
                selectedButton = button;
                aiPiece = "sunFlower";

                break;
            }
        }
    }

    // Step 4: Ensure a piece is placed no matter what
    if (!selectedButton) {
        qDebug() << "AI could not place a preferred piece. Defaulting to Nuez placement.";
        for (QPushButton *button : availableButtons) {
            int col = buttonPositions[button].second;
            if (col >= 3 && col <= 4) { // Default to placing a Nuez defensively
                selectedButton = button;
                aiPiece = "nuez";
                break;
            }
        }
    }

    // Simulate piece selection
    selectedPiece = aiPiece;

    // Update AI turn display
    ui->currentPlayerName->setText("Turno actual: " + opponentName);

    // Simulate a click on the selected button
    emit selectedButton->clicked();

    qDebug() << "Calling checkWinner after AI move...";
    checkWinner();
    qDebug() << "Finished checkWinner after AI move.";


    // Change turn to user
    currentPlayer = "currentUser";
    ui->currentPlayerName->setText("Turno actual: " + currentUser);

    qDebug() << "AI placed " << aiPiece << " on the board.";
}

void MainWindow::performZombiesAIMove() {
    qDebug() << "AI Turn (Zombies): Evaluating best move";

    if (!gameInProgress) {
        return; // Stop if the game is already over
    }

    QList<QPushButton *> availableButtons;
    QMap<QPushButton *, QPair<int, int>> buttonPositions;

    // Collect all empty buttons and their positions
    for (int i = 0; i < ui->gridLayout->count(); ++i) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
            if (button->icon().isNull()) {
                availableButtons.append(button);
                int row, col, rowSpan, colSpan;
                ui->gridLayout->getItemPosition(i, &row, &col, &rowSpan, &colSpan);
                buttonPositions[button] = QPair<int, int>(row, col);
            }
        }
    }

    if (availableButtons.isEmpty()) {
        qDebug() << "No available buttons for AI to place a piece.";
        return;
    }

    QPushButton *selectedButton = nullptr;
    QString aiPiece;
    int aiSuns = fichasLogic.getBadSunCont();

    // Step 1: Place GraveStone as the first move
    if (zombieTypeMap.isEmpty() && aiSuns >= 50) {
        QList<QPushButton *> allowedGraveStoneButtons = {
            ui->pushButton_10, ui->pushButton_28, ui->pushButton_19,
            ui->pushButton_37, ui->pushButton_46
        };

        for (QPushButton *button : allowedGraveStoneButtons) {
            if (button->icon().isNull()) {
                selectedButton = button;
                aiPiece = "graveStone";
                break;
            }
        }
    }

    // Step 2: Place ConeZombie if sufficient resources and no PeaShooters in the row
    if (!selectedButton && aiSuns >= 150) {
        QList<QPushButton *> allowedZombieButtons = {
            ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
            ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
            ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
        };

        QSet<int> rowsWithPeaShooters;
        for (const auto &peaShooter : peaShooterPositions) {
            rowsWithPeaShooters.insert(peaShooter.first);
        }

        for (QPushButton *button : allowedZombieButtons) {
            int row = buttonPositions[button].first;
            if (!rowsWithPeaShooters.contains(row)) { // Avoid rows with PeaShooters
                selectedButton = button;
                aiPiece = "coneZombie";
                break;
            }
        }
    }

    // Step 3: Place ZombieNormal if no ConeZombie and sufficient resources
    if (!selectedButton && aiSuns >= 100) {
        QList<QPushButton *> allowedZombieButtons = {
            ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
            ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
            ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
        };

        QSet<int> rowsWithPeaShooters;
        for (const auto &peaShooter : peaShooterPositions) {
            rowsWithPeaShooters.insert(peaShooter.first);
        }

        for (QPushButton *button : allowedZombieButtons) {
            int row = buttonPositions[button].first;
            if (!rowsWithPeaShooters.contains(row)) { // Avoid rows with PeaShooters
                selectedButton = button;
                aiPiece = "zombieNormal";
                break;
            }
        }
    }

    // Step 4: Place ConeZombie or ZombieNormal in any row if no options left
    if (!selectedButton && aiSuns >= 150) {
        QList<QPushButton *> allowedZombieButtons = {
            ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
            ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
            ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
        };

        for (QPushButton *button : allowedZombieButtons) {
            selectedButton = button;
            aiPiece = "coneZombie";
            break;
        }
    }

    if (!selectedButton && aiSuns >= 100) {
        QList<QPushButton *> allowedZombieButtons = {
            ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
            ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
            ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
        };

        for (QPushButton *button : allowedZombieButtons) {
            selectedButton = button;
            aiPiece = "zombieNormal";
            break;
        }
    }

    // Ensure a piece is placed
    if (!selectedButton) {
        qDebug() << "AI could not place any piece. Skipping turn is not allowed.";
        return;
    }

    // Simulate piece selection
    selectedPiece = aiPiece;

    // Update AI turn display
    ui->currentPlayerName->setText("Turno actual: " + opponentName);

    // Simulate a click on the selected button
    emit selectedButton->clicked();

    qDebug() << "Calling checkWinner after AI move...";
    checkWinner();
    qDebug() << "Finished checkWinner after AI move.";

    // Change turn to user
    currentPlayer = "currentUser";
    ui->currentPlayerName->setText("Turno actual: " + currentUser);

    qDebug() << "AI placed " << aiPiece << " on the board.";
}



void MainWindow::on_AIOption_clicked()
{
    if (ui->plantsOpt->isChecked() || ui->zombiesOpt->isChecked()) {
        ui->stackedWidget->setCurrentIndex(1); // Ir al tablero de juego

        // Configuración inicial
        if (ui->plantsOpt->isChecked()) {
            currentFaction = "Plants";
            opponentFaction = "Zombies";
        } else {
            currentFaction = "Zombies";
            opponentFaction = "Plants";
        }

        currentPlayer = "currentUser"; // Empieza el usuario
        opponentName = "IA"; // Nombre explícito para la IA
        ui->currentPlayerName->setText("Turno actual: " + currentUser);

        // Configura e inicia el cronómetro
        remainingTime = 180; // 3 minutos en segundos
        ui->cronometerLbl->setText("03:00"); // Establece el formato inicial del temporizador
        gameCountdownTimer->start(1000); // Actualizar cada segundo
        connect(gameCountdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdown);

        // Inicia el temporizador para manejar los turnos de la computadora
        QTimer *aiTimer = new QTimer(this);
        connect(aiTimer, &QTimer::timeout, this, [=]() {
            if (currentPlayer == "opponent") {
                ui->currentPlayerName->setText("Turno actual: " + opponentName); // Mostrar IA
                performAIMove(); // Realizar el movimiento de la computadora
            }
        });
        aiTimer->start(2000); // La computadora hará un movimiento cada 2 segundos si es su turno
    } else {
        QMessageBox::warning(this, "Acción requerida", "Debe seleccionar un bando (Plants o Zombies) antes de continuar.");
    }
}



void MainWindow::on_mainMenu_clicked()
{


    // Detener temporizadores activos
    if (gameCountdownTimer->isActive()) {
        gameCountdownTimer->stop();
    }
    if (shootingTimer->isActive()) {
        shootingTimer->stop();
    }
    if (zombieMovementTimer->isActive()) {
        zombieMovementTimer->stop();
    }

    // Detener temporizadores de SunFlowers
    for (auto &timer : sunFlowerTimers) {
        if (timer && timer->isActive()) {
            timer->stop();
        }
    }

    // Detener temporizadores de GraveStones
    for (auto &timer : graveStoneTimers) {
        if (timer && timer->isActive()) {
            timer->stop();
        }
    }

    // Mostrar cuadro de confirmación
    QMessageBox confirmBox;
    confirmBox.setWindowTitle("Confirmación");
    confirmBox.setText("¿Está seguro de que desea regresar al menú principal? Perderá todo su progreso.");
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox.setDefaultButton(QMessageBox::No);

    int response = confirmBox.exec();

    if (response == QMessageBox::Yes) {
        if (formInstance) { // Verifica que formInstance sea válida
            formInstance->show(); // Asegura que la ventana Form se muestre

            formInstance->navigateToPage3WithConfirmation(); // Navegar a la página 3 del formulario
        } else {
            qDebug() << "No se pudo regresar al menú principal: instancia de Form no válida.";
        }
        this->close(); // Cerrar la ventana actual (MainWindow)
    } else {
        // Reiniciar temporizadores si el usuario cancela
        if (!gameCountdownTimer->isActive()) {
            gameCountdownTimer->start(1000);
        }
        if (!shootingTimer->isActive()) {
            shootingTimer->start(1000);
        }
        if (!zombieMovementTimer->isActive()) {
            zombieMovementTimer->start(5000);
        }
        // Reiniciar temporizadores de SunFlowers
        for (auto &timer : sunFlowerTimers) {
            if (timer && !timer->isActive()) {
                timer->start(5000); // Reanudar con el intervalo original de producción
            }
        }
        // Reiniciar temporizadores de GraveStones
        for (auto &timer : graveStoneTimers) {
            if (timer && !timer->isActive()) {
                timer->start(5000); // Reanudar con el intervalo original de producción
            }
        }
    }
}



