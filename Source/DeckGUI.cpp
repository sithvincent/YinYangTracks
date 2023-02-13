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
                 PlaylistComponent* _playlistComponent,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse, 
                 String _name) : playerPlaceholder{ _djAudioPlayer },
                                playlistComponentPlaceholder{ _playlistComponent },
                                waveformDisplay(formatManagerToUse, cacheToUse, _name), // call constructor on waveform
                                isPlaying(false),
                                vidLength(0),
                                name (_name)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Adding components and making them visible
    //addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playPauseButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(waveformDisplay);


    // Attach label to sliders
    volLabel.setColour(Label::textColourId, juce::Colours::white);
    volLabel.setText("VOLUME", juce::dontSendNotification);
    volLabel.setJustificationType(Justification::centred);
    volLabel.attachToComponent(&volSlider, false);


    addAndMakeVisible(volSlider);

    // Adds the event listener to itself (the button, hence 'this') instead of some other things
    //playButton.addListener(this);
    playPauseButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Set slider range
    volSlider.setRange(0.0f, 1.0f);
    volSlider.setTextValueSuffix(" x");
    if (this->name == "deck2") {
        volSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::lavender);
        volSlider.setColour(Slider::ColourIds::thumbColourId, Colours::turquoise);
        volSlider.setColour(Slider::ColourIds::trackColourId, Colours::deeppink);
    }
    else if (this->name == "deck1") {
        volSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::paleturquoise);
        volSlider.setColour(Slider::ColourIds::thumbColourId, Colours::deeppink);
        volSlider.setColour(Slider::ColourIds::trackColourId, Colours::turquoise);
    }


    speedSlider.setRange(0.1, 10.0);
    if (this->name == "deck2") {
        speedSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::lavender);
        speedSlider.setColour(Slider::ColourIds::thumbColourId, Colours::deeppink);
        speedSlider.setColour(Slider::ColourIds::trackColourId, Colours::deeppink);
    }
    else if (this->name == "deck1") {
        speedSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::paleturquoise);
        speedSlider.setColour(Slider::ColourIds::thumbColourId, Colours::turquoise);
        speedSlider.setColour(Slider::ColourIds::trackColourId, Colours::turquoise);
    }

    posSlider.setRange(0.0, 1.0);



    // Every half a second it'll call timerCallback.
    // This abstract function inherits from Timer
    startTimer(500);

    // Creates the play and pause icons for the play/pause button
    playTriangle.addTriangle(10, 0, 20, 5, 10, 10);
    pauseRectangle.addRoundedRectangle(Rectangle(20,20), 3);

}

DeckGUI::~DeckGUI()
{
    /*This function inherits from Timer*/
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{    
    // clear the background
    //g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.fillAll(Colours::black);

    // Component Outline
    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(), 1);   

    // Paints the Words
    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("DeckGUI", getLocalBounds(), Justification::centred, true);   

    if (isPlaying) {
        playPauseButton.setColours(Colours::deeppink, Colours::mediumvioletred, Colours::mediumvioletred);
        playPauseButton.setShape(pauseRectangle, false, true, true);
    }
    else {
        playPauseButton.setColours(Colours::turquoise, Colours::paleturquoise, Colours::paleturquoise);
        playPauseButton.setShape(playTriangle, false, true, true);
    }
}

void DeckGUI::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    double rowH = getHeight() / 8;
    double columnW = getWidth() / 5;
    double padding = getWidth() / 30;

    // Button locations. Fixed no matter which deck it is in.
    playPauseButton.setBounds (columnW * 2, padding,      columnW, rowH*2-padding*2);
    stopButton.setBounds        (columnW * 2, rowH*2+ padding, columnW, rowH*2 - padding * 2);
    loadButton.setBounds        (columnW * 2, rowH*4 + padding, columnW, rowH*2 - padding * 2);

    // Slider locations - varies depending on the deck it is in.
    if (name == "deck1") {
        volSlider.setBounds(0, rowH, columnW, rowH * 5);
        speedSlider.setBounds(columnW, 0, columnW, rowH * 6);
        posSlider.setBounds(columnW * 3, 0, columnW * 2, rowH * 6);
    }
    else if (name == "deck2") {
        volSlider.setBounds(columnW * 4, rowH, columnW, rowH * 5);
        speedSlider.setBounds(columnW * 3, 0, columnW, rowH * 6);
        posSlider.setBounds(0, 0, columnW * 2, rowH * 6);
    }

    waveformDisplay.setBounds(0, rowH * 6, getWidth(), rowH * 2);
}


 //This one is a callback function from the listener 
 //It is an abstract function.
void DeckGUI::buttonClicked(Button* button) 
{
    if (button == &playPauseButton) {
        if (isPlaying == false) {
            // use dbg instead of std::cout on this non-terminal app
            DBG("play button pressed.");
            playerPlaceholder->start();
            isPlaying = true;
            repaint();
        }
        else if (isPlaying == true) {
            DBG("stop button pressed.");
            playerPlaceholder->stop();
            isPlaying = false;
            repaint();
        }
    }
    else if(button == &stopButton){
        DBG("stop button pressed.");
        playerPlaceholder->stop();
    }
    else if (button == &loadButton)
    {
        // this does work in 6.1 but the syntax is a little funky
        // https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
        // configure the dialogue (an example of lambda function)
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        // launch out of the main thread
        fChooser.launchAsync(fileChooserFlags, 
                            [this](const FileChooser& chooser)
        {
            // Gets the information from the chosen file, particularly its URL
            URL audioURL = URL { chooser.getResult() };

            // Passes the URL to DJAudioPlayer and waveformDisplay to load audio and waveform graphics
            playerPlaceholder->loadURL(audioURL, vidLength);
            waveformDisplay.loadURL(audioURL);

            if (vidLength != 0) {
                // Vector to store the tracktitles
                playlistComponentPlaceholder->addEntry(audioURL.getFileName(), vidLength, audioURL);
                vidLength = 0;
            }
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
        playerPlaceholder->loadURL(URL(File{files[0]}), vidLength);
    }
}

// Set the relative position of waveform depending on the relative position on the DJPlayer's transportSource
// Triggered by the timer
void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(playerPlaceholder->getPositionRelative());
    // If the boolean for the music library button is 'true' meaning that the user wants
    // to load a music from the library onto the deck, then pass the relevant audioURL 
    // into the DJAudioPlayer.
    if (playlistComponentPlaceholder->loadMusicIntoDeck) 
    {
        // If the music library specifies to load the track into the first deck,
        // run following code for the DJAudioPlayer object deck1 (deck2 object won't trigger)
        if (this->name == "deck1" && playlistComponentPlaceholder->deckToLoad == 1)
        {
            playerPlaceholder->loadURL(playlistComponentPlaceholder->urlToLoad, vidLength);
            waveformDisplay.loadURL(playlistComponentPlaceholder->urlToLoad);
            playlistComponentPlaceholder->loadMusicIntoDeck = false;
        }
        // If the music library specifies to load the track into the second deck,
        // run following code for the DJAudioPlayer object deck2 (deck1 object won't trigger)
        else if (this->name == "deck2" && playlistComponentPlaceholder->deckToLoad == 2)
        {
            playerPlaceholder->loadURL(playlistComponentPlaceholder->urlToLoad, vidLength);
            waveformDisplay.loadURL(playlistComponentPlaceholder->urlToLoad);
            playlistComponentPlaceholder->loadMusicIntoDeck = false;
        }
    }
}
