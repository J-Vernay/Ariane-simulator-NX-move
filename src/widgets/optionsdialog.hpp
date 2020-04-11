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
    // Constructeur
    explicit OptionsDialog(int currMazeWidth = 10, int currMazeHeight = 6, bool bonusTimeEnabled = true, bool penaltyTimeEnabled = true, QWidget *parent = nullptr);

    // Getters permettant de récupérer les règlages des options choisis par l'utilisateur
    int getMazeWidth();
    int getMazeHeight();
    bool getIsBonusTimeEnabled();
    bool getIsPenaltyTimeEnabled();

private:
    // Interface, générée par Qt
    Ui::OptionsDialog ui;
};

#endif // OPTIONSDIALOG_HPP
