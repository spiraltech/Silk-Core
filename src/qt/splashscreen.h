// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_QT_SPLASHSCREEN_H
#define SILK_QT_SPLASHSCREEN_H

#include <QSplashScreen>

class CWallet;
class NetworkStyle;

/** Class for the splashscreen with information of the running client.
 *
 * @note this is intentionally not a QSplashScreen. Silk Core initialization
 * can take a long time, and in that case a progress window that cannot be
 * moved around and minimized has turned out to be frustrating to the user.
 */
class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(Qt::WindowFlags f, const NetworkStyle *networkStyle);
    ~SplashScreen();

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    /** Slot to call finish() method as it's not defined as slot */
    void slotFinish(QWidget *mainWin);

    /** Show message and progress */
    void showMessage(const QString &message, int alignment, const QColor &color);

private:
    /** Connect core signals to splash screen */
    void subscribeToCoreSignals();
    /** Disconnect core signals to splash screen */
    void unsubscribeFromCoreSignals();
    /** Connect wallet signals to splash screen */
    void ConnectWallet(CWallet*);

    QPixmap pixmap;
    QString curMessage;
    QColor curColor;
    int curAlignment;


    QList<CWallet*> connectedWallets;
};

#endif // SILK_QT_SPLASHSCREEN_H
