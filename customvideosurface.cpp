#include "customvideosurface.h"

#include <iostream>


void CustomVideoSurface::SetAudioFileOutput(QString &audioFileName, int oversampleCount)
{
    //audioOutputFileName = audioFileName;

    oversampleLoops = oversampleCount;
    int channels = 2;
    int samplerate = 48000;

    sndFile = new SndfileHandle(audioFileName.toStdString(), SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, channels, samplerate);

}

QList<QVideoFrame::PixelFormat> CustomVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);

    // Return the formats you will support
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB565;
}

bool CustomVideoSurface::present(const QVideoFrame &frame)
{
    if (!frame.isValid())
    {
        // Close the stream.
        delete sndFile;

        return true;
    }

    // Something weird with init of the private class variable.  It's random.  Almost like C lib init isn't happening.
    // Possibly something in Qt?  Shouldn't happen.
    if (frameCounter > 10000 || frameCounter < 0)
            frameCounter = 0;
    else
        frameCounter++;

    QVideoFrame mFrame = frame;
    //Q_UNUSED(frame);
    // Handle the frame and do your processing
    mFrame.map(QAbstractVideoBuffer::ReadOnly);
    //std::cout << "-----------------" << std::endl;
    //std::cout << "mFrame width:" << mFrame.width() << std::endl;
    //std::cout << "mFrame height:" << mFrame.height() << std::endl;
    //std::cout << "mFrame bytesPerLine:" << mFrame.bytesPerLine() << std::endl;
    //std::cout << "mFrame isValid:" << mFrame.isValid() << std::endl;
    //std::cout << "mFrame planeCount:" << mFrame.planeCount() << std::endl;

    std::cout << "FrameCount:" << frameCounter << std::endl;

    //std::cout << "mFrame bits():" << mFrame.bits() << std::endl;

    // Do a pixel by pixel dump and
    int x = mFrame.width();
    int y = mFrame.height();
    int bpl = mFrame.bytesPerLine();

    uchar *rawbits = mFrame.bits();

    // allocate enough for a worst case.  We nominally won't use all of this unless the screen is pure black.
    int16_t *buffer = new int16_t[x*y*2];
    int output_counter = 0;

    for(int yi = 0; yi < y; yi++)
    {
        for(int xi = 0; xi < x; xi++)
        {
            int16_t pixel = (rawbits[(yi*bpl)+(xi<<1)]);
            int16_t red = ((pixel & 0xF800)>>11);
            int16_t green = ((pixel & 0x07E0)>>5);
            int16_t blue = (pixel & 0x001F);
            int16_t grayscale = (0.2126 * red) + (0.7152 * green / 2.0) + (0.0722 * blue);

            if (grayscale > 1)
            {
                // Emite white
                // std::cout << " ";
            }
            else
            {
                // emit black
                // std::cout << ".";
                // Ok, so this is about scaling a pixel value (if it's visible) to a set of audio coordinates that range form -32k to + 32k.
                // meaning:
                // if we have a pixel at 0,0 it would be at x:-32k, y:32k
                // and if we have a pixel at n,m (max video dimensions) it would be at x:32k, y:-32k.
                // so our math looks like:
                // audio L channel or X coord in phase seperation = -32768 + ((x+1)/width * 65535)
                // audio R channel or Y coord in phase seperation = 65536 - ((y+1)/height * 65535)


                buffer[output_counter] = (int16_t) (-32768.0 + ((((float)xi+1)/(float)x) * 65536.0));
                buffer[output_counter+1] = (int16_t) (32768.0 - ((((float)yi+1)/(float)y) * 65536.0));
                //std::cout << "X: " << buffer[output_counter] << std::endl;
                //std::cout << "Y: " << buffer[output_counter+1] << std::endl;

                output_counter += 2;
            }
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;

    // Write the output buffer 5 times so it will catch most frame/duration setttings.
    for(int q = 0; q < oversampleLoops; q++)
        sndFile->write( buffer, output_counter);

    mFrame.unmap();
    return true;
}

