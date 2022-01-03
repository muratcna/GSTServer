#include "bwidget.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QScreen>

BWidget::BWidget(QWidget *parent)
    : QWidget(parent)
    , mFilePath("")
    , mDecoder(nullptr)
{

    this->setFixedSize(350, 150);

    mPathText = new QLineEdit();
    mPathText->setReadOnly(true);

    mBtnSelectPath = new QPushButton("Select Path");

    mLblIP = new QLabel("Ip");
    mLblPort = new QLabel("Port");
    mIpText = new QLineEdit("224.0.0.1");
    mPortText = new QLineEdit("5000");

    mLblNetworkProtocol = new QLabel("Protocol");
    mLblDecoder = new QLabel("Decoder");
    mLblCast = new QLabel("Cast");

    mCbNetworkProtocol = new QComboBox;
    mCbDecoder = new QComboBox;
    mCbCast = new QComboBox;

    {
        mCbNetworkProtocol->addItem("RTP/UDP");
        mCbNetworkProtocol->addItem("MPEGTS");
    }
    {
        mCbDecoder->addItem("H264");
        mCbDecoder->addItem("MJPEG");
    }
    {
        mCbCast->addItem("UNICAST");
        mCbCast->addItem("MULTICAST");
        mCbCast->addItem("BROADCAST");
    }

    mCheckBox = new QCheckBox("Screen Shot");

    mBtnStart = new QPushButton("Start");
    mBtnStart->setEnabled(false);

    mBtnStop = new QPushButton("Stop");
    mBtnStop->setEnabled(false);

    QVBoxLayout *tVb = new QVBoxLayout;
    {
        {//1.line
            QHBoxLayout *tHb = new QHBoxLayout;
            {
                tHb->addWidget(mPathText);
                tHb->addWidget(mBtnSelectPath);
                tVb->addLayout(tHb);
            }
        }
        {//2.line
            QHBoxLayout *tHb = new QHBoxLayout;
            {
                tHb->addWidget(mLblIP);
                tHb->addWidget(mIpText);
                tHb->addWidget(mLblPort);
                tHb->addWidget(mPortText);
                tHb->addWidget(mCheckBox);
                tVb->addLayout(tHb);
            }
        }
        {//3.line
            QHBoxLayout *tHb = new QHBoxLayout;
            {
                tHb->addWidget(mLblNetworkProtocol);
                tHb->addWidget(mLblDecoder);
                tHb->addWidget(mLblCast);
                tVb->addLayout(tHb);
            }
        }
        {//4.line
            QHBoxLayout *tHb = new QHBoxLayout;
            {
                tHb->addWidget(mCbNetworkProtocol);
                tHb->addWidget(mCbDecoder);
                tHb->addWidget(mCbCast);
                tVb->addLayout(tHb);
            }
        }
        {//5.line
            QHBoxLayout *tHb = new QHBoxLayout;
            {
                tHb->addWidget(mBtnStart);
                tHb->addWidget(mBtnStop);
                tVb->addLayout(tHb);
            }
        }
    }
    setLayout(tVb);

    connect(mBtnStart, &QPushButton::clicked, this, &BWidget::onClickBtnStart,Qt::QueuedConnection);
    connect(mBtnStop, &QPushButton::clicked, this, &BWidget::onClickBtnStop,Qt::QueuedConnection);
    connect(mBtnSelectPath, &QPushButton::clicked, this, &BWidget::onClickBtnSelectPath,Qt::QueuedConnection);
    connect(mCheckBox, &QCheckBox::toggled, this, [this](bool checked){
        if(checked) {
            mPathText->setEnabled(false);
            mBtnSelectPath->setEnabled(false);
        } else {
            mPathText->setEnabled(true);
            mBtnSelectPath->setEnabled(true);
        }
    },Qt::QueuedConnection);

    QTimer *tTimer = new QTimer;
    connect(tTimer, &QTimer::timeout, this, [this](){
        if(mCheckBox->isChecked() || !mDecoder->getIsRunning() && (!mPathText->text().isEmpty()) && (!mIpText->text().isEmpty()) && (!mPortText->text().isEmpty())) {
            mBtnStart->setEnabled(true);
        }  else {
            mBtnStart->setEnabled(false);
        }

        if(!mDecoder->getIsRunning()) {
            mBtnStop->setEnabled(false);
        }

    });
    tTimer->start(100);

    mDecoder = new BDecoder;
}

BWidget::~BWidget()
{
    qDebug() << "~BWidget";
}

void BWidget::onClickBtnStart()
{
    mBtnStart->setEnabled(false);
    mBtnStop->setEnabled(true);

    qDebug() << "Protocol:" << mCbNetworkProtocol->currentText();
    qDebug() << "Decoder:" << mCbDecoder->currentText();
    qDebug() << "Cast:" << mCbCast->currentText();
    qDebug() << "Ip:" << mIpText->text();
    qDebug() << "Port:" << mPortText->text();

    mDecoder->start(mFilePath, mCbDecoder->currentText(), mCbNetworkProtocol->currentText(), mCbCast->currentText(),
                    mIpText->text(), mPortText->text().toInt(), mCheckBox->isChecked());
}

void BWidget::onClickBtnStop()
{
    mBtnStart->setEnabled(true);
    mBtnStop->setEnabled(false);

    mDecoder->waitForStop();
    mDecoder->signalStop();
}

void BWidget::onClickBtnSelectPath()
{
    QString tOpenFileName = QFileDialog::getOpenFileName(this, tr("Choose Video File"), "/", "Video Files (*.mp4)");
    if(!tOpenFileName.isEmpty()) {
        mPathText->setText(tOpenFileName);
        mFilePath = tOpenFileName;
    }
}
