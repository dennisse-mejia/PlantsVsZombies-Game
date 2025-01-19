#include "alberca.h"
#include "ui_alberca.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QList>
#include <QIcon>
#include <QPropertyAnimation>
#include "globals.h"

alberca::alberca(Form *form, QString currentFaction, QString opponentFaction, QString currentUser, QString opponentName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::alberca)
    , formInstance(form)
    , gameInProgress(true) // Inicializar el juego como en progreso
    , currentFaction(currentFaction)
    , opponentFaction(opponentFaction)
    , currentUser(currentUser)
    , opponentName(opponentName)
    , shootingTimer(new QTimer(this)) // Inicializar el temporizador de disparos
    , zombieMovementTimer(new QTimer(this)) // Inicializar el temporizador de movimiento de zombies
    , gameCountdownTimer(new QTimer(this)) // Temporizador para la cuenta regresiva
    , remainingTime(180) // 3 minutos en segundos
{

    currentPlayer = "currentUser"; // El usuario comienza
    qDebug() << "alberca initialized with currentPlayer:" << currentPlayer
             << ", currentFaction:" << currentFaction
             << ", opponentFaction:" << opponentFaction;

    setObjectName("alberca"); // Set a unique object name for debugging

    ui->setupUi(this);


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

    // Configurar estilo para los botones de PLANTAS
    ui->sunFlowerBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;" // Fondo transparente
        "    border: none;"                  // Sin borde
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
    ui->padBtn->setStyleSheet(
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
    ui->peaShooterBtn->setStyleSheet(
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

    // Configurar estilo para los botones de ZOMBIES
    ui->graveStoneBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        );
    ui->swimZombieBtn->setStyleSheet(
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
    ui->duckieZombieBtn->setStyleSheet(
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

    gameCountdownTimer->start(1000); // Cuenta regresiva cada segundo

    // Inicializar la etiqueta del jugador actual
    ui->currentPlayerName->setText("Turno actual: " + currentUser);
    ui->currentPlayerName->setStyleSheet("font-size: 16px; font-weight: bold;");

    // Conectar el temporizador de cuenta regresiva a su ranura
    connect(gameCountdownTimer, &QTimer::timeout, this, &alberca::updateCountdown);

    // Debugging: Confirmar inicialización
    qDebug() << "alberca initialized with currentFaction:" << currentFaction
             << ", opponentFaction:" << opponentFaction
             << ", currentUser:" << currentUser
             << ", opponentName:" << opponentName;

    // Inicializar el contador de soles
    ui->sunCont->setText(QString::number(fichasLogic.getSunCont()));
    ui->badSunCont->setText(QString::number(fichasLogic.getBadSunCont()));

    // Lista de botones específicos para colocar la imagen "motoSierra"
    QList<QPushButton *> motoSButtons = {
        ui->motoS1, ui->motoS2, ui->motoS3, ui->motoS4, ui->motoS5
    };

    // Asignar la imagen "motoSierra" a cada botón con el tamaño 82x92
    QIcon motoSierraIcon(":/Resources/motoSierra.png");
    for (QPushButton *button : motoSButtons) {
        if (button) {
            button->setIcon(motoSierraIcon);
            button->setIconSize(QSize(82, 92)); // Tamaño exacto
            button->setStyleSheet("border: none; padding: 0;");
            button->setMinimumSize(82, 92);
            button->setMaximumSize(82, 92);
        }
    }

    // Conectar botones PLANTAS a sus ranuras
    connect(ui->sunFlowerBtn, &QPushButton::clicked, this, &alberca::selectSunFlower);
    connect(ui->peaShooterBtn, &QPushButton::clicked, this, &alberca::selectPeaShooter);
    connect(ui->padBtn, &QPushButton::clicked, this, &alberca::selectPad);

    // Conectar botones ZOMBIES a sus ranuras
    connect(ui->graveStoneBtn, &QPushButton::clicked, this, &alberca::selectGraveStone);
    connect(ui->duckieZombieBtn, &QPushButton::clicked, this, &alberca::selectDuckieZombie);
    connect(ui->swimZombieBtn, &QPushButton::clicked, this, &alberca::selectSwimZombie);

    // Conectar cada botón de la cuadrícula a la ranura de selección
    for (int i = 0; i < ui->gridLayout->count(); ++i) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
            connect(button, &QPushButton::clicked, this, &alberca::handleGridButtonClick);

            // Ajustar el tamaño de los botones para que sean proporcionales
            button->setMinimumSize(82, 92);
            button->setMaximumSize(82, 92); // Tamaño máximo del botón
            button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            // Aplicar estilo para hacerlos transparentes
            button->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;" // Fondo transparente
                "    border: none;"                  // Sin bordes
                "}"
                );
        }
    }

    // Configuración del temporizador de disparos
    connect(shootingTimer, &QTimer::timeout, this, &alberca::handlePeaShooterShooting);

    // Configuración del temporizador de movimiento de zombies
    connect(zombieMovementTimer, &QTimer::timeout, this, &alberca::moveZombies);
    zombieMovementTimer->start(5000);
}

alberca::~alberca()
{
    delete ui;
}

void alberca::moveZombies()
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

// Mapa para rastrear los estados de los botones (qué pieza hay en cada uno)
QMap<QPushButton*, QString> buttonState;

void alberca::handleGridButtonClick()
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

    // Lista de botones de agua
    QList<QPushButton*> waterButtons = {
        ui->pushButton_20, ui->pushButton_21, ui->pushButton_22,
        ui->pushButton_23, ui->pushButton_24, ui->pushButton_25,
        ui->pushButton_26, ui->pushButton_27, ui->pushButton_28,
        ui->pushButton_11, ui->pushButton_12, ui->pushButton_13,
        ui->pushButton_14, ui->pushButton_15, ui->pushButton_16,
        ui->pushButton_17, ui->pushButton_18, ui->pushButton_19
    };

    // Check turn logic
    if (currentPlayer == "currentUser") {
        // Ensure the current user can only place pieces from their faction
        if ((currentFaction == "Plants" && (selectedPiece == "duckieZombie" || selectedPiece == "swimZombie" || selectedPiece == "graveStone")) ||
            (currentFaction == "Zombies" && (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "pad"))) {
            QMessageBox::warning(this, "Invalid Move", "You can only place pieces from your faction.");
            return;
        }
    } else if (currentPlayer == "opponent") {
        // Ensure the opponent can only place pieces from their faction
        if ((opponentFaction == "Plants" && (selectedPiece == "duckieZombie" || selectedPiece == "swimZombie" || selectedPiece == "graveStone")) ||
            (opponentFaction == "Zombies" && (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "pad"))) {
            QMessageBox::warning(this, "Invalid Move", "The opponent can only place pieces from their faction.");
            return;
        }
    } else {
        QMessageBox::warning(this, "Invalid Turn", "It's not your turn!");
        return;
    }


    // Verificar si el botón ya tiene un ícono y es un botón de agua
    if (!button->icon().isNull()) {
        if (waterButtons.contains(button) &&
            (selectedPiece == "sunFlower" || selectedPiece == "peaShooter")) {
            if (!buttonState.contains(button) || buttonState[button] != "pad") {
                QMessageBox::warning(this, "Invalid Placement", "You must place a Pad first on water buttons.");
                return;
            }
        } else {
            return; // Si no es un caso válido, no hacer nada
        }
    }



    // Listas de botones permitidos
    QList<QPushButton*> allowedGraveStoneButtons = {
        ui->pushButton_10, ui->pushButton_37, ui->pushButton_46
    };

    // Listas de botones permitidos para cada zombie
    QList<QPushButton*> allowedDuckieButtons = {
        ui->pushButton_8, ui->pushButton_9, ui->pushButton_35,
        ui->pushButton_36, ui->pushButton_44, ui->pushButton_45
    };

    QList<QPushButton*> allowedSwimButtons = {
        ui->pushButton_26, ui->pushButton_27, ui->pushButton_17,
        ui->pushButton_18, ui->pushButton_28, ui->pushButton_19
    };


    // Verifica si la pieza seleccionada es graveStone y si el botón es permitido
    if (selectedPiece == "graveStone" && !allowedGraveStoneButtons.contains(button)) {
        return; // Sale de la función si el botón no está permitido para graveStone
    }

    // Verifica si la pieza seleccionada es duckieZombie o swimZombie y si el botón es permitido
    if (selectedPiece == "duckieZombie" && !allowedDuckieButtons.contains(button)) {
       // QMessageBox::warning(this, "Invalid Placement", "Duckie Zombies can only be placed in sand lanes.");
        return;
    }

    if (selectedPiece == "swimZombie" && !allowedSwimButtons.contains(button)) {
        //QMessageBox::warning(this, "Invalid Placement", "Swim Zombies can only be placed in water lanes.");
        return;
    }



    QIcon icon;
    int cost = 0;


    if (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "pad") {
        auto plantCosts = fichasLogic.getPoolPlantPieces();
        if (plantCosts.find(selectedPiece) == plantCosts.end()) {
            qDebug() << "Invalid plant selected: " << selectedPiece;
            return;
        }
        cost = plantCosts.at(selectedPiece);
        if (fichasLogic.getSunCont() < cost) {
            QMessageBox::warning(this, "Not Enough Suns", "You don't have enough suns to place this plant.");
            return;
        }
    } else if (selectedPiece == "graveStone" || selectedPiece == "duckieZombie" || selectedPiece == "swimZombie") {
        auto zombieCosts = fichasLogic.getPoolZombiePieces();
        if (zombieCosts.find(selectedPiece) == zombieCosts.end()) {
            qDebug() << "Invalid zombie selected: " << selectedPiece;
            return;
        }
        cost = zombieCosts.at(selectedPiece);
        if (fichasLogic.getBadSunCont() < cost) {
            QMessageBox::warning(this, "Not Enough Suns", "You don't have enough suns to place this zombie.");
            return;
        }
    }

    if (selectedPiece == "sunFlower") {
        if (waterButtons.contains(button) && buttonState[button] == "pad") {
            icon = QIcon(":/Resources/padSunFlower.png");
        } else if (waterButtons.contains(button)) {
            QMessageBox::warning(this, "Invalid Placement", "You must place a Pad first on water buttons.");
            return;
        } else {
            icon = QIcon(":/Resources/sunFlower.png");
        }
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


    } else if (selectedPiece == "pad") {
        if (!waterButtons.contains(button)) {
            QMessageBox::warning(this, "Invalid Placement", "Pads can only be placed on water buttons.");
            return;
        }
        icon = QIcon(":/Resources/pad.png");
        buttonState[button] = "pad"; // Marcar el botón como ocupado con Pad

    } else if (selectedPiece == "peaShooter") {
        if (waterButtons.contains(button) && buttonState[button] == "pad") {
            icon = QIcon(":/Resources/padPeaShooter.png");
        } else if (waterButtons.contains(button)) {
            QMessageBox::warning(this, "Invalid Placement", "You must place a Pad first on water buttons.");
            return;
        } else {
            icon = QIcon(":/Resources/peaShooter.png");
        }
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

    } else if (selectedPiece == "duckieZombie" || selectedPiece == "swimZombie") {
        // Asigna el ícono adecuado para los zombies que se mueven
        if (selectedPiece == "duckieZombie") {
            icon = QIcon(":/Resources/duckieZombie.png");
        } else if (selectedPiece == "swimZombie") {
            icon = QIcon(":/Resources/swimZombie.png");
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


    // Aplicar ícono y actualizar costos
    if (!icon.isNull()) {
        button->setIcon(icon);
        button->setIconSize(QSize(button->width() * 0.9, button->height() * 0.9));
        buttonState[button] = selectedPiece; // Actualizar estado del botón


        if (selectedPiece == "sunFlower" || selectedPiece == "peaShooter" || selectedPiece == "pad") {
            fichasLogic.incrementSunCont(-cost);
            ui->sunCont->setText(QString::number(fichasLogic.getSunCont()));
        } else {
            fichasLogic.incrementBadSunCont(-cost);
            ui->badSunCont->setText(QString::number(fichasLogic.getBadSunCont()));
        }
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


void alberca::removeGraveStone(int row, int col)
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


void alberca::removeSunFlower(int row, int col)
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


void alberca::handleSunFlowerDestruction(int row, int col)
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

void alberca::handlePeaShooterShooting()
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
                (it.value() == "duckieZombie" || it.value() == "swimZombie" || it.value() == "graveStone")) {
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
                    } else if (targetType == "swimZombie") {
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
                            QIcon shadedConeIcon(":/Resources/shadedSwimZombie.png");
                            targetButton->setIcon(shadedConeIcon);

                            QTimer::singleShot(500, this, [=]() {
                                if (targetButton && !targetButton->icon().isNull()) {
                                    targetButton->setIcon(QIcon(":/Resources/swimZombie.png"));
                                }
                            });
                        }

                    } else if (targetType == "duckieZombie") {
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
                            QIcon shadedIcon(":/Resources/shadedDuckieZombie.png");
                            targetButton->setIcon(shadedIcon);

                            QTimer::singleShot(500, this, [=]() {
                                if (targetButton && !targetButton->icon().isNull()) {
                                    targetButton->setIcon(QIcon(":/Resources/duckieZombie.png"));
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


void alberca::selectSunFlower()
{
    selectedPiece = "sunFlower";
}

void alberca::selectPad()
{
    selectedPiece = "pad";
}

void alberca::selectPeaShooter()
{
    selectedPiece = "peaShooter";
}

void alberca::selectGraveStone()
{
    selectedPiece = "graveStone";
}

void alberca::selectDuckieZombie()
{
    selectedPiece = "duckieZombie";
}

void alberca::selectSwimZombie()
{
    selectedPiece = "swimZombie";
}


void alberca::updateCountdown()
{
    if (remainingTime > 0) {
        --remainingTime;

        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        ui->cronometerLbl->setText(QString("%1:%2")
                                       .arg(minutes, 2, 10, QChar('0'))
                                       .arg(seconds, 2, 10, QChar('0')));
    } else {
        checkWinner();
    }
}

void alberca::checkWinner()
{
    if (!gameInProgress) {
        return;
    }

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
        if (col == 0 && !motoSierraExists) {
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

    if (remainingTime <= 0) {
        stopGame("Plants");
    }
}

void alberca::stopGame(const QString &winner)
{
    gameInProgress = false;

    if (gameCountdownTimer->isActive()) {
        gameCountdownTimer->stop();
    }
    if (shootingTimer->isActive()) {
        shootingTimer->stop();
    }
    if (zombieMovementTimer->isActive()) {
        zombieMovementTimer->stop();
    }

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

    QList<QTimer *> allTimers = findChildren<QTimer *>();
    for (QTimer *timer : allTimers) {
        if (timer->isActive()) {
            timer->stop();
            timer->deleteLater();
        }
    }

    QMessageBox::information(this, "Game Over", "The winner is: " + winner + "!");
    // Siempre regresar a Form sin importar el resultado
    qDebug() << "El juego ha terminado. Regresando al Form.";
    emit gameEnded(winner); // Señal para regresar al Form

    // Cerrar la ventana actual
    this->close();
}

void alberca::on_mainMenu_clicked()
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
        if (formInstance) { // Verifica que formInstance sea válido
            formInstance->show(); // Asegura que la ventana Form se muestre
            formInstance->navigateToPage3WithConfirmation(); // Navegar a la página 3 del formulario
        } else {
            qDebug() << "No se pudo regresar al menú principal: formInstance no es válido.";
        }

        this->close(); // Cerrar la ventana actual (noche)
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

