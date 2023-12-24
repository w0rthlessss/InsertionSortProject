#include "AddArray.h"
#include "ui_AddArray.h"

/*
    Конструктор класса AddArray
    Инициализация интерфейса формы и подключение функции взаимодействия с интерфейсом
*/
AddArray::AddArray(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArray)
{
    ui->setupUi(this);

    //При изменении выбора заполнения вызов функции со сменой страницы
    connect(ui->inputSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeStackedWidgetPage()));
}

/*
    В деструкторе удаление указателя на объекты интерфейса
*/
AddArray::~AddArray()
{
    delete ui;
}

/*
    Функция, возвращающая выбранный метод заполнения массива
*/
int AddArray::GetInputOption() const{
    return ui->inputSelection->currentIndex();
}

/*
    Функция, возвращающая указанный размер массива для рандомного заполнения
*/
int AddArray::GetNumberOfElements() const{
    return ui->numberOfElements->value();
}

/*
    Функция, меняющая страницу StackedWidget при выборе разных методов заполнения
*/
void AddArray::ChangeStackedWidgetPage(){
    ui->stackedWidget->setCurrentIndex(ui->inputSelection->currentIndex());
    if(ui->stackedWidget->currentWidget() == ui->manualPage){
        ui->numberOfElements->setValue(1);
    }
}
