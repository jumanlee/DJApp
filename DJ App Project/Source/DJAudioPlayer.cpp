
#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{
    
}

//this is the birth of the cycle
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //tell the transport source to get ready
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

//this is being called again and again. This is life of the cycle
void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

//this is death of the cycle
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    //check for validity
    if(reader != nullptr) //good file!
    {
    //second layer of the onion. This takes a low level file reader which can take numbers out of audio files and wraps up with audio file life cycle we can use it as an audio source. Unique pointer means only can have one owner.
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));

        //then pass it to the transport source
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);

        //pass ownership of unique pointer to class scope variable.
        readerSource.reset (newSource.release());
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if(gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGai gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
    
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if(ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if(pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioOlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds(); //divide to make it a relative position 0 to 1
}

double DJAudioPlayer::getPositionSeconds()
{
    return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getLengthSeconds()
{
    return transportSource.getLengthInSeconds();
}

//set readerSource to loop/repeat the track
void DJAudioPlayer::setLooping(bool shouldLoop)
{
    if(readerSource != nullptr)
    {
        readerSource->setLooping(shouldLoop);
    }
}
