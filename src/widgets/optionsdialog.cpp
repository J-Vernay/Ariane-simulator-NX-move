#include "optionsdialog.hpp"

OptionsDialog::OptionsDialog(int currMazeWidth, int currMazeHeight,QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    this->setFixedSize(this->size());

    ui.widthValue->setValue(currMazeWidth);
    ui.heightValue->setValue(currMazeHeight);
}

int OptionsDialog::getMazeWidth()
{
    return ui.widthValue->value();
}

int OptionsDialog::getMazeHeight()
{
    return ui.heightValue->value();
}
