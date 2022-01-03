#ifndef BDECODER_H
#define BDECODER_H

#include <gst/gst.h>

#include <string>
#include <thread>

#include <QObject>

// TODO: is it necesary to be qobject?
class BDecoder
{
public:
    BDecoder();
    ~BDecoder();

    void start(QString filePath, QString decoder, QString protocol, QString network, QString ip, int port, bool isScreenShot);
    void signalStop();
    void waitForStop();

    bool getIsRunning() const;

private:
    GstElement* pipeline;

    std::thread t;
    bool isRunning;
};

#endif // BDECODER_H
