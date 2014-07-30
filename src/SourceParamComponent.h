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

#ifndef __JUCE_HEADER_243013F0C1B216AA__
#define __JUCE_HEADER_243013F0C1B216AA__

//[Headers]     -- You can add your own extra header files here --
#include "PluginProcessor.h"
#include "SpaopEditorComponent.h"
#include "PluginEditor.h"
#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

    A component containing buttons, labels and textboxes to display and control
    the paramters of a WONDER source (apart from its position). This is to be
    used as a subcomponent of the main SpaopAudioProcessorEditor.

    This is generated using the WYSISWYG-Editor of the Introjucer app (which
    is why this text contains those comments tags...).

                                                                    //[/Comments]
*/
class SourceParamComponent  : public wonderjuce::SpaopEditorComponent,
                              public TextEditor::Listener,
                              public ChangeListener,
                              public SliderListener,
                              public ButtonListener
{
public:
    //==============================================================================
    SourceParamComponent (SpaopAudioProcessor* ownerFilter);
    ~SourceParamComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
private:
    void update() override;

    void showColourSelector();

    /** Recieves callbacks from the ColourSelector when the colour is changed. */
    void changeListenerCallback (ChangeBroadcaster* changeBroadcaster);

    void textEditorTextChanged (TextEditor& textEditor);

    void textEditorReturnKeyPressed (TextEditor& textEditor);

    void textEditorFocusLost (TextEditor& textEditor);

public:
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    bool nameIsBeingEdited_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> parametersGroup;
    ScopedPointer<GroupComponent> sourceGroup;
    ScopedPointer<Slider> idSlider;
    ScopedPointer<Label> idLabel;
    ScopedPointer<Label> angleLabel;
    ScopedPointer<ToggleButton> dopplerButton;
    ScopedPointer<ToggleButton> lockIDButton;
    ScopedPointer<TextEditor> nameEditor;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<ToggleButton> typeButton;
    ScopedPointer<TextButton> colourButton;
    ScopedPointer<Slider> angleSlider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceParamComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_243013F0C1B216AA__
