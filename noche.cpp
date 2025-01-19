#include "noche.h"
#include "ui_noche.h"
#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QList>
#include <QIcon>
#include <QMessageBox>
#include <QPropertyAnimation>
#include "globals.h"
#include "alberca.h"
#include "form.h"




noche::noche(Form *form, QString currentFaction, QString opponentFaction, QString currentUser, QString opponentName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::noche)
    , formInstance(form)
    , gameInProgress(true) // Inicializar el juego como en progreso
    , currentFaction(currentFaction)
    , opponentFaction(opponentFaction)
    , currentUser(currentUser)
    , opponentName(opponentName)
    , shootingTimer(new QTimer(this)) // Initialize shooting timer
    , zombieMovementTimer(new QTimer(this))
    , gameCountdownTimer(new QTimer(this)) // Add countdown timer
    , remainingTime(180) // 3 minutes in seconds
{
    currentPlayer = "currentUser"; // El usuario comienza
    qDebug() << "noche initialized with currentPlayer:" << currentPlayer
             << ", currentFaction:" << currentFaction
             << ", opponentFaction:" << opponentFaction;

    setObjectName("noche"); // Set a unique object name for debugging

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
    ui->cuteMushBtn->setStyleSheet(
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
    ui->nuezBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        );
    ui->puffBtn->setStyleSheet(
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
    ui->sportZombieBtn->setStyleSheet(
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
    ui->oldZombieBtn->setStyleSheet(
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


    // Initialize the current player label
    ui->currentPlayerName->setText("Turno actual: " + currentUser);
    ui->currentPlayerName->setStyleSheet("font-size: 16px; font-weight: bold;");

    // Connect the countdown timer to its slot
    connect(gameCountdownTimer, &QTimer::timeout, this, &noche::updateCountdown);

    // Debugging: Confirm initialization
    qDebug() << "noche initialized with currentFaction:" << currentFaction
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
            button->setIconSize(QSize(82, 92)); // Exact size
            button->setStyleSheet("border: none; padding: 0;");
            button->setMinimumSize(82, 92);
            button->setMaximumSize(82, 92);
        }
    }

    // Conectar botones PLANTAS a sus ranuras
    connect(ui->cuteMushBtn, &QPushButton::clicked, this, &noche::selectCuteMush);
    connect(ui->nuezBtn, &QPushButton::clicked, this, &noche::selectNuez);
    connect(ui->puffBtn, &QPushButton::clicked, this, &noche::selectPuff);

    // Conectar botones ZOMBIES a sus ranuras
    connect(ui->graveStoneBtn, &QPushButton::clicked, this, &noche::selectGraveStone);
    connect(ui->sportZombieBtn, &QPushButton::clicked, this, &noche::selectSportZombie);
    connect(ui->oldZombieBtn, &QPushButton::clicked, this, &noche::selectOldZombie);

    // Conectar cada botón de la cuadrícula a la ranura de selección
    for (int i = 0; i < ui->gridLayout->count(); ++i) {
        QWidget *widget = ui->gridLayout->itemAt(i)->widget();
        if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
            connect(button, &QPushButton::clicked, this, &noche::handleGridButtonClick);

            // Ajustar el tamaño de los botones para que sean proporcionales
            button->setMinimumSize(82, 92);
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
    connect(shootingTimer, &QTimer::timeout, this, &noche::handlePuffShooting);

    // Configuración del temporizador de movimiento de zombies
    connect(zombieMovementTimer, &QTimer::timeout, this, &noche::moveZombies);
    zombieMovementTimer->start(5000);


}

noche::~noche()
{
    delete ui;
}

void noche::moveZombies()
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
                    // Si el botón motoSierra está vacío (sin ícono), mueve el zombie hacia él
                    if (nextButton->icon().isNull()) {
                        QIcon icon = currentButton->icon();
                        currentButton->setIcon(QIcon());
                        nextButton->setIcon(icon);
                        nextButton->setIconSize(QSize(nextButton->width() * 0.9, nextButton->height() * 0.9));
                        nextButton->setStyleSheet("border: none; padding: 0;");

                        // Actualiza la posición del zombie
                        zombiePositions[i].second -= 1;

                        // Detiene el juego y declara victoria para los zombies
                        gameCountdownTimer->stop(); // Detener el temporizador de cuenta regresiva
                        stopGame("Zombies"); // Declara a los zombies como ganadores
                        return;
                    }

                    // Si el botón motoSierra está ocupado, destruye tanto al zombie como al motoSierra
                    currentButton->setIcon(QIcon());
                    nextButton->setIcon(QIcon());

                    // Remueve la posición del zombie de la lista
                    zombiePositions.removeAt(i);
                    --i; // Ajusta el índice tras la eliminación

                    qDebug() << "Zombie at (" << row << ", " << col << ") was destroyed by motoSierra.";
                    // Check if plants win immediately after the zombie is destroyed
                    if (fichasLogic.getBadSunCont() <= 0 && zombiePositions.isEmpty() && graveStoneTimers.isEmpty()) {
                        gameCountdownTimer->stop(); // Stop the countdown timer
                        stopGame("Plants"); // Declare plants as winners
                        return; // Exit the function
                    }

                    continue; // Skip further processing for this zombie

                }

                // Verifica si hay un Puff en la posición a la que el zombie se va a mover
                auto it = std::find(puffPositions.begin(), puffPositions.end(), QPair<int, int>(row, col - 1));
                if (it != puffPositions.end()) {
                    // Elimina el ícono del Puff y remuévelo de la lista
                    nextButton->setIcon(QIcon());
                    puffPositions.erase(it);
                    qDebug() << "Puff at (" << row << ", " << col - 1 << ") has been removed by a zombie.";
                }

                // Verifica si hay un CuteMush en la posición a la que el zombie se va a mover
                QPair<int, int> position(row, col - 1);
                if (cuteMushTimers.contains(position)) {
                    // Elimina el CuteMush (ícono y temporizador)
                    nextButton->setIcon(QIcon());
                    removeCuteMush(row, col - 1); // Detener el temporizador asociado
                    qDebug() << "CuteMush at (" << row << ", " << col - 1 << ") has been destroyed by a zombie.";
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

void noche::handleGridButtonClick()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());

    if (!button || selectedPiece.isEmpty()) {
        return; // No button or piece selected
    }

    // Check turn logic
    if (currentPlayer == "currentUser") {
        // Ensure the current user can only place pieces from their faction
        if ((currentFaction == "Plants" && (selectedPiece == "oldZombie" || selectedPiece == "sportZombie" || selectedPiece == "graveStone")) ||
            (currentFaction == "Zombies" && (selectedPiece == "cuteMush" || selectedPiece == "puff" || selectedPiece == "nuez"))) {
            QMessageBox::warning(this, "Invalid Move", "You can only place pieces from your faction.");
            return;
        }
    } else if (currentPlayer == "opponent") {
        // Ensure the opponent can only place pieces from their faction
        if ((opponentFaction == "Plants" && (selectedPiece == "oldZombie" || selectedPiece == "sportZombie" || selectedPiece == "graveStone")) ||
            (opponentFaction == "Zombies" && (selectedPiece == "cuteMush" || selectedPiece == "puff" || selectedPiece == "nuez"))) {
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
    if ((selectedPiece == "oldZombie" || selectedPiece == "sportZombie") && !allowedZombieButtons.contains(button)) {
        return; // Sale de la función si el botón no está permitido para estos zombies
    }

    QIcon icon;

    // Validar y manejar plantas
    if (selectedPiece == "cuteMush" || selectedPiece == "puff" || selectedPiece == "nuez") {
        // Validar costos usando getPlantPieces()
        auto plantCosts = fichasLogic.getNightPlantPieces();
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
    if (selectedPiece == "graveStone" || selectedPiece == "oldZombie" || selectedPiece == "sportZombie") {
        auto zombieCosts = fichasLogic.getNightZombiePieces();
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

    // Configurar iconos y lógica específica
    if (selectedPiece == "cuteMush") {
        icon = QIcon(":/Resources/cuteMush.png");

        int index = ui->gridLayout->indexOf(button);
        int row, col, rowSpan, colSpan;
        ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            fichasLogic.incrementSunCont(50);
            ui->sunCont->setText(QString::number(fichasLogic.getSunCont()));
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
        timer->start(5000);

        cuteMushTimers[QPair<int, int>(row, col)] = timer;

        qDebug() << "CuteMush placed at (" << row << ", " << col << ")";
    } else if (selectedPiece == "nuez") {
        icon = QIcon(":/Resources/nuez.png");
    } else if (selectedPiece == "puff") {
        icon = QIcon(":/Resources/puff.png");
        int index = ui->gridLayout->indexOf(button);
        int row, col, rowSpan, colSpan;
        ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);
        puffPositions.push_back({row, col});

        qDebug() << "Puff placed at (" << row << ", " << col << ")";

        if (!shootingTimer->isActive()) {
            shootingTimer->start(1000);
            qDebug() << "Shooting timer started.";
        }
    } else if (selectedPiece == "oldZombie" || selectedPiece == "sportZombie") {
        if (selectedPiece == "oldZombie") {
            icon = QIcon(":/Resources/oldZombie.png");
        } else if (selectedPiece == "sportZombie") {
            icon = QIcon(":/Resources/sportZombie.png");
        }

        int index = ui->gridLayout->indexOf(button);
        int row, col, rowSpan, colSpan;
        ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);
        zombiePositions.push_back({row, col});

        zombieTypeMap[QPair<int, int>(row, col)] = selectedPiece;

        qDebug() << selectedPiece << " placed at (" << row << ", " << col << ")";
    } else if (selectedPiece == "graveStone") {
        icon = QIcon(":/Resources/graveStone.png");
        int index = ui->gridLayout->indexOf(button);
        int row, col, rowSpan, colSpan;
        ui->gridLayout->getItemPosition(index, &row, &col, &rowSpan, &colSpan);

        if (graveStoneTimers.contains(QPair<int, int>(row, col))) {
            qDebug() << "GraveStone already exists at (" << row << ", " << col << ").";
            return;
        }

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            fichasLogic.incrementBadSunCont(50);
            ui->badSunCont->setText(QString::number(fichasLogic.getBadSunCont()));
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
        timer->start(5000);

        graveStoneTimers[QPair<int, int>(row, col)] = timer;
        zombieTypeMap[QPair<int, int>(row, col)] = "graveStone";

        qDebug() << "GraveStone placed at (" << row << ", " << col << ")";
    }

    if (!icon.isNull()) {
        button->setIcon(icon);
        button->setIconSize(QSize(button->width() * 0.9, button->height() * 0.9));
        button->setStyleSheet("border: none; padding: 0;");
    }

    selectedPiece.clear();
    currentPlayer = (currentPlayer == "currentUser") ? "opponent" : "currentUser";
    QString currentPlayerDisplayName = (currentPlayer == "currentUser") ? currentUser : opponentName;
    ui->currentPlayerName->setText("Turno actual: " + currentPlayerDisplayName);
    checkWinner();
    qDebug() << "Turn switched. Current player: " << currentPlayer;
}

void noche::removeGraveStone(int row, int col)
{
    QPair<int, int> position(row, col);
    if (graveStoneTimers.contains(position)) {
        QTimer *timer = graveStoneTimers[position];
        timer->stop();        // Detener el temporizador
        timer->deleteLater(); // Liberar memoria del temporizador
        graveStoneTimers.remove(position); // Remover del mapa

        qDebug() << "GraveStone at (" << row << ", " << col << ") has been removed. Timer stopped.";
    } else {
        qDebug() << "No GraveStone timer found at (" << row << ", " << col << ").";
    }
}

void noche::removeCuteMush(int row, int col)
{
    QPair<int, int> position(row, col);
    if (cuteMushTimers.contains(position)) {
        QTimer *timer = cuteMushTimers[position];
        timer->stop();        // Detener el temporizador
        timer->deleteLater(); // Liberar memoria del temporizador
        cuteMushTimers.remove(position); // Remover del mapa

        qDebug() << "CuteMush at (" << row << ", " << col << ") has been removed. Timer stopped.";
        // Check if plants win after removing a graveStone
        if (fichasLogic.getBadSunCont() <= 0 && zombiePositions.isEmpty() && graveStoneTimers.isEmpty()) {
            gameCountdownTimer->stop(); // Stop the countdown timer
            stopGame("Plants"); // Declare plants as winners
        }
    } else {
        qDebug() << "No graveStone timer found at (" << row << ", " << col << ").";
    }
}

void noche::handleCuteMushDestruction(int row, int col)
{
    QPushButton *button = qobject_cast<QPushButton *>(ui->gridLayout->itemAtPosition(row, col)->widget());
    if (button && !button->icon().isNull()) {
        button->setIcon(QIcon()); // Eliminar el ícono visual
        removeCuteMush(row, col); // Limpiar el temporizador asociado
        qDebug() << "CuteMush at (" << row << ", " << col << ") has been destroyed.";
    } else {
        qDebug() << "No CuteMush found at (" << row << ", " << col << ").";
    }
}

// Mapa para rastrear los impactos recibidos por cada oldZombie en el tablero
//QMap<QPair<int, int>, int> zombieImpactCount;

void noche::handlePuffShooting()
{
    if (puffPositions.isEmpty()) {
        qDebug() << "No Puffs on the board to shoot.";
        return;
    }

    for (const auto& pos : puffPositions) {
        int row = pos.first;
        int col = pos.second;
        bool targetInRow = false;
        int closestTargetCol = -1;

        // Buscar el objetivo más cercano (zombie o graveStone) en la misma fila hacia la derecha
        for (auto it = zombieTypeMap.begin(); it != zombieTypeMap.end(); ++it) {
            if (it.key().first == row && it.key().second > col &&
                (it.value() == "oldZombie" || it.value() == "sportZombie" || it.value() == "graveStone")) {
                targetInRow = true;
                if (closestTargetCol == -1 || it.key().second < closestTargetCol) {
                    closestTargetCol = it.key().second;
                }
            }
        }

        if (!targetInRow) {
            qDebug() << "No targets in row " << row << ". Puff will not shoot.";
            continue;
        }

        // Obtener el widget en la posición del Puff
        QWidget *widget = ui->gridLayout->itemAtPosition(row, col) ? ui->gridLayout->itemAtPosition(row, col)->widget() : nullptr;
        if (!widget) {
            qDebug() << "No widget found at position (" << row << ", " << col << ")";
            continue;
        }

        // Posición inicial del Puff
        QPoint startPos = widget->mapTo(this, QPoint(0, 0));

        // Crear un QLabel para el disparo
        QLabel *shootieLabel = new QLabel(this);
        shootieLabel->setPixmap(QPixmap(":/Resources/shootieN.png"));
        shootieLabel->setGeometry(startPos.x() + widget->width(), startPos.y()+50, 30, 30);
        shootieLabel->show();

        qDebug() << "Puff at (" << row << ", " << col << ") is shooting.";

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
                    } else if (targetType == "sportZombie") {
                        // Incrementar el contador de impactos para el sportZombie
                        zombieImpactCount[position]++;
                        qDebug() << "sportZombie at (" << row << ", " << closestTargetCol << ") has been hit " << zombieImpactCount[position] << " times.";

                        // Verificar si el sportZombie ha recibido 15 impactos
                        if (zombieImpactCount[position] >= 15) {
                            qDebug() << "sportZombie at (" << row << ", " << closestTargetCol << ") has been destroyed.";
                            targetButton->setIcon(QIcon()); // Eliminar el ícono del sportZombie
                            zombieTypeMap.remove(position); // Eliminar la posición del mapa
                            zombieImpactCount.remove(position); // Eliminar el contador de impactos

                            // Eliminar la posición del sportZombie de zombiePositions
                            for (int i = 0; i < zombiePositions.size(); ++i) {
                                if (zombiePositions[i] == position) {
                                    zombiePositions.removeAt(i);
                                    break;
                                }
                            }
                        } else {
                            QIcon shadedSportIcon(":/Resources/shadedSportZombie.png");
                            targetButton->setIcon(shadedSportIcon);

                            QTimer::singleShot(500, this, [=]() {
                                if (targetButton && !targetButton->icon().isNull()) {
                                    targetButton->setIcon(QIcon(":/Resources/sportZombie.png"));
                                }
                            });
                        }

                    } else if (targetType == "oldZombie") {
                        // Incrementar el contador de impactos para el oldZombie
                        zombieImpactCount[position]++;
                        qDebug() << "oldZombie at (" << row << ", " << closestTargetCol << ") has been hit " << zombieImpactCount[position] << " times.";

                        // Verificar si el oldZombie ha recibido 10 impactos
                        if (zombieImpactCount[position] >= 10) {
                            qDebug() << "oldZombie at (" << row << ", " << closestTargetCol << ") has been destroyed.";
                            targetButton->setIcon(QIcon()); // Eliminar el ícono del oldZombie
                            zombieTypeMap.remove(position); // Eliminar la posición del mapa
                            zombieImpactCount.remove(position); // Eliminar el contador de impactos

                            // Eliminar la posición del oldZombie de zombiePositions
                            for (int i = 0; i < zombiePositions.size(); ++i) {
                                if (zombiePositions[i] == position) {
                                    zombiePositions.removeAt(i);
                                    break;
                                }
                            }
                        } else {
                            QIcon shadedIcon(":/Resources/shadedOldZombie.png");
                            targetButton->setIcon(shadedIcon);

                            QTimer::singleShot(500, this, [=]() {
                                if (targetButton && !targetButton->icon().isNull()) {
                                    targetButton->setIcon(QIcon(":/Resources/oldZombie.png"));
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

void noche::selectCuteMush()
{
    selectedPiece = "cuteMush";
}

void noche::selectNuez()
{
    selectedPiece = "nuez";
}

void noche::selectPuff()
{
    selectedPiece = "puff";
}

void noche::selectGraveStone()
{
    selectedPiece = "graveStone";
}

void noche::selectSportZombie()
{
    selectedPiece = "sportZombie";
}

void noche::selectOldZombie()
{
    selectedPiece = "oldZombie";
}

void noche::updateCountdown()
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

void noche::checkWinner()
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
    if (fichasLogic.getSunCont() <= 0 && cuteMushTimers.isEmpty()) {
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

void noche::stopGame(const QString &winner)
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

    for (auto &timer : cuteMushTimers) {
        if (timer) {
            timer->stop();
            timer->deleteLater();
        }
    }
    cuteMushTimers.clear();

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
    // Emitir señales basadas en el ganador
    if (winner == currentFaction) {
        qDebug() << "El usuario ganó. Procediendo a alberca.ui.";
        emit proceedToAlberca(); // Señal para abrir alberca.ui
    } else {
        qDebug() << "El usuario perdió. Regresando a Form.";
        emit gameEnded(winner); // Señal para regresar a Form
    }
    // Cerrar la ventana
    this->close();
}

// void noche::performAIMove() {
//     qDebug() << "AI Turn: Evaluating best move";

//     if (!gameInProgress) {
//         return; // Stop if the game is already over
//     }

//     if (opponentFaction == "Plants") {
//         // Lógica de la AI para Plants
//         performPlantsAIMove();
//     } else if (opponentFaction == "Zombies") {
//         // Lógica de la AI para Zombies
//         performZombiesAIMove();
//     }
// }

// void noche::performPlantsAIMove() {
//     qDebug() << "AI Turn: Evaluating best move";
//     if (!gameInProgress) {
//         return; // Stop if the game is already over
//     }

//     QList<QPushButton *> availableButtons;
//     QMap<QPushButton *, QPair<int, int>> buttonPositions;

//     // Collect all empty buttons and their positions
//     for (int i = 0; i < ui->gridLayout->count(); ++i) {
//         QWidget *widget = ui->gridLayout->itemAt(i)->widget();
//         if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
//             if (button->icon().isNull()) {
//                 availableButtons.append(button);
//                 int row, col, rowSpan, colSpan;
//                 ui->gridLayout->getItemPosition(i, &row, &col, &rowSpan, &colSpan);
//                 buttonPositions[button] = QPair<int, int>(row, col);
//             }
//         }
//     }
//     if (availableButtons.isEmpty()) {
//         qDebug() << "No available buttons for AI to place a piece.";
//         return; // Game should end if there are no available slots
//     }

//     QPushButton *selectedButton = nullptr;
//     QString aiPiece;
//     int aiSuns = fichasLogic.getSunCont();

//     // Track zombies in each row and their positions
//     QMap<int, int> zombiesPerRow; // Row -> Zombie Count
//     QMap<int, int> zombieClosestToEnd; // Row -> Closest Zombie Column
//     QSet<int> rowsWithPuffs; // Rows that already have a PeaShooter


//     for (auto it = zombieTypeMap.begin(); it != zombieTypeMap.end(); ++it) {
//         QPair<int, int> position = it.key();
//         QString zombieType = it.value();

//         if (zombieType == "oldZombie" || zombieType == "sportZombie") {
//             int row = position.first;
//             int col = position.second;

//             zombiesPerRow[row]++;
//             if (!zombieClosestToEnd.contains(row) || col < zombieClosestToEnd[row]) {
//                 zombieClosestToEnd[row] = col; // Track closest zombie to the end
//             }
//         }
//     }


//     // Check rows that already have PeaShooters
//     for (auto it = puffPositions.begin(); it != puffPositions.end(); ++it) {
//         rowsWithPuffs.insert(it->first);
//     }

//     // Step 1: Place PeaShooters in rows with zombies, prioritizing columns 2-4
//     for (int row : zombieClosestToEnd.keys()) {
//         int zombieCount = zombiesPerRow[row];

//         // Ensure only one PeaShooter per row unless there are multiple zombies
//         if (!rowsWithPuffs.contains(row) || zombieCount > 1) {
//             if (aiSuns >= 150) { // Place only if suns are enough
//                 // Try to place in columns 2, 3, 4 (in that order)
//                 for (int col = 1; col <= 4; ++col) {
//                     for (QPushButton *button : availableButtons) {
//                         int buttonRow = buttonPositions[button].first;
//                         int buttonCol = buttonPositions[button].second;

//                         if (buttonRow == row && buttonCol == col) {
//                             selectedButton = button;
//                             aiPiece = "puff";
//                             break;
//                         }
//                     }
//                     if (selectedButton) break;
//                 }
//             }
//         }
//         if (selectedButton) break;
//     }

//     // Step 2: If no zombies are present, place a Nuez
//     if (!selectedButton && zombieClosestToEnd.isEmpty() && aiSuns >= 100) {
//         for (QPushButton *button : availableButtons) {
//             int col = buttonPositions[button].second;
//             if (col >= 3 && col <= 4) { // Place Nuez in defensive columns
//                 selectedButton = button;
//                 aiPiece = "nuez";
//                 break;
//             }
//         }
//     }


//     // Step 2: Place SunFlowers if no zombies or resources are limited
//     if (!selectedButton && aiSuns >= 50) {
//         for (QPushButton *button : availableButtons) {
//             int col = buttonPositions[button].second;
//             if (col == 1) { // Column 1
//                 selectedButton = button;
//                 aiPiece = "cuteMush";

//                 break;
//             }
//         }
//     }

//     // Step 4: Ensure a piece is placed no matter what
//     if (!selectedButton) {
//         qDebug() << "AI could not place a preferred piece. Defaulting to Nuez placement.";
//         for (QPushButton *button : availableButtons) {
//             int col = buttonPositions[button].second;
//             if (col >= 3 && col <= 4) { // Default to placing a Nuez defensively
//                 selectedButton = button;
//                 aiPiece = "nuez";
//                 break;
//             }
//         }
//     }

//     // Simulate piece selection
//     selectedPiece = aiPiece;

//     // Update AI turn display
//     ui->currentPlayerName->setText("Turno actual: " + opponentName);

//     // Simulate a click on the selected button
//     emit selectedButton->clicked();

//     qDebug() << "Calling checkWinner after AI move...";
//     checkWinner();
//     qDebug() << "Finished checkWinner after AI move.";


//     // Change turn to user
//     currentPlayer = "currentUser";
//     ui->currentPlayerName->setText("Turno actual: " + currentUser);

//     qDebug() << "AI placed " << aiPiece << " on the board.";
// }

// void noche::performZombiesAIMove() {
//     qDebug() << "AI Turn (Zombies): Evaluating best move";

//     if (!gameInProgress) {
//         return; // Stop if the game is already over
//     }

//     QList<QPushButton *> availableButtons;
//     QMap<QPushButton *, QPair<int, int>> buttonPositions;

//     // Collect all empty buttons and their positions
//     for (int i = 0; i < ui->gridLayout->count(); ++i) {
//         QWidget *widget = ui->gridLayout->itemAt(i)->widget();
//         if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
//             if (button->icon().isNull()) {
//                 availableButtons.append(button);
//                 int row, col, rowSpan, colSpan;
//                 ui->gridLayout->getItemPosition(i, &row, &col, &rowSpan, &colSpan);
//                 buttonPositions[button] = QPair<int, int>(row, col);
//             }
//         }
//     }

//     if (availableButtons.isEmpty()) {
//         qDebug() << "No available buttons for AI to place a piece.";
//         return;
//     }

//     QPushButton *selectedButton = nullptr;
//     QString aiPiece;
//     int aiSuns = fichasLogic.getBadSunCont();

//     // Step 1: Place GraveStone as the first move
//     if (zombieTypeMap.isEmpty() && aiSuns >= 50) {
//         QList<QPushButton *> allowedGraveStoneButtons = {
//             ui->pushButton_10, ui->pushButton_28, ui->pushButton_19,
//             ui->pushButton_37, ui->pushButton_46
//         };

//         for (QPushButton *button : allowedGraveStoneButtons) {
//             if (button->icon().isNull()) {
//                 selectedButton = button;
//                 aiPiece = "graveStone";
//                 break;
//             }
//         }
//     }

//     // Step 2: Place ConeZombie if sufficient resources and no PeaShooters in the row
//     if (!selectedButton && aiSuns >= 150) {
//         QList<QPushButton *> allowedZombieButtons = {
//             ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
//             ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
//             ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
//         };

//         QSet<int> rowsWithPuffs;
//         for (const auto &puff : puffPositions) {
//             rowsWithPuffs.insert(puff.first);
//         }

//         for (QPushButton *button : allowedZombieButtons) {
//             int row = buttonPositions[button].first;
//             if (!rowsWithPuffs.contains(row)) { // Avoid rows with PeaShooters
//                 selectedButton = button;
//                 aiPiece = "sportZombie";
//                 break;
//             }
//         }
//     }

//     // Step 3: Place ZombieNormal if no ConeZombie and sufficient resources
//     if (!selectedButton && aiSuns >= 100) {
//         QList<QPushButton *> allowedZombieButtons = {
//             ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
//             ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
//             ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
//         };

//         QSet<int> rowsWithPuffs;
//         for (const auto &puff : puffPositions) {
//             rowsWithPuffs.insert(puff.first);
//         }

//         for (QPushButton *button : allowedZombieButtons) {
//             int row = buttonPositions[button].first;
//             if (!rowsWithPuffs.contains(row)) { // Avoid rows with PeaShooters
//                 selectedButton = button;
//                 aiPiece = "oldZombie";
//                 break;
//             }
//         }
//     }

//     // Step 4: Place ConeZombie or ZombieNormal in any row if no options left
//     if (!selectedButton && aiSuns >= 150) {
//         QList<QPushButton *> allowedZombieButtons = {
//             ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
//             ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
//             ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
//         };

//         for (QPushButton *button : allowedZombieButtons) {
//             selectedButton = button;
//             aiPiece = "sportZombie";
//             break;
//         }
//     }

//     if (!selectedButton && aiSuns >= 100) {
//         QList<QPushButton *> allowedZombieButtons = {
//             ui->pushButton_8, ui->pushButton_9, ui->pushButton_27,
//             ui->pushButton_17, ui->pushButton_18, ui->pushButton_35,
//             ui->pushButton_36, ui->pushButton_44, ui->pushButton_45, ui->pushButton_26
//         };

//         for (QPushButton *button : allowedZombieButtons) {
//             selectedButton = button;
//             aiPiece = "oldZombie";
//             break;
//         }
//     }

//     // Ensure a piece is placed
//     if (!selectedButton) {
//         qDebug() << "AI could not place any piece. Skipping turn is not allowed.";
//         return;
//     }

//     // Simulate piece selection
//     selectedPiece = aiPiece;

//     // Update AI turn display
//     ui->currentPlayerName->setText("Turno actual: " + opponentName);

//     // Simulate a click on the selected button
//     emit selectedButton->clicked();

//     qDebug() << "Calling checkWinner after AI move...";
//     checkWinner();
//     qDebug() << "Finished checkWinner after AI move.";

//     // Change turn to user
//     currentPlayer = "currentUser";
//     ui->currentPlayerName->setText("Turno actual: " + currentUser);

//     qDebug() << "AI placed " << aiPiece << " on the board.";
// }



void noche::on_mainMenu_clicked()
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
    for (auto &timer : cuteMushTimers) {
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
        // Reiniciar temporizadores de SunFlowers
        for (auto &timer : cuteMushTimers) {
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

