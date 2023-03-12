/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 5 Feb 2023 11:47:06pm
    Author:  Mayhem

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"

// The part after : is only added due to the addition of '&' to the formatManager in header file
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& formatManagerToUse) : formatManager(formatManagerToUse) 
                                                                          //playlistComponent(playlistComponentToUse)                
{}

DJAudioPlayer::~DJAudioPlayer() 
{}

//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)  
{        
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// This one will constantly get called as the maincomponent getNextAudioBlock is constantly called,
// and that in turn calls this
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) 
{
    // No need for transport source as resamplesources wraps around that
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() 
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::toggleLoop(bool shouldLoop)
{   
    readerSource->setLooping(shouldLoop);
    DBG("Loop toggled");
    transportSource.setLooping(shouldLoop);
}


void DJAudioPlayer::loadURL(URL audioURL, double& vidLength) {
    // This step extracts rudimentary numbers from the input
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // This step preps the input stream for the JUCE audio lifecycle    
    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        vidLength = transportSource.getLengthInSeconds();
        transportSource.setGain(0.5);
    }
    else
    {
        DBG("Something went wrong loading file.");
    }
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain>1) {
        DBG("DJAudioPlayer::setGain should be between 0 and 1.");
    }
    else {
        transportSource.setGain(gain);
    }
}

double DJAudioPlayer::getGain() {
    return transportSource.getGain();
}

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio>100) {
        DBG("DJAudioPlayer::setSpeed should be between 0 and 100.");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

double DJAudioPlayer::getSpeed() {
    return resampleSource.getResamplingRatio();
}

void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);    
}

double DJAudioPlayer::getPositionRelative() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos>1) {
        DBG("DJAudioPlayer::setPositionRelative should be between 0 and 1.");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        // Uses the function above
        setPosition(posInSecs);
    }
}


