// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SILK_QT_RECEIVECOINSDIALOG_H
#define SILK_QT_RECEIVECOINSDIALOG_H

#include "guiutil.h"

#include <QDialog>
#include <QHeaderView>
#include <QItemSelection>
#include <QKeyEvent>
#include <QMenu>
#include <QPoint>
#include <QVariant>

class OptionsModel;
class WalletModel;

namespace Ui {
    class ReceiveCoinsDialog;
}

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Dialog for requesting payment of silks */
class ReceiveCoinsDialog : public QDialog
{
    Q_OBJECT

public:
    enum ColumnWidths {
        DATE_COLUMN_WIDTH = 130,
        ADDRESS_COLUMN_WIDTH = 160,
        LABEL_COLUMN_WIDTH = 120,
        AMOUNT_MINIMUM_COLUMN_WIDTH = 160,
        MINIMUM_COLUMN_WIDTH = 120
    };

    explicit ReceiveCoinsDialog(QWidget *parent = 0);
    ~ReceiveCoinsDialog();

    void setModel(WalletModel *model);

public slots:
    void clear();
    void reject();
    void accept();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::ReceiveCoinsDialog *ui;
    GUIUtil::TableViewLastColumnResizingFixer *columnResizingFixer;
    WalletModel *model;
    QMenu *contextMenu;
    QModelIndex selectedRow();
    void copyColumnToClipboard(int column);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void on_receiveButton_clicked();
    void on_showRequestButton_clicked();
    void on_removeRequestButton_clicked();
    void on_recentRequestsView_doubleClicked(const QModelIndex &index);
    void recentRequestsView_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void updateDisplayUnit();
    void showMenu(const QPoint &point);
    void copyAddress();
    void copyURI();
    void copyLabel();
    void copyMessage();
    void copyAmount();
};

#endif // SILK_QT_RECEIVECOINSDIALOG_H
