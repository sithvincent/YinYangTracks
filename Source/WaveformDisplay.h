/*
  ==============================================================================

    WavefromDisplay.h
    Created: 7 Feb 2023 8:37:51pm
    Author:  Vincent Chong Yi Qin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*Creates the methods to draw a waveform display from given input.*/
class WaveformDisplay : public juce::Component,
                        public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    // Have to include this as it is a pure virtual function, but it has no subcomponent
    // so not necessary to implement.
    void resized() override;
    // Get audio info from incoming audio thumbnail
    void loadURL(URL audioURL);
    /*Sets actual position of playhead by calling the repaint function.*/
    void setPositionRelative(double pos);

    /* Inherited from Change Listener class. This function needed as otherwise the 
    waveform would only load if you resize window due to running on a different 'thread'
    than the main Audio Player. Relevant on initial fileload.*/
    void changeListenerCallback(ChangeBroadcaster* source) override;


private:
    // Main object responsible for drawing of the visualizer
    AudioThumbnail audioThumbnail;
    bool fileLoaded;
    double position;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};

