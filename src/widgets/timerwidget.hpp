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
    Q_OBJECT

    QLabel mTimerLabel;

    QTimer mTimer;

    QElapsedTimer mElapsedTime;

public:
    explicit TimerWidget(QWidget *parent = nullptr);


private slots:
    void refreshTimerDisplay();

};

#endif // TIMERWIDGET_HPP
