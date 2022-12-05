/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DCoderAudioProcessor::DCoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    apvts.state.addListener(this);
}

DCoderAudioProcessor::~DCoderAudioProcessor()
{
    apvts.state.removeListener(this);
}

//==============================================================================
const juce::String DCoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DCoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DCoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DCoderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DCoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DCoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DCoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DCoderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DCoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void DCoderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DCoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    midSide.setSamplerate(sampleRate);
    updateParameters();
    reset();
    isActive = true;
}

void DCoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DCoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DCoderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (!isActive)
        return;
    if (mustUpdateProcessing) {
        mustUpdateProcessing = false;
        updateParameters();
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    midSide.processStereoWidth(buffer);
}

//==============================================================================
bool DCoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DCoderAudioProcessor::createEditor()
{
    //return new DCoderAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void DCoderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void DCoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid())
    {
        apvts.replaceState(tree);
        updateParameters();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DCoderAudioProcessor();
}

void DCoderAudioProcessor::userChangedParameter()
{
    mustUpdateProcessing = true;
}

juce::AudioProcessorValueTreeState::ParameterLayout
DCoderAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    // Create your parameters
    //float value returns as a string w a mx length of 4 characters
    std::function<juce::String(float, int)> valueToTextFunction = [](float x, int l) { return juce::String(x, 2); };

    //value to text function
    std::function<float(const juce::String&)> textToValueFunction = [](const juce::String& str) {return str.getFloatValue(); };

    //parameters...


    parameters.push_back(std::make_unique<juce::AudioParameterBool>("MS", "MidSolo", false));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("SS", "SideSolo", false));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("LRIM", "InputMode", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("LROM", "OutputMode", true));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("LRS", "LRSWAP", false));

    //create our parameters for VOL
    parameters.push_back(std::make_unique<juce::AudioParameterFloat >("MG", "MGain", juce::NormalisableRange<float>(-100.0f, 20.0f, 0.1f, 3.0f), 0.0f, "dB", juce::AudioProcessorParameter::genericParameter, valueToTextFunction, textToValueFunction));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat >("SG", "SGain", juce::NormalisableRange<float>(-100.0f, 20.0f, 0.1f, 3.0f), 0.0f, "dB", juce::AudioProcessorParameter::genericParameter, valueToTextFunction, textToValueFunction));


    return { parameters.begin(), parameters.end() };
}

void DCoderAudioProcessor::updateParameters()
{

    //midSide.setStereowidthValue(apvts.getRawParameterValue("STE"));
    //midSide.updateCutFilter(apvts.getRawParameterValue("HPF"));
    bool midBtn = *apvts.getRawParameterValue("MS");
    bool sideBtn = *apvts.getRawParameterValue("SS");

    bool LRInputModeBtn = *apvts.getRawParameterValue("LRIM");
    bool LROutputModeBtn = *apvts.getRawParameterValue("LROM");

    bool LRSwapBtn = *apvts.getRawParameterValue("LRS");

    float mGainSld = *apvts.getRawParameterValue("MG");
    float sGainSld = *apvts.getRawParameterValue("SG");

    midSide.setMidState(midBtn);
    midSide.setSideState(sideBtn);

    midSide.setInputState(LRInputModeBtn);

    midSide.setOutputState(LROutputModeBtn);
    midSide.setSwapState(LRSwapBtn);

    midSide.setMidGain(mGainSld);
    midSide.setSideGain(sGainSld);
}
void DCoderAudioProcessor::reset() {
    midSide.reset();
}