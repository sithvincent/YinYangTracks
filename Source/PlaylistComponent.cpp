/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 9 Feb 2023 8:26:34pm
    Author:  Vincent Chong Yi Qin

  ==============================================================================
*/

#include <string.h>
#include <JuceHeader.h>
#include "PlaylistComponent.h"

//======================CONSTRUCTOR AND DESTRUCTOR===========================
PlaylistComponent::PlaylistComponent(): loadMusicIntoDeck(false),
                                        showFiltered(false)
{

    // ColumnID must start with one (somehow can't have 'getheight')

    tableComponent.getHeader().addColumn("ID", 1, 100);
    tableComponent.getHeader().addColumn("Track Title", 2, 300);
    tableComponent.getHeader().addColumn("Song Length", 3, 100);
    tableComponent.getHeader().addColumn("Send to Deck 1", 4, 150);
    tableComponent.getHeader().addColumn("Send to Deck 2", 5, 150);

    // 'This' refers to playlistComponent which already has a TableListBoxModel.
    // It registers the former and by extension the model with its functions with tableComponent.
    tableComponent.setModel(this);
    // The table is an object within PlaylistComponent. 
    addAndMakeVisible(tableComponent);

    // Add the searchbox that allows the user to type in the keyword to filter library entries
    addAndMakeVisible(searchBox);
    searchBox.setJustification(Justification::centred);
    searchBox.setText("Type in the name of the track you want, then click Search.", juce::dontSendNotification);
    searchBox.setSelectAllWhenFocused(true);

    // Add the Search button that filters library entries based on the words in the search box
    addAndMakeVisible(searchButton);
    searchButton.addListener(this);

    // Adds the button that clears the selection and shows the unfiltered library
    addAndMakeVisible(clearSearchButton);
    clearSearchButton.addListener(this);
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
    double padding = getHeight() / 10;
    tableComponent.setBounds(0, 0+padding, getWidth(), getHeight()-padding); 
    searchBox.setBounds(0, 0, getWidth()*0.75, padding);
    searchButton.setBounds(getWidth() * 0.75, 0, getWidth()*0.1, padding);
    clearSearchButton.setBounds(getWidth() * 0.85, 0, getWidth() * 0.15, padding);
}


//===============Functions inherited from TableListBoxModel======================

// Ensures there are sufficient rows 
int PlaylistComponent::getNumRows() {
    if (showFiltered) {
        return tempTrackTitles.size();
    }
    else {
        return trackTitles.size();
    }
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
    if (columnId == 1) {
        g.drawText(String(rowNumber), 2, 0, width - 4, height, Justification::centredLeft, true);    
    }
    else if (columnId == 2) {
        if (showFiltered) {
            g.drawText(tempTrackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
        }
        else {
            g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
        }
    }
    else if (columnId == 3) {
        if (showFiltered) {
            g.drawText(String(tempTrackLengths[rowNumber]), 2, 0, width - 4, height, Justification::centred, true);
        }
        else {
            g.drawText(String(trackLengths[rowNumber]), 2, 0, width - 4, height, Justification::centred, true);        
        }
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
            TextButton * btn1 = new TextButton("Deck 1");
            existingComponentToUpdate = btn1;
            btn1->addListener(this);
            // See purpose of these two lines in buttonClicked below.
            String id1{ std::to_string(rowNumber) };
            btn1->setComponentID(id1);
            btn1->setDescription("1");
        }
    }
    else if (columnId == 5)
    {
        // If there are no existing components (thus a nullptr), add a ptr to a button.
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn2 = new TextButton("Deck 2");
            existingComponentToUpdate = btn2;
            btn2->addListener(this);
            String id2{ std::to_string(rowNumber) };
            btn2->setComponentID(id2);
            btn2->setDescription("2");
        }
    }
    // Returns the existingComponentToUpdate which may be modified in some places (when columnid=2)
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &searchButton) {
        DBG(searchBox.getText().toStdString());
        DBG("This triggers");
        filterEntry(searchBox.getText());
    }
    else if (button == &clearSearchButton) {
        DBG("Search should be cleared");
        clearSearch();
    }
    // Controls the buttons that add the selected music to Decks 1 or 2
    else {
        int id = std::stoi(button->getComponentID().toStdString());
        // The two key things deciding what to load onto the deck
        urlToLoad = trackURLs[id];
        deckToLoad = std::stoi(button->getDescription().toStdString());
        loadMusicIntoDeck = true;
        DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
    }
    // we do not have class data members storing the Buttons, we are
    // creating them dynamically, then losing access to them. So no memory address to refer to.
    // Thus we have to do this implementation to know which button is clicked on.
  
}


void PlaylistComponent::addEntry(String trackTitle, double trackLength, URL audioURL)
{
    trackTitles.push_back(trackTitle);
    trackLengths.push_back(trackLength);
    trackURLs.push_back(audioURL);
    // Updates the table based on these new information
    tableComponent.updateContent();
}


void PlaylistComponent::filterEntry(String searchEntry)
{
    // The first for loop activates when even one entry is found
    // The purpose is to move all tracks in the library to a temporary track
    // And then emptying it to fill with the filtered tracks
    for (int i = 0; i < trackTitles.size(); i++) {
        if (trackTitles[i].toStdString().find(searchEntry.toStdString()) != std::string::npos) {
            DBG("Found");
            showFiltered = true;
            tempTrackTitles.push_back(trackTitles[i]);
            tempTrackLengths.push_back(trackLengths[i]);
            tempTrackURLs.push_back(trackURLs[i]);            
        }
    }
    // Updates the table based on these new information
    tableComponent.updateContent();
}


void PlaylistComponent::clearSearch() {
    tempTrackTitles.clear();
    tempTrackLengths.clear();
    tempTrackURLs.clear();
    showFiltered = false;
    searchBox.setText("Type in the name of the track you want to filter, then click Search.", juce::dontSendNotification);
    tableComponent.updateContent();
}

//void PlaylistComponent::printTitles() {
//    DBG("Total number of track entries: " << trackTitles.size());
//    DBG("Total number of time entries: " << trackLengths.size());
//    for (double &e: trackLengths) {
//        DBG(e);
//    }
//}