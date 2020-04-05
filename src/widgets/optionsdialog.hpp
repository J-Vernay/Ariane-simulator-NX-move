#ifndef OPTIONSDIALOG_HPP
#define OPTIONSDIALOG_HPP

#include "ui_optionsdialog.h"

/**
 * @brief Boite de dialogue pour la modification des options
 */
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(int currMazeWidth = 10, int currMazeHeight = 6, QWidget *parent = nullptr);
    int getMazeWidth();
    int getMazeHeight();

private:
    Ui::OptionsDialog ui;
};

#endif // OPTIONSDIALOG_HPP
