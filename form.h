#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QStringList>       // Para manejar la lista de usuarios
#include <QListWidgetItem>   // Para manejar elementos del QListWidget


namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void navigateToPage3WithConfirmation();


private slots:
    void on_okBtn_2_clicked();       // Maneja la validación y registro de usuarios
    void on_pushButton_clicked();    // Cambia específicamente a page_2
    void on_cancelBtn_clicked();     // Regresa a la página anterior
    void on_okBtn_clicked();         // Cambia explícitamente a page_2
    void on_createUserBtn_clicked(); // Cambia a page_5
    void on_exitGame_3_clicked();    // Termina la aplicación
    void on_renameButton_clicked();  // Maneja la acción de renombrar un usuario
    void on_renameConfirmButton_clicked(); // Maneja la confirmación de renombrar un usuario

    void on_cancelrename_clicked();

    void on_createButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_startGameBtn_clicked();
    void handleGameEnd(const QString &winner);





private:
    Ui::Form *ui;
    QStringList registeredUsers; // Lista para almacenar los usuarios registrados
    QString selectedUser;        // Usuario seleccionado para renombrar

    void loadUsersFromFile();    // Cargar usuarios desde un archivo
    void saveUsersToFile();      // Guardar usuarios en un archivo
    void loadLastUser();         // Cargar el último usuario desde un archivo
    void saveLastUser(const QString &username); // Guardar el último usuario en un archivo
};

#endif // FORM_H
