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


    // Phase method necessary to So we could remember where we were in the synthesised 
    // waveform between calls to getNextAudioBlock. This allows us to avoid discontinuities
    // phase = 0;
    // dphase = 0.0001;
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

void DJAudioPlayer::loadURL(URL audioURL) {
    // This step extracts rudimentary numbers from the input
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // This step preps the input stream for the JUCE audio lifecycle    
    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

        // Pushes the information about the file into playlist's vectors
 /*       playlistComponent.addEntry(audioURL.getFileName(),
                                    transportSource.getLengthInSeconds());
        playlistComponent.printTitles();*/
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

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio>100) {
        DBG("DJAudioPlayer::setSpeed should be between 0 and 100.");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
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


double getSongLength(AudioTransportSource& transportSource) {
    return transportSource.getLengthInSeconds();
}


// IF YOU ARE SYNTHESIZING YOUR OWN WEIRD MUSIC
//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    // Your audio-processing code goes here!
//
//    // If not producing any data, we need to clear the buffer (to prevent the output of random noise)
//    // bufferToFill.clearActiveBufferRegion();
//
//    // Create the sounds based on random values for the left earphone
//    auto* leftChan = bufferToFill.buffer->getWritePointer(0, 
//                                                         bufferToFill.startSample);
//    // for the right earphone
//    auto* rightChan = bufferToFill.buffer->getWritePointer(0,
//                                                          bufferToFill.startSample);
//    for (auto i = 0; i < bufferToFill.numSamples; ++i) {
//        // Moderate the signal strength by 0.25. Randomize the values.
//        // double sample = rand.nextDouble()*0.25;
//        // double sample = fmod (phase, 0.2);
//        double sample = sin(phase) * 0.1;
//        leftChan[i] = sample;
//        rightChan[i] = sample;
//        // Increases every 'i' but is reset to 0 when it hits 0.2
//        DBG("Sample value is: "<<sample);
//        phase += dphase;
//    }
//}