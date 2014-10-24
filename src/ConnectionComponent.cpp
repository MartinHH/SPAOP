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

#include "ConnectionComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ConnectionComponent::ConnectionComponent (SpaopAudioProcessor* ownerFilter)
    : wonderjuce::SpaopEditorComponent(ownerFilter),
      cwAddressIsBeingEdited_(false)
{
    addAndMakeVisible (infoGroupComponent = new GroupComponent ("new group",
                                                                TRANS("Connection info")));

    addAndMakeVisible (syncGroupComponent = new GroupComponent ("sync group",
                                                                TRANS("Sync")));

    addAndMakeVisible (cwGroupComponent = new GroupComponent ("cwonder group",
                                                              TRANS("cWONDER address")));

    addAndMakeVisible (cwIpEditor = new TextEditor ("cwonder ip editor"));
    cwIpEditor->setMultiLine (false);
    cwIpEditor->setReturnKeyStartsNewLine (false);
    cwIpEditor->setReadOnly (false);
    cwIpEditor->setScrollbarsShown (true);
    cwIpEditor->setCaretVisible (true);
    cwIpEditor->setPopupMenuEnabled (true);
    cwIpEditor->setText (TRANS("???.???.???.???"));

    addAndMakeVisible (cwPortEditor = new TextEditor ("cwonder port editor"));
    cwPortEditor->setMultiLine (false);
    cwPortEditor->setReturnKeyStartsNewLine (false);
    cwPortEditor->setReadOnly (false);
    cwPortEditor->setScrollbarsShown (true);
    cwPortEditor->setCaretVisible (true);
    cwPortEditor->setPopupMenuEnabled (true);
    cwPortEditor->setText (TRANS("?????"));

    addAndMakeVisible (ipLabel = new Label ("ip label",
                                            TRANS("IP:")));
    ipLabel->setFont (Font (15.00f, Font::plain));
    ipLabel->setJustificationType (Justification::centredLeft);
    ipLabel->setEditable (false, false, false);
    ipLabel->setColour (TextEditor::textColourId, Colours::black);
    ipLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (portLabel = new Label ("port label",
                                              TRANS("Port:")));
    portLabel->setFont (Font (15.00f, Font::plain));
    portLabel->setJustificationType (Justification::centredLeft);
    portLabel->setEditable (false, false, false);
    portLabel->setColour (TextEditor::textColourId, Colours::black);
    portLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (confirmCwButton = new TextButton ("confirm cwonder button"));
    confirmCwButton->setButtonText (TRANS("Confirm changes"));
    confirmCwButton->addListener (this);

    addAndMakeVisible (cStatusLabel = new Label ("c-status label",
                                                 TRANS("Connection status")));
    cStatusLabel->setFont (Font (15.00f, Font::plain));
    cStatusLabel->setJustificationType (Justification::centredLeft);
    cStatusLabel->setEditable (false, false, false);
    cStatusLabel->setColour (TextEditor::textColourId, Colours::black);
    cStatusLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (syncMasterButton = new TextButton ("sync master button"));
    syncMasterButton->setButtonText (TRANS("Sync as master"));
    syncMasterButton->addListener (this);

    addAndMakeVisible (syncSlaveButton = new TextButton ("sync slave button"));
    syncSlaveButton->setButtonText (TRANS("Sync as slave"));
    syncSlaveButton->addListener (this);


    //[UserPreSize]
    cwIpEditor->addListener(this);
    cwPortEditor->addListener(this);
    //[/UserPreSize]

    setSize (256, 320);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ConnectionComponent::~ConnectionComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    infoGroupComponent = nullptr;
    syncGroupComponent = nullptr;
    cwGroupComponent = nullptr;
    cwIpEditor = nullptr;
    cwPortEditor = nullptr;
    ipLabel = nullptr;
    portLabel = nullptr;
    confirmCwButton = nullptr;
    cStatusLabel = nullptr;
    syncMasterButton = nullptr;
    syncSlaveButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConnectionComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConnectionComponent::resized()
{
    infoGroupComponent->setBounds (0, 256, 240, 56);
    syncGroupComponent->setBounds (0, 144, 240, 104);
    cwGroupComponent->setBounds (0, 8, 240, 128);
    cwIpEditor->setBounds (104, 32, 120, 24);
    cwPortEditor->setBounds (104, 64, 120, 24);
    ipLabel->setBounds (8, 32, 56, 24);
    portLabel->setBounds (8, 64, 56, 24);
    confirmCwButton->setBounds (24, 96, 192, 24);
    cStatusLabel->setBounds (8, 272, 200, 24);
    syncMasterButton->setBounds (24, 168, 192, 24);
    syncSlaveButton->setBounds (24, 208, 192, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ConnectionComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == confirmCwButton)
    {
        //[UserButtonCode_confirmCwButton] -- add your button handler code here..
        // set new address (if valid):
        const std::string ip = cwIpEditor->getText().toStdString();
        const std::string port = cwPortEditor->getText().toStdString();
        getProcessor()->setCWonderAddress(ip, port);
        cwAddressIsBeingEdited_ = false;
        //[/UserButtonCode_confirmCwButton]
    }
    else if (buttonThatWasClicked == syncMasterButton)
    {
        //[UserButtonCode_syncMasterButton] -- add your button handler code here..
        getProcessor()->getSourceController()->syncAsMaster();
        //[/UserButtonCode_syncMasterButton]
    }
    else if (buttonThatWasClicked == syncSlaveButton)
    {
        //[UserButtonCode_syncSlaveButton] -- add your button handler code here..
        getProcessor()->getSourceController()->syncAsSlave();
        //[/UserButtonCode_syncSlaveButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ConnectionComponent::update()
{
    std::shared_ptr<wonder::SourceController> controller = getProcessor()->getSourceController();

    if(!cwAddressIsBeingEdited_){
        cwIpEditor->setText(controller->getCWonderHost(), false);
        cwPortEditor->setText(controller->getCWonderPort(), false);
    }

    cStatusLabel->setText(controller->connectionStatusString(), dontSendNotification);
    cStatusLabel->setColour(Label::textColourId, connectionColour(controller->connectionStatus()));
}

void ConnectionComponent::textEditorTextChanged(TextEditor& textEditor)
{
    if (&textEditor == cwIpEditor || &textEditor == cwPortEditor)
    {
        cwAddressIsBeingEdited_ = true;
    }
}

void ConnectionComponent::textEditorReturnKeyPressed(TextEditor &textEditor)
{
    if (&textEditor == cwIpEditor || &textEditor == cwPortEditor)
    {
        // behave as if confirm-button clicked:
        buttonClicked(confirmCwButton);
    }
}

Colour ConnectionComponent::connectionColour(wonder::ConnectionStates cStatus)
{
    switch (cStatus)
    {
        case wonder::inactive:  return Colours::grey;
        case wonder::active:    return Colours::green;
        case wonder::error:     return Colours::red;
    }
    return Colours::red;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ConnectionComponent" componentName=""
                 parentClasses="public wonderjuce::SpaopEditorComponent, public TextEditor::Listener"
                 constructorParams="SpaopAudioProcessor* ownerFilter" variableInitialisers="wonderjuce::SpaopEditorComponent(ownerFilter),&#10;cwAddressIsBeingEdited_(false)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="256" initialHeight="320">
  <BACKGROUND backgroundColour="ffff"/>
  <GROUPCOMPONENT name="new group" id="e4a6c62de9f5a13b" memberName="infoGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 256 240 56" title="Connection info"/>
  <GROUPCOMPONENT name="sync group" id="1f4b55c329762c13" memberName="syncGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 144 240 104" title="Sync"/>
  <GROUPCOMPONENT name="cwonder group" id="cedf29bc24c6bcd2" memberName="cwGroupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 8 240 128" title="cWONDER address"/>
  <TEXTEDITOR name="cwonder ip editor" id="3d9a2a0e21d3e69b" memberName="cwIpEditor"
              virtualName="" explicitFocusOrder="0" pos="104 32 120 24" initialText="???.???.???.???"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="cwonder port editor" id="4b70502607b0245f" memberName="cwPortEditor"
              virtualName="" explicitFocusOrder="0" pos="104 64 120 24" initialText="?????"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="ip label" id="3d57cecad65af09c" memberName="ipLabel" virtualName=""
         explicitFocusOrder="0" pos="8 32 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="IP:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="port label" id="384f7a92c5143010" memberName="portLabel"
         virtualName="" explicitFocusOrder="0" pos="8 64 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="confirm cwonder button" id="656f04da7959cb83" memberName="confirmCwButton"
              virtualName="" explicitFocusOrder="0" pos="24 96 192 24" buttonText="Confirm changes"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="c-status label" id="5f890d1c193d8cb5" memberName="cStatusLabel"
         virtualName="" explicitFocusOrder="0" pos="8 272 200 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Connection status" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="sync master button" id="916668e212bd5a5f" memberName="syncMasterButton"
              virtualName="" explicitFocusOrder="0" pos="24 168 192 24" buttonText="Sync as master"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sync slave button" id="1f881a201217874b" memberName="syncSlaveButton"
              virtualName="" explicitFocusOrder="0" pos="24 208 192 24" buttonText="Sync as slave"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
