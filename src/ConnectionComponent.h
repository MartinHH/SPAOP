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

#ifndef __JUCE_HEADER_A6D0D14F17594572__
#define __JUCE_HEADER_A6D0D14F17594572__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "Parsetools.h"
#include "SpaopEditorComponent.h"
#include "SourceController.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

    A component containing buttons, labels and textboxes to display and control
    SPAOP's connection state and setting. This is to be used as a subcomponent
    of the main SpaopAudioProcessorEditor.

    This is generated using the WYSISWYG-Editor of the Introjucer app (which
    is why this text contains those comments tags...).

                                                                    //[/Comments]
*/
class ConnectionComponent  : public wonderjuce::SpaopEditorComponent,
                             public TextEditor::Listener,
                             public ButtonListener
{
public:
    //==============================================================================
    ConnectionComponent (SpaopAudioProcessor* ownerFilter);
    ~ConnectionComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void update() override;

    void textEditorTextChanged(TextEditor& textEditor);

    void textEditorReturnKeyPressed(TextEditor& textEditor);

    static Colour connectionColour(wonder::ConnectionStates cStatus);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    bool cwAddressIsBeingEdited_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> infoGroupComponent;
    ScopedPointer<GroupComponent> modeGroupComponent;
    ScopedPointer<GroupComponent> cwGroupComponent;
    ScopedPointer<TextEditor> cwIpEditor;
    ScopedPointer<TextEditor> cwPortEditor;
    ScopedPointer<Label> ipLabel;
    ScopedPointer<Label> portLabel;
    ScopedPointer<TextButton> confirmCwButton;
    ScopedPointer<ToggleButton> linkWonderButton;
    ScopedPointer<Label> addrLabel;
    ScopedPointer<Label> urlLabel;
    ScopedPointer<Label> cStatusLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectionComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_A6D0D14F17594572__
