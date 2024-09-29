# Build

Qt 6.6.2 for MacOS

# Qml
Main.qml - описыввет основной UI приложения
Histogram.qml - компонента для отображения гистограммы

# С++
CHistogramModel - наследник класса QAbstractListModel, содержит в себе описание модели данных для гистограммы
CFileController - класс в котором происходит обработка комманд из приложения и запуск отдельного потока для обработки файла
