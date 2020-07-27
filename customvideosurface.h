#ifndef CUSTOMVIDEOSURFACE_H
#define CUSTOMVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <sndfile.hh>

class CustomVideoSurface : public QAbstractVideoSurface
{
public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool present(const QVideoFrame &frame);

    void SetAudioFileOutput(QString &audioFileName, int oversampleCount);


private:
    int frameCounter = 0;
    int oversampleLoops = 0;
    SndfileHandle *sndFile = nullptr;
};



#endif // CUSTOMVIDEOSURFACE_H
