#include "MainComponent.h"

//=============================== MAIN =====================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    // The individual elements are made visible inside deckGUIs already, 
    // so what's left is to make deckGUIs visible
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(playlistComponent);

    // Create a format manager (which interprets different audio formats) for all classes
    formatManager.registerBasicFormats();

    // Component and listener adding used to be done here
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}


//=============================AUDIO THREAD===============================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    // It will be called on the audio thread, not the GUI thread.
    
    // Have to specify what audio objects to add together here.
    // addInputSource replaces having to call prepareToPlay for individual players
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Here we will fill up the buffer with mixed audio instead of from just one player
    mixerSource.getNextAudioBlock(bufferToFill);
    // Thus we will comment out the following code: player1.getNextAudioBlock(bufferToFill);
}

// This will be called when the audio device stops, or when it is being
// restarted due to a setting change.
void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}


//============================= GUI THREAD ===============================

void MainComponent::paint (juce::Graphics& g)
{
    // Our component is opaque, so we must completely fill the background with a solid colour
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.  
    deck1.setBounds(0, 0, getWidth() / 2, getHeight()/2);
    deck2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight()/2);
    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);

    // Individual buttons and sliders used to be sized here
}

// Event listener and button click used to be implemented here




