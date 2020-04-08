#include "optionsdialog.hpp"

OptionsDialog::OptionsDialog(int currMazeWidth, int currMazeHeight, bool bonusTimeEnabled, bool penaltyTimeEnabled, QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    this->setFixedSize(this->size());

    ui.widthValue->setValue(currMazeWidth);
    ui.heightValue->setValue(currMazeHeight);

    ui.bonusTime->setChecked(bonusTimeEnabled);
    ui.penaltyTime->setChecked(penaltyTimeEnabled);
}

int OptionsDialog::getMazeWidth()
{
    return ui.widthValue->value();
}

int OptionsDialog::getMazeHeight()
{
    return ui.heightValue->value();
}

bool OptionsDialog::getIsBonusTimeEnabled()
{
    return ui.bonusTime->isChecked();
}

bool OptionsDialog::getIsPenaltyTimeEnabled()
{
    return ui.penaltyTime->isChecked();
}
