#ifndef BWIDGET_H
#define BWIDGET_H

#include "bdecoder.h"

#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class BWidget : public QWidget
{
    Q_OBJECT

public:
    BWidget(QWidget *parent = 0);
    ~BWidget();

    QString filePath() const;
    void setFilePath(const QString &filePath);

protected slots:
    void onClickBtnStart();
    void onClickBtnStop();
    void onClickBtnSelectPath();

protected:
    QLineEdit *mPathText;

    QLabel *mLblIP;
    QLabel *mLblPort;
    QLineEdit *mIpText;
    QLineEdit *mPortText;

    QPushButton *mBtnSelectPath;
    QPushButton *mBtnStop;
    QPushButton *mBtnStart;

    QLabel *mLblNetworkProtocol;
    QLabel *mLblDecoder;
    QLabel *mLblCast;

    QComboBox *mCbDecoder;
    QComboBox *mCbNetworkProtocol;
    QComboBox *mCbCast;

    QCheckBox *mCheckBox;

private:
    QString mFilePath;
    BDecoder *mDecoder;

};

#endif // BWIDGET_H
