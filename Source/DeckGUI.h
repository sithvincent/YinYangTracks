
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
        DeckGUI(DJAudioPlayer* _djAudioPlayer, 
            PlaylistComponent* _playlistComponent,
            AudioFormatManager& formatManagerToUse, 
            AudioThumbnailCache& cacheToUse,
            String _name);
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

        // Stores the name of the deck (ie. Deck 1 or Deck 2)
        String name;

        // Creates all the Button and Slider objects
        TextButton restartButton{ "RESTART" };
        TextButton loadButton{ "LOAD" };
        TextButton loopButton{ "LOOP" };
        ShapeButton playPauseButton{"PLAY", Colours::turquoise, Colours::paleturquoise, Colours::paleturquoise };
        Slider volSlider { Slider::SliderStyle::LinearVertical , Slider::TextEntryBoxPosition::TextBoxAbove };
        Slider speedSlider{ Slider::SliderStyle::LinearVertical , Slider::TextEntryBoxPosition::TextBoxAbove };;
        Slider posSlider{ Slider::SliderStyle::Rotary  , Slider::TextEntryBoxPosition::NoTextBox };;

        // Creates the labels attached to the sliders
        Label volLabel;
        Label speedLabel;
        Label posLabel;

        // Creates the shapes for the custom playbutton that changes shape depending on the status of the track 
        Path playTriangle;
        Path pauseRectangle;

        // Creates the booleans that can control the behaviour of the deck depending on whether the music is
        // playing, looping or a track is loaded into the deck
        bool isPlaying;
        bool isLooping;
        bool isLoaded;

        // Stores the length of the input audio file in seconds
        double vidLength;

        // We don't create a player, just a pointer to a player
        DJAudioPlayer* playerPlaceholder;


        // We don't create a player, just a pointer to a player
        PlaylistComponent* playlistComponentPlaceholder;

        /* Allows User to Select a File */
        FileChooser fChooser{ "Select a file..." };

        // Create a waveformdisplay item
        WaveformDisplay waveformDisplay;



        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
