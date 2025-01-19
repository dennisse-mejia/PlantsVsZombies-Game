cmake_minimum_required(VERSION 3.16)

project(ProyectoP3 VERSION 0.1 LANGUAGES CXX)

# Activar características de Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar dependencias de Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql) # Agregado Sql
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql) # Agregado Sql

# Archivos del proyecto
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    form.cpp
    form.h
    form.ui
    fichas.cpp
    fichas.h
    Resources/1stWall.png
    resources.qrc
    mainform.cpp
)

# Configuración de la ejecución
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProyectoP3
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        globals.h
        mainwindow.h
        mainwindow.cpp globals.cpp main.cpp mainform.cpp fichas.cpp
        mainwindow.ui
        noche.h noche.cpp noche.ui
        alberca.h alberca.cpp alberca.ui













    )
else()
    add_executable(ProyectoP3
        ${PROJECT_SOURCES}
    )
endif()

# Vincular bibliotecas necesarias
target_link_libraries(ProyectoP3 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql) # Vincula Sql

# Configuración específica para macOS
if(APPLE)
    set_target_properties(ProyectoP3 PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ProyectoP3
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

# Incluir rutas de instalación
include(GNUInstallDirs)
install(TARGETS ProyectoP3
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalizar ejecutable para Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ProyectoP3)
endif()
