#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDate>
#include <QTime>
#include <QMainWindow>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//! \brief Класс основного окна
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор
    MainWindow(QWidget *parent = nullptr);
    //! \brief Деструктор
    ~MainWindow();

public slots:
    //! \brief Нажатие кнопки добавления события
    void on_pushButton_addEvent_clicked();
    //! \brief Событие изменения предстоящих дел при изменении даты
    void on_calendar_selectionChanged();
    //! \brief Кнопка добавления привычки
    void on_pushButton_addHabit_clicked();


    //! \brief Нажатие на ячейку 1 и 5 столбцов в трекере дел
    void on_table_events_cellClicked(int row, int column);
    //! \brief Нажатие на ячейку 1 и 5 столбцов в трекере привычек
    void on_table_habitTracker_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    //first load table for current date
    void InitEventsTable();
    QFile* file_event;
    QFile* file_tracker;
    void reWriteXml();
    void readXml();
    void reWriteXml_tracker();
    void readXml_tracker();
    void readXml_today();
};
#endif // MAINWINDOW_H
