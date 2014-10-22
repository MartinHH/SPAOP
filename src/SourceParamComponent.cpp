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

#include "SourceParamComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SourceParamComponent::SourceParamComponent (SpaopAudioProcessor* ownerFilter)
    : wonderjuce::SpaopEditorComponent(ownerFilter),
      nameIsBeingEdited_(false)
{
    addAndMakeVisible (parametersGroup = new GroupComponent ("parameters group",
                                                             TRANS("Source parameters")));

    addAndMakeVisible (sourceGroup = new GroupComponent ("source group",
                                                         TRANS("Source selection")));

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

    addAndMakeVisible (angleSlider = new Slider ("angle slider"));
    angleSlider->setRange (0, 360, 0.1);
    angleSlider->setSliderStyle (Slider::Rotary);
    angleSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 60, 20);
    angleSlider->setColour (Slider::backgroundColourId, Colour (0x00000000));
    angleSlider->setColour (Slider::thumbColourId, Colour (0x00bbbbff));
    angleSlider->setColour (Slider::trackColourId, Colour (0x00ffffff));
    angleSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x00ffffff));
    angleSlider->setColour (Slider::rotarySliderOutlineColourId, Colours::black);
    angleSlider->addListener (this);


    //[UserPreSize]

    // Within WONDER, 0.0 degree angle is 3 o'clock, while for the slider,
    // 0.0 is 12 o'clock. So the rotary slider's start and end need to be
    // adjusted. Also, we don't want it to stop at end:
    angleSlider->setRotaryParameters(float_Pi * 0.5, float_Pi * 2.5, false);

    // set idSlider range
    idSlider->setRange(0, MAX_WONDER_SOURCES-1, 1);

    nameEditor->addListener(this);
    //[/UserPreSize]

    setSize (256, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SourceParamComponent::~SourceParamComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    parametersGroup = nullptr;
    sourceGroup = nullptr;
    idSlider = nullptr;
    idLabel = nullptr;
    angleLabel = nullptr;
    dopplerButton = nullptr;
    lockIDButton = nullptr;
    nameEditor = nullptr;
    nameLabel = nullptr;
    typeButton = nullptr;
    colourButton = nullptr;
    angleSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SourceParamComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SourceParamComponent::resized()
{
    parametersGroup->setBounds (0, 88, 240, 224);
    sourceGroup->setBounds (0, 8, 240, 64);
    idSlider->setBounds (56, 32, 96, 24);
    idLabel->setBounds (16, 32, 32, 24);
    angleLabel->setBounds (16, 224, 56, 24);
    dopplerButton->setBounds (16, 144, 112, 24);
    lockIDButton->setBounds (168, 32, 56, 24);
    nameEditor->setBounds (88, 112, 136, 24);
    nameLabel->setBounds (16, 112, 56, 24);
    typeButton->setBounds (16, 176, 112, 24);
    colourButton->setBounds (24, 272, 192, 24);
    angleSlider->setBounds (88, 208, 152, 56);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SourceParamComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == idSlider)
    {
        //[UserSliderCode_idSlider] -- add your slider handling code here..
        getProcessor()->setSourceID((int) sliderThatWasMoved->getValue());
        //[/UserSliderCode_idSlider]
    }
    else if (sliderThatWasMoved == angleSlider)
    {
        //[UserSliderCode_angleSlider] -- add your slider handling code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::angleParam,
            (float) SpaopAudioProcessorEditor::normalizedValue(sliderThatWasMoved) );
        //[/UserSliderCode_angleSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SourceParamComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == dopplerButton)
    {
        //[UserButtonCode_dopplerButton] -- add your button handler code here..
        getProcessor()->setParameterNotifyingHost(wonder::Source::dopplParam,
            (float) SpaopAudioProcessorEditor::normalizedValue(buttonThatWasClicked) );
        //[/UserButtonCode_dopplerButton]
    }
    else if (buttonThatWasClicked == lockIDButton)
    {
        //[UserButtonCode_lockIDButton] -- add your button handler code here..
        if (lockIDButton->getToggleState()) {
            getProcessor()->lockID();
        } else {
            getProcessor()->unlockID();
        }
        //[/UserButtonCode_lockIDButton]
    }
    else if (buttonThatWasClicked == typeButton)
    {
        //[UserButtonCode_typeButton] -- add your button handler code here..
        getProcessor()->setParameterNotifyingHost (wonder::Source::typeParam,
            (float) SpaopAudioProcessorEditor::normalizedValue(buttonThatWasClicked) );
        //[/UserButtonCode_typeButton]
    }
    else if (buttonThatWasClicked == colourButton)
    {
        //[UserButtonCode_colourButton] -- add your button handler code here..
        showColourSelector();
        //[/UserButtonCode_colourButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SourceParamComponent::update()
{
    const wonder::Source& source = getProcessor()->getSource();

    // Buttons:
    colourButton->setColour(TextButton::buttonColourId, Colour(source.getRed(),
                                                               source.getGreen(),
                                                               source.getBlue()));
    // Sliders:
    idSlider->setRange(0, MAX_WONDER_SOURCES, 1);
    // TODO: what happens if current value is out of range?
    idSlider->setValue(source.getID());
    idSlider->setEnabled(!getProcessor()->idIsLocked());
    angleSlider->setValue(source.getAngle());

    // ToggleButtons:
    dopplerButton->setToggleState(source.dopplerIsEnabled(), dontSendNotification);
    lockIDButton->setToggleState(getProcessor()->idIsLocked(), dontSendNotification);
    typeButton->setToggleState(source.getType() == wonder::Source::point,
                               dontSendNotification);

    // plane source only elements:
    const bool isPlaneSource = source.getType() == wonder::Source::plane;
    angleLabel->setEnabled(isPlaneSource);
    angleSlider->setEnabled(isPlaneSource);

    if(!nameIsBeingEdited_){
        nameEditor->setText(source.getName(), false);
    }

}

void SourceParamComponent::showColourSelector()
{
    ColourSelector* colourSelector = new ColourSelector(ColourSelector::showColourAtTop
                                                        | ColourSelector::showColourspace);
    colourSelector->addChangeListener(this);

    const wonder::Colour c = getProcessor()->getSource().getCoulour();
    colourSelector->setCurrentColour(SpaopAudioProcessorEditor::juceColour(c));
    colourSelector->setSize(COLOUR_SELECTOR_SIZE, COLOUR_SELECTOR_SIZE);

    CallOutBox::launchAsynchronously(colourSelector, colourButton->getScreenBounds(),
                                     this);
}

void SourceParamComponent::changeListenerCallback(ChangeBroadcaster *changeBroadcaster)
{
    ColourSelector* colourSelector = dynamic_cast <ColourSelector*> (changeBroadcaster);
    const wonder::Colour c = SpaopAudioProcessorEditor::wonderColour(colourSelector->getCurrentColour());
    getProcessor()->getSourceController()->updateSourceColour(getProcessor()->getSourceID(), c);
}

void SourceParamComponent::textEditorTextChanged(TextEditor &textEditor)
{
    nameIsBeingEdited_ = true;
}

void SourceParamComponent::textEditorReturnKeyPressed(TextEditor &textEditor)
{
    if (&textEditor == nameEditor && nameIsBeingEdited_) {
        getProcessor()->getSourceController()->updateSourceName(getProcessor()->getSourceID(),
                                                                textEditor.getText().toStdString());
        nameIsBeingEdited_ = false;
    }
}

void SourceParamComponent::textEditorFocusLost(TextEditor &textEditor)
{
    if (&textEditor == nameEditor && nameIsBeingEdited_) {
        getProcessor()->getSourceController()->updateSourceName(getProcessor()->getSourceID(),
                                                                textEditor.getText().toStdString());
        nameIsBeingEdited_ = false;
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SourceParamComponent" componentName=""
                 parentClasses="public wonderjuce::SpaopEditorComponent, public TextEditor::Listener, public ChangeListener"
                 constructorParams="SpaopAudioProcessor* ownerFilter" variableInitialisers="wonderjuce::SpaopEditorComponent(ownerFilter),&#10;nameIsBeingEdited_(false)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="256" initialHeight="400">
  <BACKGROUND backgroundColour="ffff"/>
  <GROUPCOMPONENT name="parameters group" id="c7f1afc9060fd53a" memberName="parametersGroup"
                  virtualName="" explicitFocusOrder="0" pos="0 88 240 224" title="Source parameters"/>
  <GROUPCOMPONENT name="source group" id="3afb455592111222" memberName="sourceGroup"
                  virtualName="" explicitFocusOrder="0" pos="0 8 240 64" title="Source selection"/>
  <SLIDER name="ID" id="f54640cb2a0de1c" memberName="idSlider" virtualName=""
          explicitFocusOrder="0" pos="56 32 96 24" min="0" max="63" int="1"
          style="IncDecButtons" textBoxPos="TextBoxRight" textBoxEditable="1"
          textBoxWidth="40" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="id-Label" id="afff56522e02a6b2" memberName="idLabel" virtualName=""
         explicitFocusOrder="0" pos="16 32 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="ID:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
  <LABEL name="angle label" id="8a9f126e40ff7f4a" memberName="angleLabel"
         virtualName="" explicitFocusOrder="0" pos="16 224 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Angle:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="dopler button" id="9e0fbaca7f4346bc" memberName="dopplerButton"
                virtualName="" explicitFocusOrder="0" pos="16 144 112 24" buttonText="Doppler effect"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="lock id button" id="535f93f6213af54c" memberName="lockIDButton"
                virtualName="" explicitFocusOrder="0" pos="168 32 56 24" buttonText="Lock"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTEDITOR name="name editor" id="6e4508e19b9ddb85" memberName="nameEditor"
              virtualName="" explicitFocusOrder="0" pos="88 112 136 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="name label" id="33500429033ecfa7" memberName="nameLabel"
         virtualName="" explicitFocusOrder="0" pos="16 112 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="type button" id="b02d6765cac6f73e" memberName="typeButton"
                virtualName="" explicitFocusOrder="0" pos="16 176 112 24" buttonText="Type is point"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <TEXTBUTTON name="colour button" id="9494ad3896b90b69" memberName="colourButton"
              virtualName="" explicitFocusOrder="0" pos="24 272 192 24" bgColOff="ffff0000"
              buttonText="Colour" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="angle slider" id="611360eafe6fd8e7" memberName="angleSlider"
          virtualName="" explicitFocusOrder="0" pos="88 208 152 56" bkgcol="0"
          thumbcol="bbbbff" trackcol="ffffff" rotarysliderfill="ffffff"
          rotaryslideroutline="ff000000" min="0" max="360" int="0.10000000000000000555"
          style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="60"
          textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
