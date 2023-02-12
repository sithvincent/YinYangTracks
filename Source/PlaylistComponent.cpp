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
PlaylistComponent::PlaylistComponent(): loadMusicIntoDeck(false)
{

    // ColumnID must start with one (somehow can't have 'getheight')
    //int(getWidth() / 8)-1
    //    int(getWidth() / 2)-1
    //    int(getWidth() * 2 / 8)-1
    tableComponent.getHeader().addColumn("ID", 1, 100);
    tableComponent.getHeader().addColumn("Track Title", 2, 400);
    tableComponent.getHeader().addColumn("Song Length", 3, 100);
    tableComponent.getHeader().addColumn("Send to Deck", 4, 200);

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
    if (columnId == 2) {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    else if (columnId == 3) {
        g.drawText(String(trackLengths[rowNumber]), 2, 0, width - 4, height, Justification::centred, true);
    }
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent&) {
    DBG("Cell clicked huh.");

}



Component* PlaylistComponent::refreshComponentForCell(int rowNumber, 
                                                      int columnId, 
                                                      bool isRowSelected, 
                                                      Component* existingComponentToUpdate)
{
    // Implement button in second column (remember that numbering starts from 1 in 6.1 onwards)
    if (columnId == 4)
    {
        // If there are no existing components (thus a nullptr), add a ptr to a button.
        if (existingComponentToUpdate == nullptr)
        {            
            // This function can be continuously called so we don't want a new btn to be created every time
            // A pointer to an existing initialized object would do.
            TextButton * btn1 = new TextButton("Add to Deck 1");
            existingComponentToUpdate = btn1;
            btn1->addListener(this);
            // See purpose of these two lines in buttonClicked below.
            String id1{ std::to_string(rowNumber) };
            btn1->setComponentID(id1);

            //TextButton* btn2 = new TextButton("Add to Deck 2");
            //existingComponentToUpdate = btn2;
            //btn2->addListener(this);
            //// See purpose of these two lines in buttonClicked below.
            //String id2{ std::to_string(rowNumber) };
            //btn2->setComponentID(id2);
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
    urlToLoad = trackURLs[id];
    loadMusicIntoDeck = true;
    DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
}


void PlaylistComponent::addEntry(String trackTitle, double trackLength, URL audioURL)
{
    trackTitles.push_back(trackTitle);
    trackLengths.push_back(trackLength);
    trackURLs.push_back(audioURL);
    // Updates the table based on these new information
    tableComponent.updateContent();
}

void PlaylistComponent::printTitles() {
    DBG("Total number of track entries: " << trackTitles.size());
    DBG("Total number of time entries: " << trackLengths.size());
    for (double &e: trackLengths) {
        DBG(e);
    }
}