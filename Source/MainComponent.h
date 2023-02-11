#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================


// Here, our self-defined main component also gains the public functions/variables in multiple other classes\
// AudioAppComponent inherits Component class
class MainComponent  :  public juce::AudioAppComponent
{                      
    public:
        //==============================================================================
        MainComponent();
        ~MainComponent() override;

        //=============================AUDIO THREAD===============================
        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        //============================= GUI THREAD ===============================
        void paint (juce::Graphics& g) override;
        void resized() override;


    private:

        //=============================AUDIO THREAD===============================
        
        // To interpret audio file formats
        AudioFormatManager formatManager;

        // Create AudioThumbnail (the soundwave) objects. 
        // It runs a single background thread that is shared by all the thumbnails that need it.
        AudioThumbnailCache thumbCache{ 100 };

        // DJAudio Player object (main)
        DJAudioPlayer player1{ formatManager };
        DJAudioPlayer player2 { formatManager };

        // Allows the playing of two tracks simultaneously (combines audio data of player 1 and 2)
        MixerAudioSource mixerSource;


        //============================= GUI THREAD ===============================
        // Takes the reference to a DJAudioPlayer object, the format manager, and audio thumbnail
        DeckGUI deck1{&player1, formatManager, thumbCache };
        DeckGUI deck2{&player2, formatManager, thumbCache };

        // Playlist Component
        PlaylistComponent playlistComponent;

        // It assigns a delete operation to the copy constructor.
        // this means that if you start passing this object around, you will have 
        // problems as the copy constructed is called when you pass things into functions as non-references.
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)



       /* Random rand;
        double phase;
        double dphase;*/
};
