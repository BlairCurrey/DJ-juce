#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (944, 600);

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
        setAudioChannels (2, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

}
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    //playlistComponent.setBounds(0, 0, getWidth() / 3, getHeight());
    //deckGUI1.setBounds(getWidth() / 3, 0, 2 * getWidth() / 3, getHeight() / 2);
    //deckGUI2.setBounds(getWidth() / 3, getHeight() / 2, 2 * getWidth() / 3, getHeight() / 2);
    int columns = 100;
    auto playlistRight = 28 * getWidth() / columns;
    playlistComponent.setBounds(0, 0, playlistRight, getHeight());
    deckGUI1.setBounds(playlistRight, 0, getWidth() - playlistRight, getHeight() / 2);
    deckGUI2.setBounds(playlistRight, getHeight() / 2, getWidth() - playlistRight, getHeight() / 2);

    //getWidth() - getWidth() / columns - getHeight() / 4
    //deckGUI1.setBounds(playlistRight, 0, getWidth() - playlistRight - getHeight() / 4, getHeight() / 2);
    //deckGUI2.setBounds(playlistRight, getHeight() / 2, getWidth() - playlistRight - getHeight() / 4, getHeight() / 2);
}