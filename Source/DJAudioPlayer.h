/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 5 Feb 2023 11:47:06pm
    Author:  Mayhem

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <string>
#include "PlaylistComponent.h"

class DJAudioPlayer : public AudioSource

{
    public:
        DJAudioPlayer(AudioFormatManager& formatManagerToUse);
                      //PlaylistComponent& playlistComponentToUse);
        ~DJAudioPlayer();

        //=======================AUDIO LIFE CYCLE==========================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        //=======================SLIDERS===================================
        // Sets and returns the current volume of the track respectively
        void setGain(double gain);
        double getGain();

        // Sets and returns the playback speed of the track respectively
        void setSpeed(double ratio);
        double getSpeed();

        // Sets and returns the current playback position of the track respectively
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);        

        //=======================AUXILLIARY FUNCTIONS======================
        // 
        // Start, stops and loops the music
        void start();
        void stop();
        void toggleLoop(bool shouldLoop);

        // Loads the actual file and puts it in transport source
        void loadURL(URL audioURL, double& vidLength);

        /**Get the relative position of the playhead */
        double getPositionRelative();
        bool trackFinished();

        


    private:
        //Main Object that that encapsulates the entire input musical file.
        //Inherits from AudioSource.
        AudioTransportSource transportSource;
        // Another Layer that Wraps Transport Source that can control speed etc.
        // Points to the created transportSource.
        ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
        // Do not create an object - instead point to another object
        AudioFormatManager &formatManager;
        //PlaylistComponent &playlistComponent;
        // A smart pointer that eliminates readerSource when it is out of scope.
        std::unique_ptr<AudioFormatReaderSource> readerSource;

};
