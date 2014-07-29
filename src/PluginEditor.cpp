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

    addAndMakeVisible (showOthersButton = new ToggleButton ("show others button"));
    showOthersButton->setButtonText (TRANS("Show other sources"));
    showOthersButton->addListener (this);
    showOthersButton->setToggleState (true, dontSendNotification);

    addAndMakeVisible (sourceZoomPort = new wonderjuce::SourceZoomPort ("source zoomport"));

    addAndMakeVisible (showNamesButton = new ToggleButton ("show names button"));
    showNamesButton->setButtonText (TRANS("Show source names"));
    showNamesButton->addListener (this);

    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (30);
    tabbedComponent->addTab (TRANS("WFS Source"), Colour (0xff777777), new SourceParamComponent (ownerFilter), true);
    tabbedComponent->addTab (TRANS("Connection"), Colour (0xff777777), new ConnectionComponent (ownerFilter), true);
    tabbedComponent->setCurrentTabIndex (0);


    //[UserPreSize]
    wonderjuce::SourcePanel* sourcePanel = sourceZoomPort->getSourcePanel();
    sourcePanel->addListener(this);
    sourcePanel->setSources(ownerFilter->getSourceController()->getSources());
    sourcePanel->setRoom(ownerFilter->getSourceController()->getRoom());

    // Since the sliders were auto-generated with the Introjucer which does
    // not allow using defines, we have to override the settings from above:
    xSlider->setRange (COORD_MIN, COORD_MAX, COORD_PRECISION);
    ySlider->setRange (COORD_MIN, COORD_MAX, COORD_PRECISION);

    //[/UserPreSize]

    setSize (728, 490);


    //[Constructor] You can add your own custom stuff here..
    zoomSlider->setValue(ownerFilter->getZoomFactor());
    sourceZoomPort->setZoomFactor(ownerFilter->getZoomFactor());

    startTimer(GUI_REFRESH_TIME);
    //[/Constructor]
}

SpaopAudioProcessorEditor::~SpaopAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    displayGroup = nullptr;
    xSlider = nullptr;
    ySlider = nullptr;
    zoomSlider = nullptr;
    yPosLabel = nullptr;
    xPosLabel = nullptr;
    zoomLabel = nullptr;
    showOthersButton = nullptr;
    sourceZoomPort = nullptr;
    showNamesButton = nullptr;
    tabbedComponent = nullptr;


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
    displayGroup->setBounds (472, 384, 240, 96);
    xSlider->setBounds (112, 408, 100, 71);
    ySlider->setBounds (280, 408, 100, 71);
    zoomSlider->setBounds (560, 448, 136, 24);
    yPosLabel->setBounds (256, 456, 48, 24);
    xPosLabel->setBounds (88, 456, 48, 24);
    zoomLabel->setBounds (480, 448, 48, 24);
    showOthersButton->setBounds (488, 400, 208, 24);
    sourceZoomPort->setBounds (24, 16, 432, 432);
    showNamesButton->setBounds (488, 424, 208, 24);
    tabbedComponent->setBounds (464, 16, 256, 352);
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
    else if (sliderThatWasMoved == zoomSlider)
    {
        //[UserSliderCode_zoomSlider] -- add your slider handling code here..
        getProcessor()->setZoomFactor(zoomSlider->getValue());
        //[/UserSliderCode_zoomSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SpaopAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == showOthersButton)
    {
        //[UserButtonCode_showOthersButton] -- add your button handler code here..
        getProcessor()->setShowOtherSources(showOthersButton->getToggleState());
        //[/UserButtonCode_showOthersButton]
    }
    else if (buttonThatWasClicked == showNamesButton)
    {
        //[UserButtonCode_showNamesButton] -- add your button handler code here..
        getProcessor()->setShowNames(showNamesButton->getToggleState());
        //[/UserButtonCode_showNamesButton]
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

    // Sliders:
    xSlider->setValue(source.getXPos());
    ySlider->setValue(source.getYPos());
    zoomSlider->setValue(getProcessor()->getZoomFactor());

    // ToggleButtons:
    showOthersButton->setToggleState(getProcessor()->showOtherSources(),
                                     dontSendNotification);
    showNamesButton->setToggleState(getProcessor()->showNames(), dontSendNotification);

    sourceZoomPort->setZoomFactor(getProcessor()->getZoomFactor());

    wonderjuce::SourcePanel* sourcePanel = sourceZoomPort->getSourcePanel();
    sourcePanel->setSource(source.getID());
    sourcePanel->setShowOtherSources(getProcessor()->showOtherSources());
    sourcePanel->setShowNames(getProcessor()->showNames());

    // update the tabbed subcomponents:
    for (int i = 0; i < tabbedComponent->getNumTabs(); i++){
        wonderjuce::SpaopEditorComponent* c =
            static_cast<wonderjuce::SpaopEditorComponent*> (tabbedComponent->getTabContentComponent(i));
        c->update();
    }

}

void SpaopAudioProcessorEditor::sourcePanelDragStarted(wonderjuce::SourcePanel *panel)
{
    if(panel == sourceZoomPort->getSourcePanel())
    {
//        AudioProcessor* processor = getProcessor();
//
//        processor->beginParameterChangeGesture(wonder::Source::xPosParam);
//        processor->beginParameterChangeGesture(wonder::Source::yPosParam);

        // ^^That is how it should be done, but it is deactivated because of this bug:
        // http://www.juce.com/forum/topic/vst3-parameterchangegesture-problem-cubase-75
    }
}

void SpaopAudioProcessorEditor::sourcePanelValuesChanged(wonderjuce::SourcePanel *panel)
{
    if(panel == sourceZoomPort->getSourcePanel())
    {
        SpaopAudioProcessor* processor = getProcessor();

        processor->setCoordinatesNotifyingHost(panel->getXPos(), panel->getYPos());
    }
}

void SpaopAudioProcessorEditor::sourcePanelDragEnded(wonderjuce::SourcePanel *panel)
{
    if(panel == sourceZoomPort->getSourcePanel())
    {
//        AudioProcessor* processor = getProcessor();
//
//        processor->endParameterChangeGesture(wonder::Source::xPosParam);
//        processor->endParameterChangeGesture(wonder::Source::yPosParam);

        // ^^That is how it should be done, but it is deactivated because of this bug:
        // http://www.juce.com/forum/topic/vst3-parameterchangegesture-problem-cubase-75
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

Colour SpaopAudioProcessorEditor::juceColour(wonder::Colour colour)
{
    return Colour(colour.r, colour.g, colour.b);
}

wonder::Colour SpaopAudioProcessorEditor::wonderColour(Colour colour)
{
    return wonder::Colour(colour.getRed(), colour.getGreen(), colour.getBlue());
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpaopAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer, public wonderjuce::SourcePanel::Listener"
                 constructorParams="SpaopAudioProcessor* ownerFilter" variableInitialisers="AudioProcessorEditor(ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="728" initialHeight="490">
  <BACKGROUND backgroundColour="ff787878"/>
  <GROUPCOMPONENT name="display group" id="e866147c42bab00a" memberName="displayGroup"
                  virtualName="" explicitFocusOrder="0" pos="472 384 240 96" title="Display"/>
  <SLIDER name="xCoord" id="a58b89fa9741293f" memberName="xSlider" virtualName=""
          explicitFocusOrder="0" pos="112 408 100 71" min="-100" max="100"
          int="0.010000000000000000208" style="LinearHorizontal" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="yCoord" id="26d1d3281f771894" memberName="ySlider" virtualName=""
          explicitFocusOrder="0" pos="280 408 100 71" min="-100" max="100"
          int="0.010000000000000000208" style="LinearHorizontal" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="zoom slider" id="2e49c59dd84b651a" memberName="zoomSlider"
          virtualName="" explicitFocusOrder="0" pos="560 448 136 24" min="1"
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
  <TOGGLEBUTTON name="show others button" id="33f3e82300ebf110" memberName="showOthersButton"
                virtualName="" explicitFocusOrder="0" pos="488 400 208 24" buttonText="Show other sources"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <VIEWPORT name="source zoomport" id="addc41de081c2973" memberName="sourceZoomPort"
            virtualName="wonderjuce::SourceZoomPort" explicitFocusOrder="0"
            pos="24 16 432 432" vscroll="1" hscroll="1" scrollbarThickness="18"
            contentType="0" jucerFile="" contentClass="wonderjuce::SourcePanel"
            constructorParams=""/>
  <TOGGLEBUTTON name="show names button" id="77a4541a43d0f171" memberName="showNamesButton"
                virtualName="" explicitFocusOrder="0" pos="488 424 208 24" buttonText="Show source names"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TABBEDCOMPONENT name="new tabbed component" id="386f797096875f80" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="464 16 256 352" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="WFS Source" colour="ff777777" useJucerComp="0" contentClassName="SourceParamComponent"
         constructorParams="ownerFilter" jucerComponentFile=""/>
    <TAB name="Connection" colour="ff777777" useJucerComp="0" contentClassName="ConnectionComponent"
         constructorParams="ownerFilter" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
