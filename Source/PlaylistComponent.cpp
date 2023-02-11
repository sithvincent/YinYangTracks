/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 9 Feb 2023 8:26:34pm
    Author:  Vincent Chong Yi Qin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//======================CONSTRUCTOR AND DESTRUCTOR===========================
PlaylistComponent::PlaylistComponent()
{
    trackTitles.push_back("Tracking");
    trackTitles.push_back("Lo2e");


    // ColumnID must start with one (somehow can't have 'getheight')
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("Artist", 2, 400);
    // 'This' refers to playlistComponent which already has a TableListBoxModel.
    // It registers the former and by extension the model with its functions with tableComponent.
    tableComponent.setModel(this);
    // The table is an object within PlaylistComponent. 
    addAndMakeVisible(tableComponent);
    
}

PlaylistComponent::~PlaylistComponent()
{
}


//======================PAINT AND GENERAL GUI===========================
void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Playlist", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}


void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, 0, getWidth(), getHeight()); 
}


//===============Functions inherited from TableListBoxModel======================

// Ensures there are sufficient rows 
int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground( Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected) 
{
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(  Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected) 
{
    g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent&) {

}



Component* PlaylistComponent::refreshComponentForCell(int rowNumber, 
                                                      int columnId, 
                                                      bool isRowSelected, 
                                                      Component* existingComponentToUpdate)
{
    // Implement button in second column (remember that numbering starts from 1 in 6.1 onwards)
    if (columnId == 2)
    {
        // If there are no existing components (thus a nullptr), add a ptr to a button.
        if (existingComponentToUpdate == nullptr)
        {            
            // This function can be continuously called so we don't want a new btn to be created every time
            // A pointer to an existing initialized object would do.
            TextButton * btn = new TextButton("Play");
            existingComponentToUpdate = btn;
            btn->addListener(this);
            // See purpose of these two lines in buttonClicked below.
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
        }
    }
    // Returns the existingComponentToUpdate which may be modified in some places (when columnid=2)
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    // we do not have class data members storing the Buttons, we are
    // creating them dynamically, then losing access to them. So no memory address to refer to.
    // Thus we have to do this implementation to know which button is clicked on.
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
}