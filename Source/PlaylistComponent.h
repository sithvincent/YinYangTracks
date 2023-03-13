#pragma once

#include <JuceHeader.h>
#include <vector>


class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener
{
    public:
        //===============Constructor and Background Draw Function ======================
        PlaylistComponent();
        ~PlaylistComponent() override;

        // Draw functions
        void paint (juce::Graphics&) override;
        void resized() override;


        //===============Functions inherited from TableListBoxModel======================
        
        // Returns number of rows in the table.
        int getNumRows() override;

        // Paints each row of the table (row background). Arguments are 'auto-passed in'.
        void paintRowBackground(Graphics &,
                                int rowNumber,
                                int width,
                                int height,
                                bool rowIsSelected) override;

        // Paints/fills up the individual cell in each row. Arguments are 'auto-passed in'.
        void paintCell( Graphics &,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) override;

        // Dictates what happens when you click on a cell. Non abstract.
        void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;


        //=============== UPDATING OF NEW ENTRIES AND REDRAW ======================

        // Create or update a custom component in every cell. Non abstract.
        // Called whenever a custom component might need to be updated e.g.when the table is changed.
        // Pointer to the component of each cell (existingComponentToUpdate) is auto passed in.
        Component* refreshComponentForCell( int rowNumber,
                                            int columnId,
                                            bool isRowSelected,
                                            Component* existingComponentToUpdate) override;

        // Adds a song entry into the library
        void addEntry(String, double, URL);

        // Filters what the library shows based on search criteria
        void filterEntry(String);

        // Filters what the library shows based on search criteria
        void clearSearch();

        // The URL of the music in the library to be loaded into the deck. 
        // Must be public because DeckGUI will access this.
        URL urlToLoad;
        int deckToLoad;
        // Tells DeckGUI whether there is music to be loaded into the deck.
        bool loadMusicIntoDeck;

        bool showFiltered;

        // Dictates what to do when a button is clicked.
        void buttonClicked(Button* button) override;



    private:

        // Main table object, uses TableListBoxModel helper functions to get information about
        // the data for the table and the way that data should be drawn. 
        // Need to register TableListBoxModel functions with this component.
        TableListBox tableComponent;

        // Vector to store the tracktitles to be displayed
        std::vector <String> trackTitles;
        std::vector <double> trackLengths;
        std::vector <URL> trackURLs;

        // Vectors to store in the background all files loaded into the library previously
        // while the library displays the filtered list of music from search
        std::vector <String> tempTrackTitles;
        std::vector <double> tempTrackLengths;
        std::vector <URL> tempTrackURLs;

        // Creates a searchbox where user can key in values
        TextEditor searchBox;
        // Creates a searchbutton where user can initialize search
        TextButton searchButton{ "SEARCH" };
        // Creates a clearSearch button where user can clear search
        TextButton clearSearchButton{ "CLEAR SEARCH" };

        // MACRO
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
