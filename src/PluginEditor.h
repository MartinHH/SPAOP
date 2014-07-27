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

#ifndef __JUCE_HEADER_BFAA1FA6C119914A__
#define __JUCE_HEADER_BFAA1FA6C119914A__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "SourceZoomPort.h"
#include "Source.h"
#include "WonderHeader.h"
#include "ConnectionComponent.h"

#ifndef GUI_REFRESH_TIME
#define GUI_REFRESH_TIME 100    /**< How often the GUI updates itself (in ms). */
#endif

#define COLOUR_SELECTOR_SIZE 200    /**< Size of the colour selection popup. */

#ifndef INITIAL_ZOOM_FACTOR
#define INITIAL_ZOOM_FACTOR ((COORD_MAX) > 10.0 ? (COORD_MAX) / 10.0 : 1.0)
                                    /**< Initial zoom factor. */
#endif
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]

    The main GUI class for the SPAOP plugin.

    This is generated using the WYSISWYG-Editor of the Introjucer app (which
    is why this text contains those comments tags...).

    @see [juce::AudioProcessorEditor]
        (http://www.juce.com/api/classAudioProcessorEditor.html)

                                                                    //[/Comments]
*/
class SpaopAudioProcessorEditor  : public AudioProcessorEditor,
                                   public Timer,
                                   public wonderjuce::SourcePanel::Listener,
                                   public TextEditor::Listener,
                                   public ChangeListener,
                                   public SliderListener,
                                   public ButtonListener
{
public:
    //==============================================================================
    SpaopAudioProcessorEditor (SpaopAudioProcessor* ownerFilter);
    ~SpaopAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

private:
    void timerCallback();

    void changeListenerCallback (ChangeBroadcaster* changeBroadcaster);

    void sourcePanelDragStarted (wonderjuce::SourcePanel*);

    void sourcePanelValuesChanged (wonderjuce::SourcePanel* panel);

    void sourcePanelDragEnded (wonderjuce::SourcePanel*);

    void textEditorTextChanged (TextEditor& textEditor);

    void textEditorReturnKeyPressed (TextEditor& textEditor);

    void textEditorFocusLost (TextEditor& textEditor);

    /** Returns the Processor. */
    SpaopAudioProcessor* getProcessor() const;

    /** Returns the Plugin's SourceController instance */
    wonder::SourceController* getSourceController() const;
    
    void showColourSelector();
    
public:
    /** Normalizes a slider's value to the range of [0.0 ; 1.0]. */
    static double normalizedValue(Slider* slider);
    
    /** Converts a ToggleButton's state to the range of [0.0 ; 1.0]. */
    static double normalizedValue(Button* button);
    
    static Colour connectionColour(wonder::ConnectionStates cStatus);
    
    /** Converts a wonder::Colour to a juce::Colour. */
    static juce::Colour juceColour(wonder::Colour colour);
    
    /** Converts a juce::Colour to a wonder::Colour. */
    static wonder::Colour wonderColour(juce::Colour colour);
    
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
    ScopedPointer<GroupComponent> displayGroup;
    ScopedPointer<GroupComponent> parametersGroup;
    ScopedPointer<GroupComponent> sourceGroup;
    ScopedPointer<GroupComponent> connectionGroup;
    ScopedPointer<Slider> xSlider;
    ScopedPointer<Slider> ySlider;
    ScopedPointer<Slider> idSlider;
    ScopedPointer<Label> idLabel;
    ScopedPointer<Label> angleLabel;
    ScopedPointer<ToggleButton> dopplerButton;
    ScopedPointer<ToggleButton> lockIDButton;
    ScopedPointer<ToggleButton> linkWonderButton;
    ScopedPointer<Slider> zoomSlider;
    ScopedPointer<Label> yPosLabel;
    ScopedPointer<Label> xPosLabel;
    ScopedPointer<Label> zoomLabel;
    ScopedPointer<Label> addrLabel;
    ScopedPointer<Label> urlLabel;
    ScopedPointer<Label> cStatusLabel;
    ScopedPointer<TextEditor> nameEditor;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<ToggleButton> typeButton;
    ScopedPointer<TextButton> colourButton;
    ScopedPointer<ToggleButton> showOthersButton;
    ScopedPointer<wonderjuce::SourceZoomPort> sourceZoomPort;
    ScopedPointer<Slider> angleSlider;
    ScopedPointer<ToggleButton> showNamesButton;
    ScopedPointer<TabbedComponent> tabbedComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaopAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_BFAA1FA6C119914A__
