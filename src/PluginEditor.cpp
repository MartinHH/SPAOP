/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SpaopAudioProcessorEditor::SpaopAudioProcessorEditor (SpaopAudioProcessor* ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    addAndMakeVisible (displayGroup = new GroupComponent ("display group",
                                                          TRANS("Display")));

    addAndMakeVisible (parametersGroup = new GroupComponent ("parameters group",
                                                             TRANS("Source parameters")));

    addAndMakeVisible (sourceGroup = new GroupComponent ("source group",
                                                         TRANS("Source selection")));

    addAndMakeVisible (connectionGroup = new GroupComponent ("connection group",
                                                             TRANS("Connection")));

    addAndMakeVisible (xSlider = new Slider ("xCoord"));
    xSlider->setRange (-100, 100, 0.01);
    xSlider->setSliderStyle (Slider::LinearHorizontal);
    xSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 60, 20);
    xSlider->addListener (this);

    addAndMakeVisible (ySlider = new Slider ("yCoord"));
    ySlider->setRange (-100, 100, 0.01);
    ySlider->setSliderStyle (Slider::LinearHorizontal);
    ySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 60, 20);
    ySlider->addListener (this);

    addAndMakeVisible (idSlider = new Slider ("ID"));
    idSlider->setRange (0, 63, 1);
    idSlider->setSliderStyle (Slider::IncDecButtons);
    idSlider->setTextBoxStyle (Slider::TextBoxRight, false, 40, 20);
    idSlider->addListener (this);

    addAndMakeVisible (idLabel = new Label ("id-Label",
                                            TRANS("ID:")));
    idLabel->setFont (Font (15.00f, Font::plain));
    idLabel->setJustificationType (Justification::centred);
    idLabel->setEditable (false, false, false);
    idLabel->setColour (TextEditor::textColourId, Colours::black);
    idLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (angleLabel = new Label ("angle label",
                                               TRANS("Angle:")));
    angleLabel->setFont (Font (15.00f, Font::plain));
    angleLabel->setJustificationType (Justification::centredLeft);
    angleLabel->setEditable (false, false, false);
    angleLabel->setColour (TextEditor::textColourId, Colours::black);
    angleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (dopplerButton = new ToggleButton ("dopler button"));
    dopplerButton->setButtonText (TRANS("Doppler effect"));
    dopplerButton->addListener (this);

    addAndMakeVisible (lockIDButton = new ToggleButton ("lock id button"));
    lockIDButton->setButtonText (TRANS("Lock"));
    lockIDButton->addListener (this);

    addAndMakeVisible (linkWonderButton = new ToggleButton ("link wonder button"));
    linkWonderButton->setButtonText (TRANS("Link to WONDER"));
    linkWonderButton->addListener (this);

    addAndMakeVisible (zoomSlider = new Slider ("zoom slider"));
    zoomSlider->setRange (1, 50, 0.1);
    zoomSlider->setSliderStyle (Slider::LinearHorizontal);
    zoomSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    zoomSlider->addListener (this);

    addAndMakeVisible (yPosLabel = new Label ("yPos label",
                                              TRANS("yPos:")));
    yPosLabel->setFont (Font (15.00f, Font::plain));
    yPosLabel->setJustificationType (Justification::centredLeft);
    yPosLabel->setEditable (false, false, false);
    yPosLabel->setColour (TextEditor::textColourId, Colours::black);
    yPosLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (xPosLabel = new Label ("xPos label",
                                              TRANS("xPos:")));
    xPosLabel->setFont (Font (15.00f, Font::plain));
    xPosLabel->setJustificationType (Justification::centredLeft);
    xPosLabel->setEditable (false, false, false);
    xPosLabel->setColour (TextEditor::textColourId, Colours::black);
    xPosLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (zoomLabel = new Label ("zoom label",
                                              TRANS("Zoom:")));
    zoomLabel->setFont (Font (15.00f, Font::plain));
    zoomLabel->setJustificationType (Justification::centredLeft);
    zoomLabel->setEditable (false, false, false);
    zoomLabel->setColour (TextEditor::textColourId, Colours::black);
    zoomLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (addrLabel = new Label ("addr label",
                                              TRANS("Address:")));
    addrLabel->setFont (Font (15.00f, Font::plain));
    addrLabel->setJustificationType (Justification::centredLeft);
    addrLabel->setEditable (false, false, false);
    addrLabel->setColour (TextEditor::textColourId, Colours::black);
    addrLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (urlLabel = new Label ("url label",
                                             TRANS("??????")));
    urlLabel->setFont (Font (15.00f, Font::plain));
    urlLabel->setJustificationType (Justification::centredLeft);
    urlLabel->setEditable (false, false, false);
    urlLabel->setColour (TextEditor::textColourId, Colours::black);
    urlLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (cStatusLabel = new Label ("c-status label",
                                                 TRANS("Connection status")));
    cStatusLabel->setFont (Font (15.00f, Font::plain));
    cStatusLabel->setJustificationType (Justification::centredLeft);
    cStatusLabel->setEditable (false, false, false);
    cStatusLabel->setColour (TextEditor::textColourId, Colours::black);
    cStatusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (nameEditor = new TextEditor ("name editor"));
    nameEditor->setMultiLine (false);
    nameEditor->setReturnKeyStartsNewLine (false);
    nameEditor->setReadOnly (false);
    nameEditor->setScrollbarsShown (true);
    nameEditor->setCaretVisible (true);
    nameEditor->setPopupMenuEnabled (true);
    nameEditor->setText (String::empty);

    addAndMakeVisible (nameLabel = new Label ("name label",
                                              TRANS("Name:")));
    nameLabel->setFont (Font (15.00f, Font::plain));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (TextEditor::textColourId, Colours::black);
    nameLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (typeButton = new ToggleButton ("type button"));
    typeButton->setButtonText (TRANS("Type is point"));
    typeButton->addListener (this);
    typeButton->setToggleState (true, dontSendNotification);

    addAndMakeVisible (colourButton = new TextButton ("colour button"));
    colourButton->setButtonText (TRANS("Colour"));
    colourButton->addListener (this);
    colourButton->setColour (TextButton::buttonColourId, Colours::red);

    addAndMakeVisible (showOthersButton = new ToggleButton ("show others button"));
    showOthersButton->setButtonText (TRANS("Show other sources"));
    showOthersButton->addListener (this);
    showOthersButton->setToggleState (true, dontSendNotification);

    addAndMakeVisible (sourceZoomPort = new wonderjuce::SourceZoomPort ("source zoomport"));

    addAndMakeVisible (angleSlider = new Slider ("angle slider"));
    angleSlider->setRange (0, 360, 0.1);
    angleSlider->setSliderStyle (Slider::Rotary);
    angleSlider->setTextBoxStyle (Slider::TextBoxAbove, false, 60, 20);
    angleSlider->setColour (Slider::backgroundColourId, Colour (0x00000000));
    angleSlider->setColour (Slider::thumbColourId, Colour (0x00bbbbff));
    angleSlider->setColour (Slider::trackColourId, Colour (0x00ffffff));
    angleSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x00ffffff));
    angleSlider->setColour (Slider::rotarySliderOutlineColourId, Colours::black);
    angleSlider->addListener (this);


    //[UserPreSize]
    wonderjuce::SourcePanel* sourcePanel = sourceZoomPort->getSourcePanel();
    sourcePanel->addListener(this);
    sourcePanel->setSources(ownerFilter->getSourceController()->getSources());
    sourcePanel->setRoom(ownerFilter->getSourceController()->getRoom());

    nameEditor->addListener(this);
    nameIsBeingEdited_ = false;

    //[/UserPreSize]

    setSize (720, 490);


    //[Constructor] You can add your own custom stuff here..

    // Within WONDER, 0.0 degree angle is 3 o'clock, while for the slider,
    // 0.0 is 12 o'clock. So the rotary slider's start and end need to be
    // adjusted. Also, we don't want it to stop at end:
    angleSlider->setRotaryParameters(float_Pi * 0.5, float_Pi * 2.5, false);

    idSlider->setRange(0, MAX_WONDER_SOURCES-1, 1);

    zoomSlider->setValue(ownerFilter->getZoomFactor());
    sourceZoomPort->setZoomFactor(ownerFilter->getZoomFactor());

    startTimer(100);
    //[/Constructor]
}

SpaopAudioProcessorEditor::~SpaopAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    displayGroup = nullptr;
    parametersGroup = nullptr;
    sourceGroup = nullptr;
    connectionGroup = nullptr;
    xSlider = nullptr;
    ySlider = nullptr;
    idSlider = nullptr;
    idLabel = nullptr;
    angleLabel = nullptr;
    dopplerButton = nullptr;
    lockIDButton = nullptr;
    linkWonderButton = nullptr;
    zoomSlider = nullptr;
    yPosLabel = nullptr;
    xPosLabel = nullptr;
    zoomLabel = nullptr;
    addrLabel = nullptr;
    urlLabel = nullptr;
    cStatusLabel = nullptr;
    nameEditor = nullptr;
    nameLabel = nullptr;
    typeButton = nullptr;
    colourButton = nullptr;
    showOthersButton = nullptr;
    sourceZoomPort = nullptr;
    angleSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SpaopAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff787878));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpaopAudioProcessorEditor::resized()
{
    displayGroup->setBounds (464, 392, 240, 88);
    parametersGroup->setBounds (464, 80, 240, 168);
    sourceGroup->setBounds (464, 8, 240, 64);
    connectionGroup->setBounds (464, 256, 240, 128);
    xSlider->setBounds (112, 408, 100, 71);
    ySlider->setBounds (280, 408, 100, 71);
    idSlider->setBounds (520, 32, 96, 24);
    idLabel->setBounds (480, 32, 32, 24);
    angleLabel->setBounds (624, 136, 56, 24);
    dopplerButton->setBounds (480, 136, 112, 24);
    lockIDButton->setBounds (632, 32, 56, 24);
    linkWonderButton->setBounds (480, 280, 208, 24);
    zoomSlider->setBounds (552, 448, 136, 24);
    yPosLabel->setBounds (256, 456, 48, 24);
    xPosLabel->setBounds (88, 456, 48, 24);
    zoomLabel->setBounds (480, 448, 48, 24);
    addrLabel->setBounds (480, 312, 64, 24);
    urlLabel->setBounds (552, 312, 136, 24);
    cStatusLabel->setBounds (480, 344, 200, 24);
    nameEditor->setBounds (552, 104, 136, 24);
    nameLabel->setBounds (480, 104, 56, 24);
    typeButton->setBounds (480, 168, 112, 24);
    colourButton->setBounds (480, 208, 104, 24);
    showOthersButton->setBounds (480, 416, 208, 24);
    sourceZoomPort->setBounds (24, 16, 432, 432);
    angleSlider->setBounds (624, 168, 56, 72);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SpaopAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == xSlider)
    {
        //[UserSliderCode_xSlider] -- add your slider handling code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::xPosParam,
                                (float) normalizedValue(sliderThatWasMoved) );
        //[/UserSliderCode_xSlider]
    }
    else if (sliderThatWasMoved == ySlider)
    {
        //[UserSliderCode_ySlider] -- add your slider handling code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::yPosParam,
                                (float) normalizedValue(sliderThatWasMoved) );
        //[/UserSliderCode_ySlider]
    }
    else if (sliderThatWasMoved == idSlider)
    {
        //[UserSliderCode_idSlider] -- add your slider handling code here..
        getProcessor()->getSourceController()->setID( (int) sliderThatWasMoved->getValue() );
        //[/UserSliderCode_idSlider]
    }
    else if (sliderThatWasMoved == zoomSlider)
    {
        //[UserSliderCode_zoomSlider] -- add your slider handling code here..
        getProcessor()->setZoomFactor(zoomSlider->getValue());
        //[/UserSliderCode_zoomSlider]
    }
    else if (sliderThatWasMoved == angleSlider)
    {
        //[UserSliderCode_angleSlider] -- add your slider handling code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::angleParam,
                                (float) normalizedValue(sliderThatWasMoved) );
        //[/UserSliderCode_angleSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SpaopAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == dopplerButton)
    {
        //[UserButtonCode_dopplerButton] -- add your button handler code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::dopplParam,
                                (float) normalizedValue(buttonThatWasClicked) );
        //[/UserButtonCode_dopplerButton]
    }
    else if (buttonThatWasClicked == lockIDButton)
    {
        //[UserButtonCode_lockIDButton] -- add your button handler code here..
        getSourceController()->setIdIsLocked(lockIDButton->getToggleState());
        //[/UserButtonCode_lockIDButton]
    }
    else if (buttonThatWasClicked == linkWonderButton)
    {
        //[UserButtonCode_linkWonderButton] -- add your button handler code here..
        getSourceController()->setLinkedToWonder(linkWonderButton->getToggleState());
        //[/UserButtonCode_linkWonderButton]
    }
    else if (buttonThatWasClicked == typeButton)
    {
        //[UserButtonCode_typeButton] -- add your button handler code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::typeParam,
                                                   (float) normalizedValue(buttonThatWasClicked) );
        //[/UserButtonCode_typeButton]
    }
    else if (buttonThatWasClicked == colourButton)
    {
        //[UserButtonCode_colourButton] -- add your button handler code here..
        showColourSelector();
        //[/UserButtonCode_colourButton]
    }
    else if (buttonThatWasClicked == showOthersButton)
    {
        //[UserButtonCode_showOthersButton] -- add your button handler code here..
        getProcessor()->setShowOtherSources(showOthersButton->getToggleState());
        //[/UserButtonCode_showOthersButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

SpaopAudioProcessor* SpaopAudioProcessorEditor::getProcessor() const
{
    return static_cast <SpaopAudioProcessor*>(getAudioProcessor());
}

wonder::SourceController* SpaopAudioProcessorEditor::getSourceController() const
{
    return getProcessor()->getSourceController();
}

void SpaopAudioProcessorEditor::timerCallback()
{
    wonder::SourceController* controller = getSourceController();
    const wonder::Source& source = controller->getSource();

    // Buttons:
    colourButton->setColour(TextButton::buttonColourId, Colour(source.getRed(),
                                                               source.getGreen(),
                                                               source.getBlue()));

    // Sliders:
    idSlider->setRange(0, MAX_WONDER_SOURCES, 1);
    // TODO: what happens if current value is out of range?
    idSlider->setValue(source.getID());
    idSlider->setEnabled(!controller->isLinkedToWonder() && !controller->idIsLocked());

    xSlider->setValue(source.getXPos());
    ySlider->setValue(source.getYPos());
    angleSlider->setValue(source.getAngle());
    zoomSlider->setValue(getProcessor()->getZoomFactor());

    // ToggleButtons:
    dopplerButton->setToggleState(source.dopplerIsEnabled(), dontSendNotification);
    linkWonderButton->setToggleState(controller->isLinkedToWonder(), dontSendNotification);
    lockIDButton->setToggleState(controller->idIsLocked(), dontSendNotification);
    typeButton->setToggleState(source.getType() == wonder::Source::point,
                                 dontSendNotification);
    showOthersButton->setToggleState(getProcessor()->showOtherSources(),
                                     dontSendNotification);

    const bool isPlaneSource = source.getType() == wonder::Source::plane;
    angleLabel->setEnabled(isPlaneSource);
    angleSlider->setEnabled(isPlaneSource);

    if(!nameIsBeingEdited_){
        nameEditor->setText(source.getName());
        nameIsBeingEdited_ = false;
    }

    sourceZoomPort->setZoomFactor(getProcessor()->getZoomFactor());

    wonderjuce::SourcePanel* sourcePanel = sourceZoomPort->getSourcePanel();
    sourcePanel->setSource(source.getID());
    sourcePanel->setShowOtherSources(getProcessor()->showOtherSources());

    urlLabel->setText(controller->getDataDestHostAndPort(), dontSendNotification);

    cStatusLabel->setText(controller->connectionStatusString(), dontSendNotification);
    cStatusLabel->setColour(Label::textColourId, connectionColour(controller->connectionStatus()));

}

void SpaopAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster *changeBroadcaster)
{
    ColourSelector* colourSelector = dynamic_cast <ColourSelector*> (changeBroadcaster);
    getSourceController()->updateSourceColour(wonderColour(colourSelector->getCurrentColour()));
}

void SpaopAudioProcessorEditor::sourcePanelValuesChanged(wonderjuce::SourcePanel *panel)
{
    if(panel == sourceZoomPort->getSourcePanel())
    {
        AudioProcessor* processor = getProcessor();

        processor->setParameterNotifyingHost(wonder::Source::xPosParam,
                                             panel->getXPos());

        processor->setParameterNotifyingHost(wonder::Source::yPosParam,
                                             panel->getYPos());
    }
}

void SpaopAudioProcessorEditor::textEditorTextChanged(juce::TextEditor &textEditor)
{
    nameIsBeingEdited_ = true;
}

void SpaopAudioProcessorEditor::textEditorReturnKeyPressed(TextEditor &textEditor)
{
    if (&textEditor == nameEditor) {
        getProcessor()->getSourceController()->updateSourceName(textEditor.getText().toStdString());
        nameIsBeingEdited_ = false;
    }
}

void SpaopAudioProcessorEditor::textEditorFocusLost(TextEditor &textEditor)
{
    if (&textEditor == nameEditor) {
        getProcessor()->getSourceController()->updateSourceName(textEditor.getText().toStdString());
        nameIsBeingEdited_ = false;
    }
}

double SpaopAudioProcessorEditor::normalizedValue(Slider* slider)
{
    return NORMALIZE(slider->getMinimum(), slider->getMaximum(), slider->getValue());
}

double SpaopAudioProcessorEditor::normalizedValue(Button* button)
{
    return button->getToggleState() ? 1.0 : 0.0;
}

Colour SpaopAudioProcessorEditor::connectionColour(wonder::ConnectionStates cStatus)
{
    switch (cStatus)
    {
        case wonder::inactive:  return Colours::grey;
        case wonder::active:    return Colours::green;
        case wonder::error:     return Colours::red;
    }
    return Colours::red;
}

Colour SpaopAudioProcessorEditor::juceColour(wonder::Colour colour)
{
    return Colour(colour.r, colour.g, colour.b);
}

wonder::Colour SpaopAudioProcessorEditor::wonderColour(Colour colour)
{
    return wonder::Colour(colour.getRed(), colour.getGreen(), colour.getBlue());
}

void SpaopAudioProcessorEditor::showColourSelector()
{
    ColourSelector* colourSelector = new ColourSelector(ColourSelector::showColourAtTop
                                                      | ColourSelector::showColourspace);
    colourSelector->addChangeListener(this);
    colourSelector->setCurrentColour(juceColour(getProcessor()->getSourceController()
                                                ->getSource().getCoulour()));
    colourSelector->setSize(COLOUR_SELECTOR_SIZE, COLOUR_SELECTOR_SIZE);

    CallOutBox::launchAsynchronously(colourSelector, colourButton->getScreenBounds(),
                                     this);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpaopAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer, public wonderjuce::SourcePanel::Listener, public TextEditor::Listener, public ChangeListener"
                 constructorParams="SpaopAudioProcessor* ownerFilter" variableInitialisers="AudioProcessorEditor(ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="720" initialHeight="490">
  <BACKGROUND backgroundColour="ff787878"/>
  <GROUPCOMPONENT name="display group" id="e866147c42bab00a" memberName="displayGroup"
                  virtualName="" explicitFocusOrder="0" pos="464 392 240 88" title="Display"/>
  <GROUPCOMPONENT name="parameters group" id="c7f1afc9060fd53a" memberName="parametersGroup"
                  virtualName="" explicitFocusOrder="0" pos="464 80 240 168" title="Source parameters"/>
  <GROUPCOMPONENT name="source group" id="3afb455592111222" memberName="sourceGroup"
                  virtualName="" explicitFocusOrder="0" pos="464 8 240 64" title="Source selection"/>
  <GROUPCOMPONENT name="connection group" id="75c4d4aef7a76922" memberName="connectionGroup"
                  virtualName="" explicitFocusOrder="0" pos="464 256 240 128" title="Connection"/>
  <SLIDER name="xCoord" id="a58b89fa9741293f" memberName="xSlider" virtualName=""
          explicitFocusOrder="0" pos="112 408 100 71" min="-100" max="100"
          int="0.010000000000000000208" style="LinearHorizontal" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="yCoord" id="26d1d3281f771894" memberName="ySlider" virtualName=""
          explicitFocusOrder="0" pos="280 408 100 71" min="-100" max="100"
          int="0.010000000000000000208" style="LinearHorizontal" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="ID" id="f54640cb2a0de1c" memberName="idSlider" virtualName=""
          explicitFocusOrder="0" pos="520 32 96 24" min="0" max="63" int="1"
          style="IncDecButtons" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="id-Label" id="afff56522e02a6b2" memberName="idLabel" virtualName=""
         explicitFocusOrder="0" pos="480 32 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="ID:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="angle label" id="8a9f126e40ff7f4a" memberName="angleLabel"
         virtualName="" explicitFocusOrder="0" pos="624 136 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Angle:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="dopler button" id="9e0fbaca7f4346bc" memberName="dopplerButton"
                virtualName="" explicitFocusOrder="0" pos="480 136 112 24" buttonText="Doppler effect"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="lock id button" id="535f93f6213af54c" memberName="lockIDButton"
                virtualName="" explicitFocusOrder="0" pos="632 32 56 24" buttonText="Lock"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="link wonder button" id="a50c2401bf941894" memberName="linkWonderButton"
                virtualName="" explicitFocusOrder="0" pos="480 280 208 24" buttonText="Link to WONDER"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="zoom slider" id="2e49c59dd84b651a" memberName="zoomSlider"
          virtualName="" explicitFocusOrder="0" pos="552 448 136 24" min="1"
          max="50" int="0.10000000000000000555" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="yPos label" id="8e38e3d5ea7a7958" memberName="yPosLabel"
         virtualName="" explicitFocusOrder="0" pos="256 456 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="yPos:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="xPos label" id="81e02301812e4e4" memberName="xPosLabel"
         virtualName="" explicitFocusOrder="0" pos="88 456 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="xPos:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="zoom label" id="4e82c19758586a72" memberName="zoomLabel"
         virtualName="" explicitFocusOrder="0" pos="480 448 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Zoom:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="addr label" id="c9c821ac478f8e2f" memberName="addrLabel"
         virtualName="" explicitFocusOrder="0" pos="480 312 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Address:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="url label" id="2ae8e48f7526ed29" memberName="urlLabel"
         virtualName="" explicitFocusOrder="0" pos="552 312 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="??????" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="c-status label" id="5f890d1c193d8cb5" memberName="cStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="480 344 200 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Connection status" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="name editor" id="6e4508e19b9ddb85" memberName="nameEditor"
              virtualName="" explicitFocusOrder="0" pos="552 104 136 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="name label" id="33500429033ecfa7" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="480 104 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="type button" id="b02d6765cac6f73e" memberName="typeButton"
                virtualName="" explicitFocusOrder="0" pos="480 168 112 24" buttonText="Type is point"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TEXTBUTTON name="colour button" id="9494ad3896b90b69" memberName="colourButton"
              virtualName="" explicitFocusOrder="0" pos="480 208 104 24" bgColOff="ffff0000"
              buttonText="Colour" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="show others button" id="33f3e82300ebf110" memberName="showOthersButton"
                virtualName="" explicitFocusOrder="0" pos="480 416 208 24" buttonText="Show other sources"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <VIEWPORT name="source zoomport" id="addc41de081c2973" memberName="sourceZoomPort"
            virtualName="wonderjuce::SourceZoomPort" explicitFocusOrder="0"
            pos="24 16 432 432" vscroll="1" hscroll="1" scrollbarThickness="18"
            contentType="0" jucerFile="" contentClass="wonderjuce::SourcePanel"
            constructorParams=""/>
  <SLIDER name="angle slider" id="611360eafe6fd8e7" memberName="angleSlider"
          virtualName="" explicitFocusOrder="0" pos="624 168 56 72" bkgcol="0"
          thumbcol="bbbbff" trackcol="ffffff" rotarysliderfill="ffffff"
          rotaryslideroutline="ff000000" min="0" max="360" int="0.10000000000000000555"
          style="Rotary" textBoxPos="TextBoxAbove" textBoxEditable="1"
          textBoxWidth="60" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
