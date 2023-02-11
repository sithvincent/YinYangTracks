
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*Inherits Component (allowing it to add components) & Listeners*/
class DeckGUI  :public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
    public:
        // A pointer to a player is passed into DeckGUI
        DeckGUI(DJAudioPlayer* _djAudioPlayer, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse);
        ~DeckGUI() ;

        // Painting and resizing
        void paint (Graphics&) override;
        void resized() override;

        // Implement Listener Methods
        void buttonClicked(juce::Button*) override;
        void sliderValueChanged(juce::Slider*) override;

        // Drag and drop    
        bool isInterestedInFileDrag(const StringArray& files) override;
        void filesDropped(const StringArray& files, int x, int y) override;

        /*This abstract function inherits from Timer*/
        void timerCallback() override;

    private:
        // Buttons and Sliders
        TextButton playButton{ "PLAY" };
        TextButton stopButton{ "STOP" };
        TextButton loadButton{ "LOAD" };
        Slider volSlider;
        Slider speedSlider;
        Slider posSlider;

        // We don't create a player, just a pointer to a player
        DJAudioPlayer* playerPlaceholder;

        /* Allows User to Select a File */
        FileChooser fChooser{ "Select a file..." };

        // Create a waveformdisplay item
        WaveformDisplay waveformDisplay;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
