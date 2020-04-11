#include "timerwidget.hpp"

#include <QTime>
#include <QFontDatabase>

const unsigned int TIMER_REFRESH_PER_SECOND = 20;


TimerWidget::TimerWidget(QWidget *parent)
    : QWidget(parent), mTimerLabel(this), mTimer(this), mPauseTimeMs(0)
{
    // Ajout du label dans le widget
    this->setLayout(new QVBoxLayout(this));
    this->layout()->addWidget(&mTimerLabel);

    // Mise en place d'une police custom
    int id = QFontDatabase::addApplicationFont(":/res/fonts/digital-7.ttf");
    if (id != -1)
    {
        QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
        mTimerLabel.setFont(QFont(fontFamily, 32));
    }

    // Sélection d'une couleur
    mTimerLabel.setStyleSheet("QLabel {color: #ee2222}");

    // Configuration du timer de rafraichissement
    mTimer.setSingleShot(false);
    mTimer.setInterval(1000/TIMER_REFRESH_PER_SECOND);
    mTimer.start();
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(refreshTimerDisplay()));

    // Mise à l'origine de l'horloge
    mElapsedTime.start();

    // Configuration initiale du texte
    mTimerLabel.setText("00:00:000");
}

QTime TimerWidget::stop()
{
    mTimer.stop();
    return QTime(0,0,0,0).addMSecs(mElapsedTime.elapsed() - mPauseTimeMs);
}

void TimerWidget::restart()
{
    mElapsedTime.restart();
    mPauseTimeMs = 0;

    mTimer.start();
}

void TimerWidget::pause()
{
    mPauseTimer.restart();
    mTimer.stop();
}

void TimerWidget::resume()
{
    mPauseTimeMs += mPauseTimer.elapsed();
    mTimer.start();
}

void TimerWidget::subTime(int ms)
{
    mPauseTimeMs += ms;
}

void TimerWidget::refreshTimerDisplay()
{
    QTime elapsed = QTime(0,0,0,0).addMSecs(mElapsedTime.elapsed() - mPauseTimeMs);
    QString labelText = QString("%1:%2:%3")
            .arg(elapsed.minute(), 2, 10, QChar('0'))
            .arg(elapsed.second(), 2, 10, QChar('0'))
            .arg(elapsed.msec(), 3, 10, QChar('0'));
    mTimerLabel.setText(labelText);
}
