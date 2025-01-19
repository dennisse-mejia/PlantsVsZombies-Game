#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include "mainwindow.h" // Incluye MainWindow para abrirlo
#include "noche.h"
#include "alberca.h"



Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->startGameBtn->setStyleSheet("QPushButton {"
                                    "background-color: transparent;" // Eliminar el fondo
                                    "border: none;"                  // Eliminar el borde
                                    "}");

    // Forzar que siempre inicie en la página "page"
    ui->stackedWidget->setCurrentWidget(ui->page);
    qDebug() << "La página inicial configurada es 'page'.";

    // Configurar userPost como solo lectura
    ui->userPost->setReadOnly(true);

    // Conexiones de botones
    connect(ui->okBtn_2, &QPushButton::clicked, this, &Form::on_okBtn_2_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Form::on_pushButton_clicked); // Configuración para el botón de page
    connect(ui->renameButton, &QPushButton::clicked, this, &Form::on_renameButton_clicked);
    connect(ui->exitGame_3, &QPushButton::clicked, this, &Form::on_exitGame_3_clicked);
    connect(ui->createUserBtn, &QPushButton::clicked, this, &Form::on_createUserBtn_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Form::on_pushButton_4_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &Form::on_pushButton_5_clicked);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &Form::on_pushButton_6_clicked);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &Form::on_pushButton_7_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &Form::on_pushButton_8_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &Form::on_pushButton_9_clicked);

    // Cargar usuarios desde el archivo
    loadUsersFromFile();

    // Cargar el último usuario logueado
    loadLastUser();
}

Form::~Form()
{
    saveUsersToFile();
    delete ui;
}

void Form::on_pushButton_clicked()
{
    // Este botón es el que se encuentra en "page"
    if (registeredUsers.isEmpty()) {
        // Si no hay usuarios registrados, ir a page_2
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        qDebug() << "No hay usuarios registrados. Mostrando 'page_2' para crear un nuevo usuario.";
    } else {
        QFile file("last_user.txt");
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString lastUser = in.readLine().trimmed();

            if (!lastUser.isEmpty() && registeredUsers.contains(lastUser)) {
                // Si hay un usuario logueado, ir a page_4 y mostrarlo en userPost
                ui->userPost->setText( lastUser + "!");
                ui->stackedWidget->setCurrentWidget(ui->page_4);
                qDebug() << "Usuario encontrado: " << lastUser << ". Mostrando 'page_4'.";
            } else {
                // Si no hay un usuario válido, ir a page_2
                ui->stackedWidget->setCurrentWidget(ui->page_2);
                qDebug() << "No hay un último usuario válido. Mostrando 'page_2'.";
            }
        } else {
            // Si no existe el archivo, ir a page_2
            ui->stackedWidget->setCurrentWidget(ui->page_2);
            qDebug() << "Archivo 'last_user.txt' no encontrado. Mostrando 'page_2'.";
        }
    }
}

void Form::loadUsersFromFile()
{
    QFile file("registered_users.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo de usuarios para lectura.";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            registeredUsers.append(line);
            ui->userListWidget->addItem(line);
        }
    }

    file.close();
    qDebug() << "Usuarios cargados desde el archivo: " << registeredUsers;
}

void Form::saveUsersToFile()
{
    QFile file("registered_users.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo de usuarios para escritura.";
        return;
    }

    QTextStream out(&file);
    for (const QString &user : registeredUsers) {
        out << user << "\n";
    }

    file.close();
    qDebug() << "Usuarios guardados correctamente.";
}

void Form::saveLastUser(const QString &username)
{
    QFile file("last_user.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << username;
        file.close();
        qDebug() << "Último usuario guardado: " << username;
    } else {
        qDebug() << "No se pudo guardar el último usuario.";
    }
}

void Form::loadLastUser()
{
    QFile file("last_user.txt");

    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString lastUser = in.readLine().trimmed();

        if (!lastUser.isEmpty() && registeredUsers.contains(lastUser)) {
            // Establece el usuario cargado en userPost
            ui->userPost->setText(lastUser + "!");
            qDebug() << "Último usuario cargado: " << lastUser;
        } else {
            qDebug() << "No hay usuario válido registrado en 'last_user.txt'.";
        }
    } else {
        qDebug() << "Archivo 'last_user.txt' no encontrado.";
    }
}


void Form::on_okBtn_2_clicked()
{
    QString uName = ui->usernameField->text();

    if (uName.isEmpty()) {
        QMessageBox::information(this, "Error", "El nombre de usuario no puede estar vacío.");
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        return;
    }

    if (registeredUsers.contains(uName)) {
        QMessageBox::information(this, "Bienvenido de nuevo", "¡Bienvenido de nuevo, " + uName + "!");
        ui->userPost->setText( uName);
        saveLastUser(uName);
        ui->stackedWidget->setCurrentWidget(ui->page_4);
    } else {
        registeredUsers.append(uName);
        ui->userListWidget->addItem(uName);
        QMessageBox::information(this, "Registro Exitoso", "Usuario " + uName + " registrado exitosamente.");
        ui->userPost->setText( uName);
        saveLastUser(uName);
        ui->stackedWidget->setCurrentWidget(ui->page_4);
    }
}

void Form::on_cancelBtn_clicked()
{
    int currentIndex = ui->stackedWidget->currentIndex(); // Obtiene el índice actual

    if (currentIndex > 0) {
        ui->stackedWidget->setCurrentIndex(currentIndex - 1); // Cambia a la página anterior
        qDebug() << "Página cambiada a:" << currentIndex - 1;
    } else {
        qDebug() << "Ya estás en la primera página.";
    }
}

void Form::on_okBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void Form::on_createUserBtn_clicked()
{
    // Cambiar a la página page_5
    ui->stackedWidget->setCurrentWidget(ui->page_7);
    qDebug() << "Cambiando a la página page_7.";
}

void Form::on_exitGame_3_clicked()
{
    QApplication::quit(); // Termina la aplicación
    qDebug() << "El programa ha terminado.";
}

void Form::on_renameConfirmButton_clicked()
{
    // Obtén el nuevo nombre del QLineEdit
    QString newName = ui->rename->text().trimmed(); // Remueve espacios en blanco

    if (newName.isEmpty()) {
        // Si el nuevo nombre está vacío, muestra un error
        QMessageBox::warning(this, "Error", "El nuevo nombre no puede estar vacío.");
        return;
    }

    if (registeredUsers.contains(newName)) {
        // Si el nombre ya existe, muestra un error
        QMessageBox::warning(this, "Error", "El nombre ingresado ya está en uso.");
        return;
    }

    // Verifica si hay un usuario seleccionado
    if (selectedUser.isEmpty()) {
        QMessageBox::warning(this, "Error", "No hay un usuario seleccionado.");
        return;
    }

    // Encuentra el índice del usuario seleccionado en la lista de usuarios registrados
    int index = registeredUsers.indexOf(selectedUser);
    if (index != -1) {
        // Actualiza el nombre en la lista de usuarios registrados
        registeredUsers[index] = newName;

        // Actualiza el QListWidget con el nuevo nombre
        QListWidgetItem *selectedItem = ui->userListWidget->item(index);
        if (selectedItem) {
            selectedItem->setText(newName);
        }

        // Limpia el QLineEdit y regresa a la página page_7
        ui->rename->clear();
        ui->stackedWidget->setCurrentWidget(ui->page_7);

        // Guarda los cambios en el archivo
        saveUsersToFile();

        // Mensaje de confirmación
        QMessageBox::information(this, "Éxito", "El usuario ha sido renombrado correctamente.");

        // Mensaje de depuración
        qDebug() << "El usuario fue renombrado de" << selectedUser << "a" << newName;

        // Actualiza la variable `selectedUser`
        selectedUser = newName;
    } else {
        // Si no se encontró el usuario seleccionado, muestra un error
        QMessageBox::warning(this, "Error", "No se pudo encontrar el usuario seleccionado.");
    }
}


void Form::on_cancelrename_clicked()
{
    // Cambia a la página page_7
    ui->stackedWidget->setCurrentWidget(ui->page_7);

    // Mensaje de depuración para confirmar el cambio
    qDebug() << "Cancelado el cambio de nombre, regresando a la página page_7.";
}


void Form::on_renameButton_clicked()
{
    // Verifica si hay un usuario seleccionado
    QListWidgetItem *selectedItem = ui->userListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Error", "Por favor, selecciona un usuario para renombrar.");
        return;
    }

    // Establece el usuario seleccionado en la variable global
    selectedUser = selectedItem->text();
    qDebug() << "Usuario seleccionado para renombrar:" << selectedUser;

    // Cambia a la página page_8
    ui->stackedWidget->setCurrentWidget(ui->page_8);
}


void Form::on_createButton_clicked()
{
    // Cambia a la página page_9
    ui->stackedWidget->setCurrentWidget(ui->page_9);

    // Mensaje de depuración para confirmar el cambio
    qDebug() << "Cambiando a la página page_9.";
}


void Form::on_pushButton_6_clicked()
{
    // Obtén el usuario seleccionado del QListWidget
    QListWidgetItem *selectedItem = ui->userListWidget->currentItem();

    if (selectedItem) {
        // Obtén el texto del usuario seleccionado
        QString selectedUserName = selectedItem->text();

        // Guarda el usuario seleccionado como último usuario
        saveLastUser(selectedUserName);

        // Establece el texto en el QLineEdit userPost
        ui->userPost->setText( selectedUserName + "!");

        // Cambia a la página page_4
        ui->stackedWidget->setCurrentWidget(ui->page_4);

        // Mensaje de depuración
        qDebug() << "Usuario seleccionado: " << selectedUserName << " y guardado como último usuario.";
    } else {
        // Si no hay ningún usuario seleccionado, muestra un mensaje de error
        QMessageBox::warning(this, "Error", "Por favor selecciona un usuario de la lista.");
    }
}



void Form::on_pushButton_7_clicked()
{
    // Obtén el usuario seleccionado del QListWidget
    QListWidgetItem *selectedItem = ui->userListWidget->currentItem();

    if (selectedItem) {
        // Guarda el usuario seleccionado en la variable
        selectedUser = selectedItem->text();

        // Cambia a la página page_6
        ui->stackedWidget->setCurrentWidget(ui->page_6);

        // Mensaje de depuración
        qDebug() << "Preparado para eliminar el usuario:" << selectedUser;
    } else {
        // Si no hay ningún usuario seleccionado, muestra un mensaje de error
        QMessageBox::warning(this, "Error", "Por favor selecciona un usuario de la lista.");
    }
}

void Form::on_pushButton_9_clicked()
{
    // Verifica si hay un usuario seleccionado para eliminar
    if (!selectedUser.isEmpty()) {
        // Encuentra el índice del usuario seleccionado en la lista de usuarios registrados
        int index = registeredUsers.indexOf(selectedUser);

        if (index != -1) {
            // Elimina el usuario de la lista de usuarios registrados
            registeredUsers.removeAt(index);

            // Elimina el usuario del QListWidget
            delete ui->userListWidget->takeItem(index);

            // Limpia la variable `selectedUser`
            selectedUser.clear();

            // Guarda los cambios en el archivo
            saveUsersToFile();

            // Mensaje de confirmación
            QMessageBox::information(this, "Éxito", "El usuario ha sido eliminado correctamente.");

            // Verifica si el QListWidget está vacío
            if (registeredUsers.isEmpty()) {
                // Si no hay usuarios registrados, redirige a page_2
                ui->stackedWidget->setCurrentWidget(ui->page_2);
                qDebug() << "No hay usuarios registrados. Mostrando 'page_2' para crear un nuevo usuario.";
            } else {
                // Regresa a la página page_7 si aún hay usuarios
                ui->stackedWidget->setCurrentWidget(ui->page_7);
                qDebug() << "Usuario eliminado correctamente. Regresando a 'page_7'.";
            }
        } else {
            // Mensaje de error si no se encuentra el usuario en la lista
            QMessageBox::warning(this, "Error", "El usuario seleccionado no se encuentra en la lista.");
        }
    } else {
        // Mensaje de error si no hay un usuario seleccionado
        QMessageBox::warning(this, "Error", "No hay ningún usuario seleccionado para eliminar.");
    }
}


void Form::on_pushButton_8_clicked()
{
    // Cambia directamente a la página page_4
    ui->stackedWidget->setCurrentWidget(ui->page_4);

    // Mensaje de depuración para confirmar el cambio
    qDebug() << "Cancelado, regresando a la página page_4.";
}



void Form::on_pushButton_5_clicked()
{
    // Cambia directamente a la página page_4
    ui->stackedWidget->setCurrentWidget(ui->page_7);

    // Mensaje de depuración para confirmar el cambio
    qDebug() << "Cancelado, regresando a la página page_7.";
}


void Form::on_pushButton_4_clicked()
{
    // Obtiene el texto ingresado en el QLineEdit newUser
    QString newUserName = ui->newUser->text().trimmed();

    // Verifica si el campo está vacío
    if (newUserName.isEmpty()) {
        QMessageBox::warning(this, "Error", "El nombre de usuario no puede estar vacío.");
        return;
    }

    // Verifica si el usuario ya existe en la lista
    if (registeredUsers.contains(newUserName)) {
        QMessageBox::warning(this, "Error", "El nombre de usuario ya existe.");
        return;
    }

    // Agrega el nuevo usuario a la lista de usuarios registrados
    registeredUsers.append(newUserName);

    // Muestra el nuevo usuario en el QListWidget
    ui->userListWidget->addItem(newUserName);

    // Guarda el nuevo usuario en el archivo
    saveUsersToFile();

    // Actualiza el último usuario logueado
    saveLastUser(newUserName);

    // Actualiza el QLineEdit userPost con el nuevo usuario
    ui->userPost->setText( newUserName);

    // Limpia el QLineEdit después de agregar el usuario
    ui->newUser->clear();

    // Mensaje de confirmación
    QMessageBox::information(this, "Éxito", "El nuevo usuario ha sido agregado correctamente.");

    // Cambia automáticamente a la página page_4
    ui->stackedWidget->setCurrentWidget(ui->page_4);

    // Mensaje de depuración
    qDebug() << "Nuevo usuario agregado y logueado: " << newUserName;
}


// void Form::on_startGameBtn_clicked()
// {
//     this->close(); // Close the current window (form.ui)

//     // Pass registeredUsers and currentUser to MainWindow
//     MainWindow *mainWindow = new MainWindow(registeredUsers, ui->userPost->text().replace("!", ""), this);

//     // Connect the signal to handle game end
//     connect(mainWindow, &MainWindow::gameEnded, this, &Form::handleGameEnd);

//     mainWindow->show();

//     // Debugging: Confirm data being passed
//     qDebug() << "Starting MainWindow with registeredUsers:" << registeredUsers;
//     qDebug() << "Current user:" << ui->userPost->text().replace("!", "");
// }

void Form::on_startGameBtn_clicked()
{
    this->hide(); // Ocultar Form temporalmente

    // Crear una instancia de MainWindow
    MainWindow *mainWindow = new MainWindow(this, registeredUsers, ui->userPost->text().replace("!", ""), nullptr);

    connect(mainWindow, &MainWindow::proceedToNoche, this, [=]() {
        qDebug() << "Abriendo noche.ui.";

        // Crear la instancia de Noche con los valores obtenidos mediante getters
        noche *nocheWindow = new noche(this,
                                       mainWindow->getCurrentFaction(),
                                       mainWindow->getOpponentFaction(),
                                       mainWindow->getCurrentUser(),
                                       mainWindow->getOpponentName(),
                                       nullptr);
        // Conectar las señales de noche
        connect(nocheWindow, &noche::proceedToAlberca, this, [=]() {
            qDebug() << "Abriendo alberca.ui.";
            // Aquí puedes crear y mostrar alberca.ui
            alberca *albercaWindow = new alberca(this,
                                                 nocheWindow->getCurrentFaction(),
                                                 nocheWindow->getOpponentFaction(),
                                                 nocheWindow->getCurrentUser(),
                                                 nocheWindow->getOpponentName(),
                                                 nullptr);

            // Conectar la señal de alberca para regresar al Form
            connect(albercaWindow, &alberca::gameEnded, this, &Form::handleGameEnd);

            albercaWindow->show();
        });

        connect(nocheWindow, &noche::gameEnded, this, &Form::handleGameEnd);

        nocheWindow->show();
    });

    connect(mainWindow, &MainWindow::gameEnded, this, &Form::handleGameEnd);

    mainWindow->show();
}

void Form::navigateToPage3WithConfirmation()
{
    // Cambiar a la página 3
    ui->stackedWidget->setCurrentIndex(3);

    // Mostrar mensaje de depuración
    qDebug() << "Navegando a la página 3.";
}



void Form::handleGameEnd(const QString &winner)
{
    // Log para depuración
    qDebug() << "El ganador no es el usuario actual. Regresando a página 3.";

    // Regresar a la página 3
    ui->stackedWidget->setCurrentIndex(3);

    // Asegurarse de que Form se muestre como la ventana principal
    this->show();
}





// void Form::handleGameEnd(const QString &winner)
// {
//     // Debugging: Log the winner
//     qDebug() << "Game ended. Winner: " << winner;

//     // Go to page 3 of the stacked widget
//     ui->stackedWidget->setCurrentIndex(3);

//     // Show the form window
//     this->show();
// }


