#ifndef TIMERWIDGET_HPP
#define TIMERWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QTimer>
#include <QElapsedTimer>

/**
 * @brief Widget représentant le chronomètre
 */
class TimerWidget : public QWidget
{
    // LEXIQUE : RT (Real Time) => Temps écoulé depuis le début de la partie
    //           IGT (In Game Time) => Temps de jeu effectif = RT - Temps total de pause

    Q_OBJECT

    QLabel mTimerLabel;             // Label d'affichage du temps en cours

    QTimer mTimer;                  // Timer de rafraichissement de l'affichage du chronomètre

    QElapsedTimer mElapsedTime;     // Temps écoulé depuis le début de la partie (RT)

    QElapsedTimer mPauseTimer;      // Temps écoulé depuis le début de la pause (si on est en pause)
    int mPauseTimeMs;               // Nombre total de millisecondes de pause, à retirer à mElapsedTime pour obtenir l'IGT

public:
    // Constructeur
    explicit TimerWidget(QWidget *parent = nullptr);

    // Arrêt du chronomètre. Retourne l'IGT
    QTime stop();

    // Redémarre le chronomètre
    void restart();

    // Met en pause le chronomètre
    void pause();

    // Reprendre le chronomètre, après une pause.
    void resume();

    // Redire **ms** millisecondes à l'IGT. En pratique, ajoute **ms** millisecondes au temps de pause
    void subTime(int ms);

    // Getter du nombre de millisecondes de l'IGT
    int getMs() { return mElapsedTime.elapsed() - mPauseTimeMs; };

private slots:
    // Slot de rafraichissement de l'affichage du chronomètre
    void refreshTimerDisplay();

};

#endif // TIMERWIDGET_HPP
