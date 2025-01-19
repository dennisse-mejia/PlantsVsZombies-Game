#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QStringList>
#include <QMap>        // Para QMap
#include <QPair>       // Para QPair

extern QStringList registeredUsers; // List of registered users
extern QString currentUser;         // Currently logged-in user
extern QMap<QPair<int, int>, int> zombieImpactCount;


#endif // GLOBALS_H


