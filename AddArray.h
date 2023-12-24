#ifndef ADDARRAY_H
#define ADDARRAY_H

#include <QDialog>

namespace Ui {
class AddArray;
}

class AddArray : public QDialog
{
    Q_OBJECT
public slots:
    void ChangeStackedWidgetPage();
public:
    explicit AddArray(QWidget *parent = nullptr);
    ~AddArray();
    int GetNumberOfElements() const;
    int GetInputOption() const;

private:
    Ui::AddArray *ui;
};

#endif // ADDARRAY_H
