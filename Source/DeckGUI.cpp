/*
  ==============================================================================

    DeckGUI.cpp
    Created: 6 Feb 2023 11:44:26am
    Author:  QC222

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
// Receive the memory address of a dj audio player and assigns it to playerPlaceholder
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse) : playerPlaceholder{ _djAudioPlayer }, 
                                                    waveformDisplay(formatManagerToUse, cacheToUse) // call constructor on waveform
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Adding components and making them visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    // Adds the event listener to itself (the button, hence 'this') instead of some other things
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Set slider range
    volSlider.setRange(0.0, 1.0);
    // Added after crash
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    // Every half a second it'll call timerCallback.
    // This abstract function inherits from Timer
    startTimer(500);

    // By default the values are between 0 and 10.
    //speedSlider.setRange(0.0, 10.0);
}

DeckGUI::~DeckGUI()
{
    /*This function inherits from Timer*/
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{    
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

    // All the following added after crash
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("DeckGUI", getLocalBounds(),
        Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    double rowH = getHeight() / 8;
    playButton.setBounds    (0, 0, getWidth(), rowH);
    stopButton.setBounds    (0, rowH, getWidth(), rowH);
    volSlider.setBounds     (0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds   (0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds     (0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    loadButton.setBounds    (0, rowH * 7, getWidth(), rowH);

}


 //This one is a callback function from the listener 
 //It is an abstract function.
void DeckGUI::buttonClicked(Button* button) {
    if (button == &playButton) {
        // use dbg instead of std::cout on this non-terminal app
        DBG("play button pressed."); 
        playerPlaceholder->start();
    }
    else if(button == &stopButton){
        DBG("stop button pressed.");
        playerPlaceholder->stop();
    }
    else if (button == &loadButton)
    {
        // this does work in 6.1 but the syntax is a little funky
    // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
    // - configure the dialogue
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that. 
        fChooser.launchAsync(fileChooserFlags, 
                            [this](const FileChooser& chooser)
                            {
                                File chosenFile = chooser.getResult();
                                playerPlaceholder->loadURL(URL{ chosenFile });
                                waveformDisplay.loadURL(URL{chosenFile});
                            });
    }
}   

//This one is a callback function from the listener 
 //It is an abstract function.
void DeckGUI::sliderValueChanged (Slider* slider) {
    // if the slider in question is the vol slider
    if (slider == &volSlider) {
        // crucial part that changes the volume of music
        playerPlaceholder->setGain(slider->getValue());
        // if you wish to turn volume slider into frequency slider
        //dphase = volslider.getvalue() * 0.01;
    }
    else if (slider == &speedSlider) {
        playerPlaceholder->setSpeed(slider->getValue());
    }
    else if (slider == &posSlider) {
        playerPlaceholder->setPositionRelative(slider->getValue());
    }
}


bool DeckGUI::isInterestedInFileDrag(const StringArray& files) {
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y) {
    DBG("FilesDrop");
    if (files.size() == 1) {
        // Call the methods in player1 as it has been passed as a pointer to DeckGUI
        playerPlaceholder->loadURL(URL(File{files[0]}));
    }
}

// Set the relative position of waveform depending on the relative position on the DJPlayer's transportSource
// Triggered by the timer
void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(playerPlaceholder->getPositionRelative());
}