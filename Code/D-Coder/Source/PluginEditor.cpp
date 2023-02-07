/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DCoderAudioProcessorEditor::DCoderAudioProcessorEditor (DCoderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    verticalGradientMeterM([&]() {return audioProcessor.midSide.getRMSLevelMid(); }),
    verticalGradientMeterS([&]() {return audioProcessor.midSide.getRMSLevelSide(); })
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setSize (1383,255);

    bgImage = ImageCache::getFromMemory(BinaryData::Background_raw_png, BinaryData::Background_raw_pngSize);
    //bgImage = ImageCache::getFromMemory(BinaryData::render_png, BinaryData::render_pngSize);
    grill = ImageCache::getFromMemory(BinaryData::grill_withoutWorld_png, BinaryData::grill_withoutWorld_pngSize);

    midLableImage = ImageCache::getFromMemory(BinaryData::mid_lables_png, BinaryData::mid_lables_pngSize);
    sideLableImage = ImageCache::getFromMemory(BinaryData::side_lables_png, BinaryData::side_lables_pngSize);
    lineLableImage = ImageCache::getFromMemory(BinaryData::lines_lables_png, BinaryData::lines_lables_pngSize);
    generalLableImage = ImageCache::getFromMemory(BinaryData::general_lables_png, BinaryData::general_lables_pngSize);

    addAndMakeVisible(verticalGradientMeterM);
    addAndMakeVisible(verticalGradientMeterS);


    addButtons();

    addRotaries();

}

DCoderAudioProcessorEditor::~DCoderAudioProcessorEditor()
{
}

//==============================================================================
void DCoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.drawImage(bgImage, getLocalBounds().toFloat());

}


void DCoderAudioProcessorEditor::paintOverChildren(juce::Graphics& g)
{
    g.drawImageAt(generalLableImage, 0, 0);
    g.drawImageAt(lineLableImage, 0, 0);
    g.drawImageAt(midLableImage, 0, 0);
    g.drawImageAt(sideLableImage, 0, -3);
    g.drawImageAt(grill, 0, 0);

}

void DCoderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    verticalGradientMeterM.setBounds(1229, 37, 17, 181);
    verticalGradientMeterS.setBounds(1283, 37, 17, 181);
}

void DCoderAudioProcessorEditor::addButtons() {

    //------MID SOLO-----
    midSoloImage = ImageCache::getFromMemory(BinaryData::midSolo_btn_png, BinaryData::midSolo_btn_pngSize);
    midSoloLightImage = ImageCache::getFromMemory(BinaryData::midSolo_light_btn_png, BinaryData::midSolo_light_btn_pngSize);
    midSoloBtn.setBounds(274, 20, 52, 30);
    addAndMakeVisible(midSoloBtn);
    midSoloBtn.setImages(false, false, false, midSoloImage, 1.0f, {}, midSoloImage, 1.0f, {}, midSoloLightImage, 1.0f, {}, {});
    midSoloBtn.setClickingTogglesState(true);
    midSoloBtn.setMouseCursor(MouseCursor::PointingHandCursor);
    audioProcessor.midSide.setMidState(&midSoloBtn);
    midSoloAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MS", midSoloBtn);


    
    //------SIDE SOLO-----
    sideSoloImage = ImageCache::getFromMemory(BinaryData::sideSolo_btn_png, BinaryData::sideSolo_btn_pngSize);
    sideSoloLightImage = ImageCache::getFromMemory(BinaryData::sideSolo_light_btn_png, BinaryData::sideSolo_light_btn_pngSize);
    sideSoloBtn.setBounds(275, 150, 48, 29);
    addAndMakeVisible(sideSoloBtn);
    sideSoloBtn.setClickingTogglesState(true);
    sideSoloBtn.setImages(true, false, false, sideSoloImage, 1.0f, {}, sideSoloImage, 1.0f, {}, sideSoloLightImage, 1.0f, {}, {});
    sideSoloBtn.setMouseCursor(MouseCursor::PointingHandCursor);
    audioProcessor.midSide.setSideState(&sideSoloBtn);
    sideSoloAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "SS", sideSoloBtn);



    //-----INPUT MODE-----
    inImage = ImageCache::getFromMemory(BinaryData::in_btn_png, BinaryData::in_btn_pngSize);
    inLightImage = ImageCache::getFromMemory(BinaryData::in_light_btn_png, BinaryData::in_light_btn_pngSize);
    inBtn.setBounds(1147, 34, 68, 89);
    addAndMakeVisible(inBtn);
    inBtn.setClickingTogglesState(true);
    inBtn.setImages(true, false, false, inImage, 1.0f, {}, inImage, 1.0f, {}, inLightImage, 1.0f, {}, {});
    inBtn.setMouseCursor(MouseCursor::PointingHandCursor);
    audioProcessor.midSide.setInputState(&inBtn);
    inAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "LRIM", inBtn);



    //-----OUTPUT MODE-----
    outImage = ImageCache::getFromMemory(BinaryData::out_btn_png, BinaryData::out_btn_pngSize);
    outLightImage = ImageCache::getFromMemory(BinaryData::out_light_btn_png, BinaryData::out_light_btn_pngSize);
    outBtn.setBounds(1144, 165, 64, 51);
    addAndMakeVisible(outBtn);
    outBtn.setClickingTogglesState(true);
    outBtn.setImages(true, false, false, outImage, 1.0f, {}, outImage, 1.0f, {}, outLightImage, 1.0f, {}, {});
    outBtn.setMouseCursor(MouseCursor::PointingHandCursor);
    audioProcessor.midSide.setOutputState(&outBtn);
    outAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "LROM", outBtn);



    //-----SWAP-----
    swapImage = ImageCache::getFromMemory(BinaryData::swap_png, BinaryData::swap_pngSize);
    swapLightImage = ImageCache::getFromMemory(BinaryData::swapLight_png, BinaryData::swapLight_pngSize);
    swapBtn.setBounds(1142, 113, 67, 34);
    addAndMakeVisible(swapBtn);
    swapBtn.setClickingTogglesState(true);
    swapBtn.setImages(true, false, false, swapImage, 1.0f, {}, swapImage, 1.0f, {}, swapLightImage, 1.0f, {}, {});
    swapBtn.setMouseCursor(MouseCursor::PointingHandCursor);
    audioProcessor.midSide.setSwapState(&swapBtn);
    swapAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "LRS", swapBtn);
}

void DCoderAudioProcessorEditor::addRotaries() {
    
    //Licht: 5684.8 W
    //----------------MID--------------------------------------------------------------------------------------------------
    //Ratio
    ratioKnob.sprite = ImageCache::getFromMemory(BinaryData::ratio_strip_v2_png, BinaryData::ratio_strip_v2_pngSize);
    ratioKnob.customSkew = 1;
    
    ratioSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    
    ratioSlider->setBounds(383, 8, 131, 112);

    ratioSlider->setLookAndFeel(&ratioKnob);
    ratioSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(ratioSlider.get());
    ratioAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Ratio", *ratioSlider);

    //Attack
    attackKnob.sprite = ImageCache::getFromMemory(BinaryData::attack_strip_png, BinaryData::attack_strip_pngSize);
    attackKnob.customSkew = 0.6f;

    attackSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    attackSlider->setBounds(514, 8, 143, 112);
    attackSlider->setLookAndFeel(&attackKnob);
    attackSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(attackSlider.get());
    attackAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Attack", *attackSlider);

    //Release
    releaseKnob.sprite = ImageCache::getFromMemory(BinaryData::release_strip_png, BinaryData::release_strip_pngSize);
    releaseKnob.customSkew = 0.55f;

    releaseSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    releaseSlider->setBounds(666, 7, 142, 113);
    releaseSlider->setLookAndFeel(&releaseKnob);
    releaseSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(releaseSlider.get());
    releaseAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Release", *releaseSlider);

    //Threshold
    thresholdKnob.sprite = ImageCache::getFromMemory(BinaryData::threshold_strip_png, BinaryData::threshold_strip_pngSize);
    thresholdKnob.customSkew = 1.f;

    thresholdSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    thresholdSlider->setBounds(808, 7, 160, 113);
    thresholdSlider->setLookAndFeel(&thresholdKnob);
    thresholdSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(thresholdSlider.get());
    thresholdAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "Threshold", *thresholdSlider);

    //MidGain
    midGainKnob.sprite = ImageCache::getFromMemory(BinaryData::midGain_strip_png, BinaryData::midGain_strip_pngSize);
    midGainKnob.customSkew = 1.3;

    midGainSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    midGainSlider->setBounds(999, 9, 131, 115);
    midGainSlider->setLookAndFeel(&midGainKnob);
    midGainSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(midGainSlider.get());
    midGainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MG", *midGainSlider);


    //----------------SIDE--------------------------------------------------------------------------------------------------

    ////LowCut
    lcKnob.sprite = ImageCache::getFromMemory(BinaryData::lowCut_strip_png, BinaryData::lowCut_strip_pngSize);
    lcKnob.customSkew = 0.3;

    lcSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    lcSlider->setBounds(347, 134, 137, 114);//347,137
    lcSlider->setLookAndFeel(&lcKnob);
    lcSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(lcSlider.get());
    lcAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LCFreq", *lcSlider);

    ////EQMidGain
    mGKnob.sprite = ImageCache::getFromMemory(BinaryData::eqMidGain_strip_png, BinaryData::eqMidGain_strip_pngSize);
    mGKnob.customSkew = 1;

    mGSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    mGSlider->setBounds(483, 133, 130, 115);//483,136
    mGSlider->setLookAndFeel(&mGKnob);
    mGSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(mGSlider.get());
    mGAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PGain", *mGSlider);

    ////EQ Mid Freq
    mFreqKnob.sprite = ImageCache::getFromMemory(BinaryData::eqMidFreq_strip_png, BinaryData::eqMidFreq_strip_pngSize);
    mFreqKnob.customSkew = 0.4;

    mFreqSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    mFreqSlider->setBounds(605, 133, 133, 115);//605,136
    mFreqSlider->setLookAndFeel(&mFreqKnob);
    mFreqSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(mFreqSlider.get());
    mFreqAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PFreq", *mFreqSlider);

    ////EQ Mid Q
    mQKnob.sprite = ImageCache::getFromMemory(BinaryData::eqMidQ_strip_png, BinaryData::eqMidQ_strip_pngSize);
    mQKnob.customSkew = 1;

    mQSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    mQSlider->setBounds(728, 133, 130, 116); //728,136
    mQSlider->setLookAndFeel(&mQKnob);
    mQSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(mQSlider.get());
    mQAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PQuality", *mQSlider);

    ////HighCut
    hcKnob.sprite = ImageCache::getFromMemory(BinaryData::highCut_strip_png, BinaryData::highCut_strip_pngSize);
    hcKnob.customSkew = 0.6f;

    hcSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    hcSlider->setBounds(854, 134, 166, 116);//854, 137
    hcSlider->setLookAndFeel(&hcKnob);
    hcSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(hcSlider.get());
    hcAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HCFreq", *hcSlider);

    //SideGain
    sideGainKnob.sprite = ImageCache::getFromMemory(BinaryData::sideGain_strip_png, BinaryData::sideGain_strip_pngSize);
    sideGainKnob.customSkew = 1.3;

    sideGainSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
    sideGainSlider->setBounds(996, 135, 150, 115);//996,138
    sideGainSlider->setLookAndFeel(&sideGainKnob);
    sideGainSlider->setMouseCursor(MouseCursor::PointingHandCursor);
    addAndMakeVisible(sideGainSlider.get());
    sideGainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SG", *sideGainSlider);
}