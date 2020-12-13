#include <QCheckBox>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QXmlStreamReader>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_current_date->setAlignment(Qt::AlignCenter);
    QString fileName = QString("events_") + ui->calendar->selectedDate().toString(Qt::DateFormat::ISODate) + QString(".xml");
    file_event = new QFile(fileName);
    QString fileName_tracker = "tracker.xml";
    file_tracker = new QFile(fileName_tracker);
    readXml_tracker();

    InitEventsTable();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());
    ui->lineEdit_nameEvent->setPlaceholderText("Name");
    ui->lineEdit_noteEvent->setPlaceholderText("Note");

    ui->label_current_date->setText(QDate::currentDate().toString());

    ui->comboBox_repetitionRate->addItem("Every hour");
    ui->comboBox_repetitionRate->addItem("Every day");
    ui->comboBox_repetitionRate->addItem("Every week");

    readXml_today();
}

MainWindow::~MainWindow()
{
    reWriteXml();
    reWriteXml_tracker();
    delete ui;
}

void MainWindow::InitEventsTable()
{
    ui->table_events->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ui->table_habitTracker->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    readXml();
}



void MainWindow::on_pushButton_addEvent_clicked()
{
    if (ui->lineEdit_nameEvent->text().length() > 0)
    {
        int count_row = ui->table_events->rowCount();
        ui->table_events->insertRow(count_row);
        ui->table_events->setItem(count_row, 0, new QTableWidgetItem("No"));
        ui->table_events->setItem(count_row, 1, new QTableWidgetItem(ui->timeEdit->text()));
        ui->table_events->setItem(count_row, 2, new QTableWidgetItem(ui->lineEdit_nameEvent->text()));
        ui->table_events->setItem(count_row, 3, new QTableWidgetItem(ui->lineEdit_noteEvent->text()));
        ui->table_events->setItem(count_row, 4, new QTableWidgetItem("X"));
        ui->table_events->item(count_row, 0)->setFlags(Qt::ItemIsDropEnabled);
        ui->table_events->item(count_row, 4)->setFlags(Qt::ItemIsDropEnabled);

        reWriteXml();
        readXml_today();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Event name is null");
    }
}

void MainWindow::reWriteXml()
{
    if (file_event->open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter xmlWriter(file_event);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement(QString("date"));
        xmlWriter.writeAttribute("id", ui->dateEdit->date().toString(Qt::DateFormat::ISODate));
        for (int i = 0; i < ui->table_events->rowCount(); i++)
        {
            xmlWriter.writeStartElement("event");
            xmlWriter.writeTextElement("done", ui->table_events->item(i, 0)->text());
            xmlWriter.writeTextElement("time", ui->table_events->item(i, 1)->text());
            xmlWriter.writeTextElement("name", ui->table_events->item(i, 2)->text());
            xmlWriter.writeTextElement("note", ui->table_events->item(i, 3)->text());
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
        file_event->close();
    }
}

void MainWindow::readXml()
{
    if (file_event->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xml(file_event);
        while (!xml.atEnd() && !xml.hasError())
        {
            xml.readNext();
            if (xml.tokenType() == QXmlStreamReader::StartElement)//открывающий тэг
            {
                if (xml.name() == "date")
                {
                    int i = 0;
                    while(xml.name() != "date" || xml.tokenType() != QXmlStreamReader::EndElement)//while not found </date>
                    {
                        if (xml.tokenType() == QXmlStreamReader::StartElement)
                        {
                            if (xml.name() == "done")
                            {
                                i = ui->table_events->rowCount();
                                ui->table_events->insertRow(i);
                                xml.readNext();//read text between <done></done>
                                ui->table_events->setItem(i, 0, new QTableWidgetItem(xml.text().toString()));
                            }
                            if (xml.name() == "time")
                            {
                                xml.readNext();
                                ui->table_events->setItem(i, 1, new QTableWidgetItem(xml.text().toString()));
                            }

                            if (xml.name() == "name")
                            {
                                xml.readNext();
                                ui->table_events->setItem(i, 2, new QTableWidgetItem(xml.text().toString()));
                            }

                            if (xml.name() == "note")
                            {
                                xml.readNext();
                                ui->table_events->setItem(i, 3, new QTableWidgetItem(xml.text().toString()));
                                ui->table_events->setItem(i, 4, new QTableWidgetItem("X"));
                                ui->table_events->item(i, 0)->setFlags(Qt::ItemIsDropEnabled);
                                ui->table_events->item(i, 4)->setFlags(Qt::ItemIsDropEnabled);
                            }
                        }
                        xml.readNext();
                    }
                }
            }
        }
        file_event->close();
    }
}

void MainWindow::on_calendar_selectionChanged()
{
    reWriteXml();
    ui->label_current_date->setText(ui->calendar->selectedDate().toString());
    while (ui->table_events->rowCount() != 0)
    {
        ui->table_events->removeRow(0);
    }
    QString fileName = QString("events_") + ui->calendar->selectedDate().toString(Qt::DateFormat::ISODate) + QString(".xml");
    file_event->setFileName(fileName);
    readXml();
}

void MainWindow::on_pushButton_addHabit_clicked()
{
    if (ui->lineEdit_habitName->text().length() > 0)
    {
        int count_row = ui->table_habitTracker->rowCount();
        ui->table_habitTracker->insertRow(count_row);

        ui->table_habitTracker->setItem(count_row, 0, new QTableWidgetItem(ui->lineEdit_habitName->text()));
        ui->table_habitTracker->setItem(count_row, 1, new QTableWidgetItem(ui->comboBox_repetitionRate->currentText()));
        ui->table_habitTracker->setItem(count_row, 2, new QTableWidgetItem(QTime::currentTime().toString()));
        ui->table_habitTracker->setItem(count_row, 3, new QTableWidgetItem(QDate::currentDate().toString(Qt::DateFormat::ISODate)));
        ui->table_habitTracker->setItem(count_row, 4, new QTableWidgetItem("Ok"));
        ui->table_habitTracker->setItem(count_row, 5, new QTableWidgetItem("X"));
        ui->table_habitTracker->item(count_row, 4)->setFlags(Qt::ItemIsDropEnabled);
        ui->table_habitTracker->item(count_row, 5)->setFlags(Qt::ItemIsDropEnabled);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Habit name is null");
    }
}

void MainWindow::on_table_events_cellClicked(int row, int column)
{
    //done changed
    if (column == 0)
    {
        if (ui->table_events->item(row, 0)->text() == "No")
        {
            ui->table_events->item(row, 0)->setText("Yes");
        }
        else
        {
            ui->table_events->item(row, 0)->setText("No");
        }
    }

    //if delete
    if (column == 4)
    {
        ui->table_events->removeRow(row);
    }

    reWriteXml();
    if (ui->calendar->selectedDate() == QDate::currentDate())
    {
        readXml_today();
    }
}

void MainWindow::on_table_habitTracker_cellClicked(int row, int column)
{
    //if delete
    if (column == 5)
    {
        ui->table_habitTracker->removeRow(row);
    }
    //status
    if (column == 4)
    {
        if (ui->table_habitTracker->item(row, column)->text() == "Ok")
        {
            QMessageBox::information(this, "Ok", "You follow your schedule");
        }
        else
        {
            if (QMessageBox::question(this, "Hello", "Did you complete this action at the right time?", QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok);
            {
                ui->table_habitTracker->item(row, column)->setText("Ok");
                ui->table_habitTracker->item(row, 3)->setText(QDate::currentDate().toString(Qt::DateFormat::ISODate));
                ui->table_habitTracker->item(row, 2)->setText(QTime::currentTime().toString());
            }
        }
    }
}

void MainWindow::reWriteXml_tracker()
{
    if (file_tracker->open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter xmlWriter(file_tracker);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement(QString("habit"));
        for (int i = 0; i < ui->table_habitTracker->rowCount(); i++)
        {
            xmlWriter.writeTextElement("name", ui->table_habitTracker->item(i, 0)->text());
            xmlWriter.writeTextElement("repeat", ui->table_habitTracker->item(i, 1)->text());
            xmlWriter.writeStartElement("last_do");
            xmlWriter.writeTextElement("time", ui->table_habitTracker->item(i, 2)->text());
            xmlWriter.writeTextElement("date", ui->table_habitTracker->item(i, 3)->text());
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
        file_tracker->close();
    }
}

void MainWindow::readXml_tracker()
{
    if (file_tracker->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xml(file_tracker);
        while (!xml.atEnd() && !xml.hasError())
        {
            xml.readNext();
            if (xml.tokenType() == QXmlStreamReader::StartElement)//открывающий тэг
            {
                if (xml.name() == "habit")
                {
                    QXmlStreamAttributes attributes = xml.attributes();
                    int i = 0;
                    while(xml.name() != "habit" || xml.tokenType() != QXmlStreamReader::EndElement)//while not found </habit>
                    {
                        if (xml.tokenType() == QXmlStreamReader::StartElement)
                        {
                            if (xml.name() == "name")
                            {
                                i = ui->table_habitTracker->rowCount();
                                ui->table_habitTracker->insertRow(i);
                                xml.readNext();//read text between <done></done>
                                ui->table_habitTracker->setItem(i, 0, new QTableWidgetItem(xml.text().toString()));
                            }
                            if (xml.name() == "repeat")
                            {
                                xml.readNext();
                                ui->table_habitTracker->setItem(i, 1, new QTableWidgetItem(xml.text().toString()));
                            }

                            if (xml.name() == "time")
                            {
                                xml.readNext();
                                ui->table_habitTracker->setItem(i, 2, new QTableWidgetItem(xml.text().toString()));
                            }

                            if (xml.name() == "date")
                            {
                                xml.readNext();
                                ui->table_habitTracker->setItem(i, 3, new QTableWidgetItem(xml.text().toString()));
                                if (ui->table_habitTracker->item(i, 1)->text() == "Every week")
                                {
                                    QDate date_last_do = QDate::fromString(ui->table_habitTracker->item(i, 1)->text(), Qt::DateFormat::ISODate);
                                    if (QDate::currentDate().day() - date_last_do.day() > 7)
                                    {
                                        ui->table_habitTracker->setItem(i, 4, new QTableWidgetItem("Attention"));
                                    }
                                    else
                                    {
                                        ui->table_habitTracker->setItem(i, 4, new QTableWidgetItem("Ok"));
                                    }
                                }
                                if (ui->table_habitTracker->item(i, 1)->text() == "Every day" || ui->table_habitTracker->item(i, 1)->text() == "Every hour")
                                {
                                    QDate date_last_do = QDate::fromString(ui->table_habitTracker->item(i, 3)->text(), Qt::DateFormat::ISODate);
                                    if (QDate::currentDate().day() > date_last_do.day())
                                    {
                                        ui->table_habitTracker->setItem(i, 4, new QTableWidgetItem("Attention"));
                                    }
                                    else
                                    {
                                        if (ui->table_habitTracker->item(i, 1)->text() == "Every day")
                                        {
                                            ui->table_habitTracker->setItem(i, 4, new QTableWidgetItem("Ok"));
                                        }
                                        if (ui->table_habitTracker->item(i, 1)->text() == "Every hour")
                                        {
                                            QTime time_last_do = QTime::fromString(ui->table_habitTracker->item(i, 2)->text());
                                            QTime time_last_do_ok = time_last_do.addSecs(60*60);//from sec to hour
                                            if (QTime::currentTime() > time_last_do_ok)
                                            {
                                                ui->table_habitTracker->setItem(i, 4, new QTableWidgetItem("Attention"));
                                            }
                                            else
                                            {
                                                ui->table_habitTracker->setItem(i, 4, new QTableWidgetItem("Ok"));
                                            }
                                        }
                                    }
                                }
                                ui->table_habitTracker->setItem(i, 5, new QTableWidgetItem("X"));
                                ui->table_habitTracker->item(i, 4)->setFlags(Qt::ItemIsDropEnabled);
                                ui->table_habitTracker->item(i, 5)->setFlags(Qt::ItemIsDropEnabled);
                            }
                        }
                        xml.readNext();
                    }
                }
            }
        }
        file_tracker->close();
    }
}

void MainWindow::readXml_today()
{
    QString file_Name = QString("events_") + QDate::currentDate().toString(Qt::DateFormat::ISODate) + QString(".xml");
    QFile* file_today = new QFile(file_Name);
    QString text_remainder;
    int countSeconds_forRemainder = 3 * 3600;//3 hours * 60 minutes * 60 seconds
    QTime timeForRemainder = QTime::currentTime().addSecs(countSeconds_forRemainder);
    bool done_thisEvent = true;
    bool goodEvent = false;
    if (file_today->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xml(file_today);
        while (!xml.atEnd() && !xml.hasError())
        {
            xml.readNext();
            if (xml.tokenType() == QXmlStreamReader::StartElement)//открывающий тэг
            {
                if (xml.name() == "date")
                {
                    while(xml.name() != "date" || xml.tokenType() != QXmlStreamReader::EndElement)//while not found </date>
                    {
                        if (xml.tokenType() == QXmlStreamReader::StartElement)
                        {
                            if (xml.name() == "done")
                            {
                                xml.readNext();//read text between <done></done>
                                if (xml.text().toString() == "No")
                                {
                                    done_thisEvent = false;
                                }
                                else
                                {
                                    done_thisEvent = true;
                                }

                            }
                            if (!done_thisEvent)//если действие еще не выполнено
                            {
                                if (xml.name() == "time")
                                {
                                    xml.readNext();
                                    QTime timeThisEvent = QTime::fromString(xml.text().toString());
                                    if (timeThisEvent < timeForRemainder && timeThisEvent > QTime::currentTime())
                                    {
                                        text_remainder += xml.text().toString() + QString(" - ");
                                        goodEvent = true;
                                    }
                                    else
                                    {
                                        goodEvent = false;
                                    }
                                }
                                if (goodEvent)//подходит нам по времени и еще не выполнено
                                {
                                    if (xml.name() == "name")
                                    {
                                        xml.readNext();
                                        text_remainder += xml.text().toString() + QString(". \n");
                                    }

                                    if (xml.name() == "note")
                                    {
                                        xml.readNext();
                                        if (xml.text().toString().length() > 0)
                                        {
                                            text_remainder += QString("Note: ") + xml.text().toString() + QString(". \n");
                                        }
                                    }
                                }
                            }
                        }
                        xml.readNext();
                    }
                }
            }
        }
        file_today->close();
    }

    if (text_remainder.length() > 0)
    {
        ui->label_reminderEvents->setText(text_remainder);
    }
    else
    {
        ui->label_reminderEvents->setText("Нет предстоящих событий");
    }
}
