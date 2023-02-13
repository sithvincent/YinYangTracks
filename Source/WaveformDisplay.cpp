
#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
// Constructor Function - audio thumbnail initialized with values and fileloaded set to false
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse, 
                                 AudioThumbnailCache& cacheToUse,
                                 String _attachedDeck) :
                                 audioThumbnail(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded (false),
                                 position(0),
                                 attachedDeck(_attachedDeck)
{
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(Colours::black);   // clear the background
    // draw an outline around the component
    g.setColour(Colours::white);
    g.drawRect(getLocalBounds(), 1);
    // Change the colour of the waveform column if a file is loaded
    if (fileLoaded) {
        if (this->attachedDeck == "deck1") {
            g.setColour(juce::Colours::turquoise);
        }
        else if (this->attachedDeck == "deck2") {
            g.setColour(juce::Colours::deeppink);
        }
        // Draws the actual waveform display
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1.0f);
        // Draw the moving rectangle (this is the item that will constantly get changed during repaint)
        if (this->attachedDeck == "deck1") {
            g.setColour(juce::Colours::deeppink);
        }
        else if (this->attachedDeck == "deck2") {
            g.setColour(juce::Colours::turquoise);
        }
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    // Else just fill it up as per normal
    else {        
        g.setFont(24.0f);
        g.drawText("File Not Loaded!", getLocalBounds(),
            Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumbnail.clear();
    // Note that the new here means that a pointer is sent to setSource and memory is dynamically allocated
    // setSource returns true if the file is loaded, false if something goes wrong. 
    // No worry about file leaks because source will be deleted if no longer needed
    fileLoaded = audioThumbnail.setSource(new URLInputSource(audioURL));

    // This wasn't typed in yesterday.
  /*  if (fileLoaded)
    {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    }
    else {
        std::cout << "wfd: not loaded! " << std::endl;
    }*/
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    // Originally was just if (pos != position) which triggered the error
    if (pos != position && pos > 0) {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::resized()
{

}
